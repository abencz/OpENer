/*******************************************************************************
 * Copyright (c) 2009, Rockwell Automation, Inc.
 * All rights reserved. 
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


#include "opener_user_conf.h"
#include <opener_api.h>
#include "networkhandler.h"
#include <encap.h>
#include <cipconnectionmanager.h>
#include <endianconv.h> 
#include <trace.h>

/* communication buffer */
EIP_UINT8 g_acPCEthernetCommBuffer[PC_OPENER_ETHERNET_BUFFER_SIZE];

static struct tcp_pcb *TCPlistener;
static struct udp_pcb *UDPlistener;

#define MAX_NO_OF_TCP_SOCKETS 10

/*!< This var holds the TCP socket the received to last explicit message.
* It is needed for opening point to point connection to determine the peer's
* address.
*/
static struct tcp_pcb *g_pCurrentActiveTCPSocket;


// UDP UNSOLICITED DATA RECEIVE CALLBACK
// The callback function is responsible for deallocating the pbuf.

void udp_unsolicited_receive_callback(void * arg,			// arg specified when the callback was registered
    struct udp_pcb * pcb,						// pcb handling the receive
    struct pbuf * p,							// the packet
    struct ip_addr * addr,						// source IP address
    u16_t port)								// source UDP port number
  {
    EIP_UINT8 *rxp;							// pointer into the receive buf
    int rxlen; 								// size of the received message
    int bytesleft; 							// bytes left (allows for multiple messages per packet)
    int replylen;
    err_t status;
    struct pbuf * r;							// reply buffer

    OPENER_ASSERT(p->tot_len<=sizeof(g_acPCEthernetCommBuffer));	// TODO this needs to be hardened
    pbuf_copy_partial(p, g_acPCEthernetCommBuffer, sizeof(g_acPCEthernetCommBuffer), 0);	// copy the packet into a contiguous receive buffer
    rxlen = p->tot_len;					// size of the received message
    pbuf_free(p);							// we no longer need the packet buffer

    OPENER_TRACE_INFO("Data received on UDP\n");

    rxp = &g_acPCEthernetCommBuffer[0];					// point to the start of the message
    do
      {
	replylen = handleReceivedExplictData((unsigned)pcb, rxp, rxlen, &bytesleft);	// bogus a socket fd (use the pcb address cast to an int)

	rxp += rxlen - bytesleft;					// bump the buffer pointer by the amount of data that was eaten
	rxlen = bytesleft;      					// dec the data size by the same amount

	if (replylen > 0)
	  {
	    OPENER_TRACE_INFO("reply sent\n");

	    r = pbuf_alloc(PBUF_TRANSPORT, 0, PBUF_REF);
	    OPENER_ASSERT(r != 0);     					// TODO harden
	    r->payload = &g_acPCEthernetCommBuffer[0];
	    r->len = r->tot_len = replylen;
	    status = udp_send(pcb, r);
	    if(status)OPENER_TRACE_ERR("networkhandler: UDP response was not fully sent\n");
	    pbuf_free(r);       					// reference counting makes sure the header does not get freed prematurely, but what about the replybuf?
	  }
      } while (rxlen > 0);
  }


// UDP CONNECTED DATA CALLBACK

void udp_registered_receive_callback(void * arg,			// arg specified when the callback was registered
    struct udp_pcb * pcb,       					// pcb handling the receive
    struct pbuf * pbuf, 						// the packet
    struct ip_addr * addr,      					// source IP address
    u16_t port) 							// source UDP port number
  {
    EIP_UINT8 *rxp;     						// pointer into the receive buf
    int rxlen;  							// size of the received message

    OPENER_ASSERT(pbuf->len==pbuf->tot_len);
    OPENER_ASSERT(pbuf->tot_len<=sizeof(g_acPCEthernetCommBuffer));       	// TODO this needs to be hardened
    rxlen = pbuf_copy_partial(pbuf, g_acPCEthernetCommBuffer, PC_OPENER_ETHERNET_BUFFER_SIZE, 0);   	// copy the packet into a contiguous receive buffer
    OPENER_ASSERT(rxlen==pbuf->tot_len);
    rxp = &g_acPCEthernetCommBuffer[0];    						// point to the start of the message
    pbuf_free(pbuf);    						// we no longer need the packet buffer

    if (rxlen == 0)
      {
	OPENER_TRACE_INFO("connection closed by client\n");
	udp_disconnect(pcb);					    	// close socket
	udp_remove(pcb);
	return;
      }
    if (rxlen <= 0)
      {
	OPENER_TRACE_INFO("networkhandler: error on recv");
	udp_disconnect(pcb);						// close socket
	udp_remove(pcb);
	return;
      }

    handleReceivedConnectedData(g_acPCEthernetCommBuffer, rxlen);
  }

