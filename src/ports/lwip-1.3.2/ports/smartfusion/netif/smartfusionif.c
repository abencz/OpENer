/**
 * @file
 * lwIP Ethernet interface for Actel SmartFusion
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/**
 * Copyright (c) 2010, Rockwell Automation, Inc.
 *
 * This file is derived from the "ethernetif.c" skeleton Ethernet network
 * interface driver for lwIP.
 *
 */

#include <stdio.h>
#include <string.h>

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "lwip/init.h"
#include "lwipopts.h"
#include "netif/etharp.h"

#include "mss_ethernet_mac.h"

#include "netif/smartfusionif.h"
#include "opener_api.h"

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

/**
 * In this function, the hardware should be initialized.
 * Called from smartfusionif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{

  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
  extern unsigned char MacAddress[6];
  netif->hwaddr[0] = MacAddress[0];
  netif->hwaddr[1] = MacAddress[1];
  netif->hwaddr[2] = MacAddress[2];
  netif->hwaddr[3] = MacAddress[3];
  netif->hwaddr[4] = MacAddress[4];
  netif->hwaddr[5] = MacAddress[5];

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

  /* Do whatever else is needed to initialize interface. */
	MSS_MAC_init(1);  // designate phy port number


	uint32_t mac_cfg;

	mac_cfg = MSS_MAC_get_configuration();
	mac_cfg &= ~( MSS_MAC_CFG_RECEIVE_ALL | MSS_MAC_CFG_STORE_AND_FORWARD | MSS_MAC_CFG_PASS_BAD_FRAMES );
	mac_cfg |=
	    MSS_MAC_CFG_PROMISCUOUS_MODE |
	    MSS_MAC_CFG_FULL_DUPLEX_MODE |
	    MSS_MAC_CFG_TRANSMIT_THRESHOLD_MODE |
	    MSS_MAC_CFG_THRESHOLD_CONTROL_00;
	MSS_MAC_configure(mac_cfg );

	MSS_MAC_set_mac_address(netif->hwaddr);
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf might be
 * chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */


static unsigned char buffer[4096];

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
	struct pbuf *q;
	unsigned char *ptr=buffer;
 
	// copy the pbuf chain into a single contiguous scratch buffer
	for(q = p; q != 0; q = q->next)
		{
		/* Send the data from the pbuf to the interface, one pbuf at a
		time. The size of the data in each pbuf is kept in the ->len
		variable.  if q->next == NULL then this is the last pbuf in the
		chain. */

		memcpy(ptr,q->payload,q->len);
		ptr+=q->len;
		}

	if(!MSS_MAC_tx_packet(buffer, p->tot_len, MSS_MAC_BLOCKING))
		{
		printf("Failed Sending Data to Eth len =%d\n\r", p->tot_len);
		return( ~ERR_OK);
		}

	LINK_STATS_INC(link.xmit);

	return ERR_OK;
	}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif)
{
	struct pbuf *p, *q;
	unsigned len;
	unsigned l;

	p = 0;
	l = 0;

	/* Obtain the size of the packet and put it into the "len" variable. */
	len = MSS_MAC_rx_packet(buffer, 4096, MSS_MAC_NONBLOCKING); 

	if(len)
		{
		/* We allocate a pbuf chain of pbufs from the pool. */
		p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

		if (p != NULL)
			{
			/* We iterate over the pbuf chain until we have read the entire packet into the pbuf. */
			for(q = p; q != 0; q = q->next)
				{
				/* Read enough bytes to fill this pbuf in the chain. The
				 * available data in the pbuf is given by the q->len
				 * variable.
				 * This does not necessarily have to be a memcpy, you can also preallocate
				 * pbufs for a DMA-enabled MAC and after receiving truncate it to the
				 * actually received size. In this case, ensure the tot_len member of the
				 * pbuf is the sum of the chained pbuf len members.
				 */
				memcpy((u8_t*)q->payload, &buffer[l], q->len);
				l = l + q->len;
				}

			LINK_STATS_INC(link.recv);
			}
		else
			{
			LINK_STATS_INC(link.memerr);
			LINK_STATS_INC(link.drop);
			}
		}
	return p;
	}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void
smartfusionif_input(struct netif *netif)
{
  struct pbuf *p;

  /* move received packet into a new pbuf */
  p = low_level_input(netif);
  /* no packet could be read, silently ignore this */
  if (p == 0) return;
  if (ethernet_input(p, netif)!=ERR_OK)
	{
	LWIP_DEBUGF(NETIF_DEBUG, ("smartfusionif_input: IP input error\n"));
	pbuf_free(p);

	/* Adjust the link statistics */
	LINK_STATS_INC(link.memerr);
	LINK_STATS_INC(link.drop);
	}
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
smartfusionif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 1000000);

  netif->state = 0;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;


  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}


