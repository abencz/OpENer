/*
 * main for opENer port to the Actel SmartFusion eval board
 */

/*******************************************************************************
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * This file is part of not-an-OS for the Cortex-M3.
 * BSD licensed, see README.txt
 *
 ******************************************************************************/

// Some of the code in this module was morphed from Actel's lwIP webserver demo, therefore:

/*******************************************************************************
 * (c) Copyright 2009 Actel Corporation.  All rights reserved.
 *
 *******************************************************************************/

// standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// includes for Actel's SmartFusion chip
#include "a2fxxxm3.h"
#include "mss_watchdog/mss_watchdog.h"
#include "mss_uart/mss_uart.h"
#include "mss_gpio/mss_gpio.h"
#include "mss_nvm/mss_nvm.h"
#include "mss_ethernet_mac/mss_ethernet_mac.h"

// includes for Actel's SmartFusion eval board
#include "oled_driver/oled.h"

// includes for lwIP
#include "lwipinc.h"

// includes for this port
#include "local.h"
#include "serial.h"
#include "threads.h"
#include "mutex.h"

// includes for opENer
#include "opener_user_conf.h"
#include "opener_api.h"
#include "networkhandler.h"
#include "parm.h"




unsigned char MacAddress[6];					// RAM copy of the MAC ID
struct netif MACB_if;						// low level Ethernet interface struct required by lwIP
char hostname[16];						// ram copy of the hostname
struct ip_addr xIpAddr, xNetMask, xGateway;			// ram copy of ip address, etc.
u32_t lastIpAddr=-1, lastNetMask=-1, lastGateway=-1;		// last known ip address, etc.

#define IpAddr MACB_if.ip_addr.addr				// short names for ip address, etc.
#define NetMask MACB_if.netmask.addr
#define Gateway MACB_if.gw.addr


extern void ethernetif_input( void * pvParameters );


// defines lifted from Actel's lwIP demo

#define configCPU_CLOCK_HZ			( ( unsigned long ) SystemFrequency )
#define configTICK_RATE_HZ			( 1000 )

#define portNVIC_SYSTICK_CTRL		( ( volatile unsigned long *) 0xe000e010 )
#define portNVIC_SYSTICK_LOAD		( ( volatile unsigned long *) 0xe000e014 )

#define SYS_TICK_CTRL_AND_STATUS_REG      0xE000E010
#define SYS_TICK_CONFIG_REG               0xE0042038
#define SYS_TICK_FCLK_DIV_32_NO_REF_CLK   0x31000000
#define ENABLE_SYS_TICK                   0x7

#define portNVIC_SYSTICK_CLK		0x00000004
#define portNVIC_SYSTICK_INT		0x00000002
#define portNVIC_SYSTICK_ENABLE		0x00000001




// TIMER TICK STUFF

volatile unsigned milliseconds = 0;				// running count of time since powerup


void
SysTick_Handler(void)                                           // timer tick ISR
{
  milliseconds += 1000 / configTICK_RATE_HZ;
}

// Simple periodic polling function
#define PERIODIC(var,time,function) \
    if((milliseconds - var) > time) \
      {                             \
        var += time;                \
	trigon(0);		    \
        function;                   \
	trigoff(0);		    \
      }                             \
    


// init the chip and board hardware