// TCP DATA SENT CALLBACK

err_t tcp_sent_callback(void * arg, struct tcp_pcb * tpcb, u16_t len)
  {
    // TODO unlock the reply buf?

    return ERR_OK;
  }

// TCP DATA RECEIVE CALLBACK

err_t tcp_receive_callback(void * arg,  				// arg specified earlier
    struct tcp_pcb *pcb,						// pcb that is delivering the data
    struct pbuf *pbuf,  						// the packet
    err_t err)  							// TCP uses this to tell us what's happening to the connection perhaps?
  {
    EIP_UINT8 *rxp;
    int rxlen;
    int bytesread;
    int bytesleft;
    int replylen;
    int txspace;
    int rxoff=0;
    err_t status;

    if (err != ERR_OK)
      return err;       						// don't try to receive if error

    if (pbuf==0)							// check if connection is closing
      {
	tcp_close(pcb); 						// close out end
	return ERR_OK;
      }

    while (rxoff < pbuf->tot_len)
      {
	rxlen = pbuf_copy_partial(pbuf, g_acPCEthernetCommBuffer, 4, rxoff); // copy the first four words into the contiguous receive buffer
	OPENER_ASSERT(rxlen==4);       					//need at least four bytes of the header at this point
	rxp = &g_acPCEthernetCommBuffer[2];				// at this place EIP stores the data length
	rxlen= ltohs(&rxp) + ENCAPSULATION_HEADER_LENGTH - 4;   	// -4 is for the 4 bytes we have already read
									// (NOTE this advances the buffer pointer)


	if (rxlen+4 > PC_OPENER_ETHERNET_BUFFER_SIZE)
	  {     							//TODO can this be handled in a better way?
	    printf("too large packet received will be ignored\n");      // this may corrupt the connection ???
	    return EIP_ERROR;
	  }

	bytesread = pbuf_copy_partial(pbuf, &g_acPCEthernetCommBuffer[4], rxlen, rxoff+4); // copy the rest of the message into the contiguous receive buffer
        OPENER_ASSERT(bytesread == rxlen);
	rxlen += 4;
	rxoff += rxlen;
	tcp_recved(pcb, rxlen); 					// tell TCP we have received the data

	OPENER_TRACE_INFO("Data received on tcp\n");

	g_pCurrentActiveTCPSocket = pcb;

	replylen = handleReceivedExplictData((unsigned)pcb, g_acPCEthernetCommBuffer, rxlen, &bytesleft);
									// bogus a socket fd (use the pcb address cast to an int) -- I do not think this is used anywhere
	    
	g_pCurrentActiveTCPSocket = (struct tcp_pcb *)-1;
	
	if(bytesleft != 0)
	  {
	    OPENER_TRACE_WARN("Warning: received packet was to long: %d Bytes left!\n", bytesleft);
	  }

	if (replylen > 0)
	  {
	    OPENER_TRACE_INFO("reply sent\n");
	    txspace = tcp_sndbuf(pcb);  				// see how much data can be sent
	    OPENER_ASSERT(txspace >= replylen);				// TODO harden this

	    status = tcp_write(pcb, g_acPCEthernetCommBuffer, replylen, TCP_WRITE_FLAG_COPY); // TODO need to examine serial reuse of the reply buf
	    if (status != ERR_OK)
	      {
		OPENER_TRACE_INFO("TCP response was not sent OK: %d\n", status);
	      }
	    tcp_output(pcb);    					// push the data out
	  }
      }

    pbuf_free(pbuf);    						// we no longer need the packet buffer

    return ERR_OK;
  }

// TCP CONNECTION ACCEPT CALLBACK

err_t tcp_connection_accept_callback(void *arg, 			// arg registered for this listener (ignored in this case)
    struct tcp_pcb *newpcb,     					// pcb for new tcp connection
    err_t err)
  {
    OPENER_TRACE_INFO("networkhandler: new TCP connection\n");

    tcp_accepted(TCPlistener);  					// tell the listener that the call was accepted

    tcp_recv(newpcb, tcp_receive_callback);     			// set the callback for received data on the new connection
    tcp_sent(newpcb, tcp_sent_callback);				// set the callback for transmitted data on the new connection

    return ERR_OK;
  }

/* INT8 Start_NetworkHandler()
 * 	start a TCP listening socket, accept connections, receive data in select loop, call manageConnections periodicaly.
 * 	return status
 * 			-1 .. error
 */

