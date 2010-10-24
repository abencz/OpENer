/*******************************************************************************
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * This file is part of not-an-OS for the Cortex-M3.
 * BSD licensed, see README.txt
 *
 * Author: Jonathan Engdahl jrengdahl@ra.rockwell.com
 ******************************************************************************/

/*
 * a simple command line interpreter
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "local.h"
#include "threads.h"
#include "mutex.h"
#include "parm.h"

#include "mss_gpio.h"
#include "mss_nvm.h"
#include "lwipinc.h"


extern char printbuf[MAXPRINTF];
extern char buf[INBUFLEN];
extern int getdec(char **p);
extern int gethex(char **p);
extern unsigned getip(char **p);
extern void skip(char **p);
extern mutex printlock;                         // a mutex to protect output sequences
extern struct netif MACB_if;
#define IpAddr MACB_if.ip_addr.addr
#define NetMask MACB_if.netmask.addr
#define Gateway MACB_if.gw.addr


static unsigned long ip, nm, gw;

static void printip(void)
	{
	printf("new ip   = %d.%d.%d.%d\n", ip&255, ip>>8&255, ip>>16&255, ip>>24&255); 		// print results so far
	printf("new mask = %d.%d.%d.%d\n", nm&255, nm>>8&255, nm>>16&255, nm>>24&255);
	printf("new gw   = %d.%d.%d.%d\n", gw&255, gw>>8&255, gw>>16&255, gw>>24&255);
	}

// print help string, see usage below
#define HELP(s) else if(buf[0]=='?' && puts(s) && 0){}

THREAD(interp, unsigned arg)
	{
	printf("hello, world!\n");

	ip=IpAddr;							// zero numbers if they are not valid
	nm=NetMask;
	gw=Gateway;

	while(1)
		{
		char *p;

		mutex_lock(&printlock);
		putchar('>');							// output the command line prompt
                mutex_unlock(&printlock);
		getline(buf,INBUFLEN);						// get a command line
		p = &buf[0];
		skip(&p);							// skip command and following whitespace

		if(buf[0]==0 || buf[0]=='\r' || buf[0]=='\n' || buf[0]==' '){}	// ignore blank lines

		HELP(  "addresses and data are in hex, sizes are in decimal in bytes\n")


		HELP(  "mac xx:xx:xx:xx:xx:xx			specify new MAC ID")
		else if(buf[0]=='m' && buf[1]=='a' && buf[2]=='c')
			{
			MACID m;

			extern int getMACID(char **p, MACID m);
			if(getMACID(&p, m)==0)
				{
				NVM_write((unsigned)&NVM_MACID, (unsigned char *)&m, sizeof(m));
				printf("saved, must reset to use new MAC ID\n");
				}
			else printf("invalid MAC ID\n");
			}

		HELP(  "i ddd.ddd.ddd.ddd			specify new ip address")
		else if(buf[0]=='i')
			{
			ip = getip(&p);

			printip();
			}

		HELP(  "n ddd.ddd.ddd.ddd			specify new netmask")
		else if(buf[0]=='n')
			{
			nm = getip(&p);

			printip();
			}

		HELP(  "g ddd.ddd.ddd.ddd			specify new gateway")
		else if(buf[0]=='g')
			{
			gw = getip(&p);

			printip();
			}


		HELP(  "st					save and use static ip address")
		else if(buf[0]=='s' && buf[1]=='t')
			{
			struct parm parm;

			parm.useStatic = 1;
			parm.ip = ip;
			parm.nm = nm;
			parm.gw = gw;
			NVM_write((unsigned)&NVM_PARM, (unsigned char *)&parm, sizeof(parm));
			extern void useStaticIP(unsigned, unsigned, unsigned);
			useStaticIP(ip, nm, gw);
			printf("\nStatic IP address saved.\n");
			}

		HELP(  "dh					discard static ip address and use DHCP")
		else if(buf[0]=='d' && buf[1]=='h')
			{
			struct parm parm;

			parm.useStatic = 0;
			parm.ip = 0;
			parm.nm = 0;
			parm.gw = 0;
			NVM_write((unsigned)&NVM_PARM, (unsigned char *)&parm, sizeof(parm));
			extern void useDHCP(void);
			useDHCP();
			printf("\nUsing DHCP, this change has been saved.\n");
			}

		HELP(  "d <addr> <size>}			dump memory")
		else if(buf[0]=='d')						// memory dump d <hex address> <dec size> {<dump width>}
			{
			unsigned *a;
			int s;

			a = (unsigned *)gethex(&p);				// get the first arg
			skip(&p);
			if(isdigit(*p))s = getdec(&p);				// the second arg is the size
			else s = 4;

			dump(a,s);						// do the dump
			}

		HELP(  "m <addr> <data32>...			modify memory")
		else if(buf[0]=='m')						// memory modify <address> <data> ...
			{
			unsigned *a;
			unsigned v;

			a = (unsigned *)gethex(&p);				// get the first arg
			skip(&p);
			while(isxdigit(*p))					// while there is any data left on the command line
				{
				v = gethex(&p);					// get the data
				skip(&p);					// skip that arg and folowing whitespace
				*a++ = v;					// store the data
				}
			}

		HELP(  "f <addr> <size> <data32>		fill memory")
		else if(buf[0]=='f')						// memory fill <address> <size> <data32>
			{
			unsigned *a;
			int s;
			unsigned v;

			a = (unsigned *)gethex(&p);				// get the first arg
			skip(&p);
			s = getdec(&p);						// the second arg is the size
			skip(&p);
			v = gethex(&p);						// value is the third arg

			while(s>0)						// fill memory with the value
				{
				*a++=v;
				s-=4;
				}
			}


		HELP(  "g <bit> <value>				set GPIO bit using bitbanding")
		else if(buf[0]=='g')
			{
			unsigned b;
			unsigned v;

			b = getdec(&p);						// get the first arg
			skip(&p);
			v = getdec(&p);						// the second arg is the value

			GPIO_BITBAND->GPIO_OUT[b] = v;
			}


		else if(buf[0] == '?')
			{
			ip=NVM_PARM.ip;						// zero numbers if they are not valid
			nm=NVM_PARM.nm;
			gw=NVM_PARM.gw;

			printf("\n");
			printip();
			}

		// else I dunno what you want
		else printf("illegal command\n");
		}
	}