void init_system()
  {
    /* Disable the Watch Dog Timer */
    MSS_WD_disable( );

    /* Initialize the OLED */
    OLED_init();

    /*Initialize and Configure GPIO*/
    MSS_GPIO_init();
    MSS_GPIO_config( MSS_GPIO_0 , MSS_GPIO_OUTPUT_MODE );
    MSS_GPIO_config( MSS_GPIO_1 , MSS_GPIO_OUTPUT_MODE );
    MSS_GPIO_config( MSS_GPIO_2 , MSS_GPIO_OUTPUT_MODE );
    MSS_GPIO_config( MSS_GPIO_3 , MSS_GPIO_OUTPUT_MODE );
    MSS_GPIO_config( MSS_GPIO_4 , MSS_GPIO_OUTPUT_MODE );
    MSS_GPIO_config( MSS_GPIO_5 , MSS_GPIO_OUTPUT_MODE );
    MSS_GPIO_config( MSS_GPIO_6 , MSS_GPIO_OUTPUT_MODE );
    MSS_GPIO_config( MSS_GPIO_7 , MSS_GPIO_OUTPUT_MODE );
    MSS_GPIO_config( MSS_GPIO_11 , MSS_GPIO_OUTPUT_MODE );
    MSS_GPIO_config( MSS_GPIO_12 , MSS_GPIO_OUTPUT_MODE );
    MSS_GPIO_config( MSS_GPIO_13 , MSS_GPIO_OUTPUT_MODE );
    MSS_GPIO_config( MSS_GPIO_14 , MSS_GPIO_OUTPUT_MODE );
    MSS_GPIO_config( MSS_GPIO_15 , MSS_GPIO_OUTPUT_MODE );


    *(volatile unsigned long *)SYS_TICK_CTRL_AND_STATUS_REG = ENABLE_SYS_TICK;
    *(volatile unsigned long *)SYS_TICK_CONFIG_REG          = SYS_TICK_FCLK_DIV_32_NO_REF_CLK;

    *(portNVIC_SYSTICK_LOAD) = ( configCPU_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
    *(portNVIC_SYSTICK_CTRL) = portNVIC_SYSTICK_CLK | portNVIC_SYSTICK_INT | portNVIC_SYSTICK_ENABLE;
  }


struct oled_data write_data;
char line1[20];
char line2[20];

/**************************************************************************/
/* Function to display the welcome text on OLED */
/**************************************************************************/

void oled_welcome_display(void)
    {
	uint8_t j;

	write_data.line1          = FIRST_LINE;
	write_data.char_offset1   = 0;
	write_data.string1        = "      opENer";

	write_data.line2          = SECOND_LINE;
	write_data.char_offset2   = 0;
	write_data.string2        = "";

	write_data.contrast_val                 = OLED_CONTRAST_VAL;
	write_data.on_off                       = OLED_HORIZ_SCROLL_OFF;
	write_data.column_scrool_per_step       = OLED_HORIZ_SCROLL_STEP;
	write_data.start_page                   = OLED_START_PAGE;
	write_data.time_intrval_btw_scroll_step = OLED_HORIZ_SCROLL_TINVL;
	write_data.end_page                     = OLED_END_PAGE;

	OLED_write_data(&write_data,BOTH_LINES);
    }

// report the ip address, netmask, and gateway when they have changed

void reportIP(void)
    {
	printf("ip address = %d.%d.%d.%d\n",
		IpAddr&255,
		IpAddr>>8&255,
		IpAddr>>16&255,
		IpAddr>>24&255);

	printf("netmask    = %d.%d.%d.%d\n",
		NetMask&255,
		NetMask>>8&255,
		NetMask>>16&255,
		NetMask>>24&255);

	printf("gateway    = %d.%d.%d.%d\n",
		Gateway&255,
		Gateway>>8&255,
		Gateway>>16&255,
		Gateway>>24&255);

	sprintf(line2, "ip=%d.%d.%d.%d\n",
		IpAddr&255,
		IpAddr>>8&255,
		IpAddr>>16&255,
		IpAddr>>24&255);

	write_data.string2        = line2;
	OLED_write_data(&write_data,SECOND_LINE);
    }


// write the CIP I/O value to the OLED display

void write_val(unsigned val)
{
  static unsigned last = 0;

  if(val != last)
    {
      sprintf(line1, "   opENer  %d", val);
      write_data.string1 = line1;
      OLED_write_data(&write_data, BOTH_LINES);
      last = val;
    }
}


// setup to use DHCP IP addressing

void useDHCP(void)
{
  xIpAddr.addr = 0;
  xNetMask.addr = 0;
  xGateway.addr = 0;

  netif_set_addr(&MACB_if, &xIpAddr, &xNetMask, &xGateway);

  dhcp_start(&MACB_if);
}


// setup to use static IP addressing

void useStaticIP(unsigned ip, unsigned nm, unsigned gw)
{
  xIpAddr.addr = ip;
  xNetMask.addr = nm;
  xGateway.addr = gw;

  dhcp_stop(&MACB_if);

  netif_set_addr(&MACB_if, &xIpAddr, &xNetMask, &xGateway);
}



// a simple random number generator of reasonable quality

unsigned random(unsigned seed)
{
  return seed * 1664525 + 1013904223;
}

PORT waitEnetPoll = 0;                                  // this is where the Ethernet thread waits for a wakeup call from background
EIP_UINT16 nUniqueConnectionID;


// this thread sets up and polls the lwIP TCP/IP stack
// and the CIP stack
// some of the background polling is done from this thread
// because subroutines called from the true background loop CANNNOT call printf
// (the background loop cannot call "wait")

THREAD(enetThread, unsigned arg)
{

  //////////////////////////////////////////////////////////
  // YOU MUST SET THESE TO VALID VALUES FOR YOUR LOCATION //
  //////////////////////////////////////////////////////////

  for(int i = 0; i < 6; i++)
    MacAddress[i] = NVM_MACID[i];                                       // get MAC ID from flash
  printf("MAC ID = %02x:%02x:%02x:%02x:%02x:%02x\n", MacAddress[0], MacAddress[1], MacAddress[2], MacAddress[3], MacAddress[4], MacAddress[5]);
  if(MacAddress[0] == 0xff                                              // check for valid MAC ID
      || (MacAddress[0] | MacAddress[1] | MacAddress[2] | MacAddress[3] | MacAddress[4] | MacAddress[5]) == 0)
    {
      PORT forever = 0;

      printf("you must set a valid MAC ID\n");
      printf("Ethernet will not start\n");
      wait(&forever);
    }
  configureMACAddress(MacAddress);                                      // set MAC ID in CIP
  setDeviceSerialNumber(MacAddress[3] << 16 | MacAddress[4] << 8 | MacAddress[5]); // use MAC ID to fabricate a serial number TODO is there a better way?
  configureDomainName("na.home.ra-int.com");                            // TODO figure out a way to make this variable
  sprintf(hostname, "opENer-%02X%02X%02X", MacAddress[3], MacAddress[4], MacAddress[5]); // hostname
  printf("hostname = %s\n", hostname);
  configureHostName(hostname);

  struct parm parm;                                                     // get IP parameters from flash
  parm = NVM_PARM;

  if(parm.useStatic == 1)
    {
      xIpAddr.addr = parm.ip;
      xNetMask.addr = parm.nm;
      xGateway.addr = parm.gw;
      printf("using static IP address\n");
    }
  else
    {
      xIpAddr.addr = 0;
      xNetMask.addr = 0;
      xGateway.addr = 0;
      printf("using DHCP\n");
    }

  lwip_init(); // init the lwIP stack

  netif_add(&MACB_if, &xIpAddr, &xNetMask, &xGateway, 0, smartfusionif_init, ip_input); /* add data to netif */
  netif_set_default(&MACB_if);                                          /* make it the default interface */
  netif_set_up(&MACB_if);                                               /* bring it up */

  configureNetworkInterface(IpAddr, NetMask, Gateway);                  // init CIP ip parms

  if(parm.useStatic != 1)                                               // start DHCP if needed
    {
      dhcp_start(&MACB_if);
    }

  /* nUniqueConnectionID should be sufficiently random or incremented and stored
   * in non-volatile memory each time the device boots.
   */

  unsigned seed;
  seed = random(NVM_SEED);                                              // get and advance a random seed from flash
  NVM_write((unsigned)&NVM_SEED, (unsigned char *)&seed, 4);            // write it back for next powerup
  printf("random seed = %08x\n", seed);

  CIP_Init(seed);                                                       // init the CIP stack


  Start_NetworkHandler();                                               // init the gasket between lwIP and CIP


  unsigned arp_timer = 0;
  unsigned tcp_timer = 0;
  unsigned cip_timer = 0;
  unsigned dhcp_fine_timer = 0;
  unsigned dhcp_coarse_timer = 0;

  while(1)                                                              // this is the lwIP and CIP polling loop
    {
      wait(&waitEnetPoll);

      // Periodically call CIP timer.
      extern void
      CIPtick(void);
      PERIODIC(cip_timer, OPENER_TIMER_TICK, CIPtick());

      // There is also another method, calling tcp_fasttmr and tcp_slowtmr
      // directly at the appropriate intervals.
      PERIODIC(tcp_timer, TCP_TMR_INTERVAL, tcp_tmr());

      // Periodically call ARP timer to expire stale entries. Normally every 5 seconds.
      PERIODIC(arp_timer, ARP_TMR_INTERVAL, etharp_tmr());

      // Call DHCP fine and coarse timer functions, normally every 500ms and 60s
      // respectively.
      PERIODIC(dhcp_fine_timer, DHCP_FINE_TIMER_MSECS, dhcp_fine_tmr());
      PERIODIC(dhcp_coarse_timer, (DHCP_COARSE_TIMER_SECS*1000u), dhcp_coarse_tmr());

      while(MSS_MAC_rx_pckt_size())                                     // as long as there are Ethernet frames
	{
	  trigon(0);
	  smartfusionif_input(&MACB_if);                                // process them
	  trigoff(0);
	}

      if(IpAddr != lastIpAddr                                           // if the IP address has changed
	  || NetMask != lastNetMask || Gateway != lastGateway)
	{
	  reportIP();                                                   // report it

	  trigon(0);
	  configureNetworkInterface(IpAddr, NetMask, Gateway);          // and tell CIP about it
	  trigoff(0);

	  lastIpAddr = IpAddr;
	  lastNetMask = NetMask;
	  lastGateway = Gateway;
	}

    }
}


void uart_poll(void);

// background thread
// Setup the board, then become the background polling loop.
// This thread and anything it calls cannot execute a wait.
// This includes calling printf.

THREAD(background, unsigned arg)
  {
  /* Initialization all necessary hardware components */
  init_system();
  uart_init();
  oled_welcome_display();

  extern THREAD(interp, unsigned arg);
  SPAWN(interp, 1024, 0);				// spawn the serial console command line interpreter
  SPAWN(enetThread, 8192, 0);				// spawn the Ethernet thread

  while(1)                                              // this background loop polls the UART and the Ethernet thread
    {
      uart_poll();					// poll the UART

      wake(0, &waitEnetPoll);                           // poll the Ethernet thread
    }
}



// main
// Come here from CMSIS boot routine.
// All this does is init HEAP, then start the background thread
// and report the fault if the background thread ever tries to "wait"

void main(void)
{
  // fill heap with ascii "HEAP"
  register unsigned d;                                  // dest pointer, in a register
  register unsigned sp asm("r13");                      // this is the actual stack pointer, r13
  extern unsigned _break,_end;

  for(d=(_break==0?(unsigned)&_end:_break); d < sp; d+=4)
      {
	*(unsigned *)d = 0x50414548;                    // init heap space to "HEAP"
      }

  SPAWN(background, 800, 0);				// create the init and polling loop thread

  printk("\npanic: background attempted to wait\n");	// print a message
  while(1);						// and hang if background thread attempts to wait
}