EIP_STATUS Start_NetworkHandler()
  {
    err_t status;

    TCPlistener = tcp_new();					    /* create a new TCP listener socket */
    OPENER_ASSERT(TCPlistener!=0);
    status = tcp_bind(TCPlistener, IP_ADDR_ANY, OPENER_ETHERNET_PORT);
    OPENER_ASSERT(status==0);
    TCPlistener = tcp_listen(TCPlistener);
    OPENER_ASSERT(TCPlistener!=0);
    tcp_accept(TCPlistener, tcp_connection_accept_callback);

    UDPlistener = udp_new();					    /* create a new UDP listener socket */
    OPENER_ASSERT(UDPlistener!=0);
    udp_bind(UDPlistener, IP_ADDR_ANY, OPENER_ETHERNET_PORT);
    udp_recv(UDPlistener, udp_unsolicited_receive_callback, 0); 	// set the callback

    return EIP_OK;
  }


// this gets called every 10 ms from the timer handler
void CIPtick(void)
  {
    /* call manage_connections() in connection manager every TIMERTICK ms */
  manageConnections();
  }


/* INT8 sendUDPData(struct sockaddr_in pa_addr, int sockfd, INT8 *pa_data, UINT16 pa_datalength)
 * send udp datagram to pa_addr on socket sockfd.
 * 	 pa_addr	remote address
 * 	 sockfd		sockethandle
 * 	 pa_data	pointer to data which will be sent
 * 	 pa_datalength	length of data ind pa_data
 *
 * return status	0 .. success
 * 			-1 .. error
 */

EIP_STATUS
IApp_SendUDPData(struct sockaddr_in *pa_addr, int sockfd,
    EIP_UINT8 *pa_data, EIP_UINT16 pa_datalength)
  {
    struct pbuf *r;
    err_t status;
    struct udp_pcb *pcb;

    pcb = (struct udp_pcb *)sockfd;
    r = pbuf_alloc(PBUF_TRANSPORT, 0, PBUF_REF);
    if(r==0)
	{
	OPENER_TRACE_ERR("cannot alloc idp_pcb in IApp_SendUDPData\n");
	return EIP_ERROR;
	}
    r->payload = pa_data;
    r->len = r->tot_len = pa_datalength;
    status = udp_sendto(pcb, r, (struct ip_addr *)&pa_addr->sin_addr, htons(pa_addr->sin_port));
    if(status)OPENER_TRACE_ERR("networkhandler: error with sendto in IApp_SendUDPData\n");
    pbuf_free(r);							// reference counting makes sure the header does not get freed prematurely, but what about the replybuf?

    return EIP_OK;
  }

// create a new UDP socket for the connection manager
// returns the fd if successful, else -1
// the my_addr param is used to input the local address to bind the port to
// then it is used to return the address of the peer

int 
IApp_CreateUDPSocket(int pa_direction,   				// direction: CONSUMING or PRODUCING
    struct sockaddr_in *my_addr)					// bind address, used for producing only
  {
    struct udp_pcb *pcb;

    /* create a new UDP socket */
    pcb = udp_new();
    if (pcb ==0)
      {
	OPENER_TRACE_ERR("networkhandler: cannot create UDP socket\n");
	return EIP_INVALID_SOCKET;
      }
    OPENER_TRACE_INFO("networkhandler: created UDP socket %08x\n", pcb);

    /* check if it is sending or receiving */
    if (pa_direction == CONSUMING)
      { /* bind is only for consuming necessary */
	struct ip_addr addr;

	addr.addr = htonl(my_addr->sin_addr.s_addr);
	if (udp_bind(pcb, &addr, htons(my_addr->sin_port)) != ERR_OK)
	  {
	    OPENER_TRACE_ERR("networkhandler: error on bind udp\n");
	    return EIP_INVALID_SOCKET;
	  }
	  OPENER_TRACE_INFO("networkhandler: bind UDP socket %08x port %d\n", pcb, my_addr->sin_port);
      }

	
    // this look like a hack -- ask Alois what this is all about
    if ((pa_direction == CONSUMING) || (0 == my_addr->sin_addr.s_addr))
        {
	  /* we have a peer to peer producer or a consuming connection*/
	  my_addr->sin_addr.s_addr = g_pCurrentActiveTCPSocket->remote_ip.addr;
        }
      
    udp_recv(pcb, udp_registered_receive_callback, 0);  		// set the callback

    return (int)pcb;
  }


void
IApp_CloseUDPSocket(int pa_nSockFd)
{
  if (EIP_INVALID_SOCKET != pa_nSockFd)
    {
    OPENER_TRACE_INFO("networkhandler: shutdown UDP socket %x\n", pa_nSockFd);
    udp_remove((struct udp_pcb *)pa_nSockFd);
    }
}

void
IApp_CloseTCPSocket(int pa_nSockFd)
{
  if (EIP_INVALID_SOCKET != pa_nSockFd)
    {
    OPENER_TRACE_INFO("networkhandler: shutdown TCP socket %x\n", pa_nSockFd);
    tcp_close((struct tcp_pcb *)pa_nSockFd);
    }
}
