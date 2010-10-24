/*******************************************************************************
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * This file is part of not-an-OS for the Cortex-M3.
 * BSD licensed, see README.txt
 *
 * Author: Jonathan Engdahl jrengdahl@ra.rockwell.com
 ******************************************************************************/

/* rudimentary standard stdio:
 *   putchar
 *   getchar
 *   puts
 *   printf
 *
 *   also includes printk, which is used for early powerup and assert messages
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "threads.h"
#include "serial.h"
#include "mutex.h"
#include "local.h"


char printbuf[MAXPRINTF];			// a single printf buffer shared by all threads (!)

mutex printlock;				// a mutex to protect the single printf buffer,
						// and to keep prints from interleaving on the screen

static PORT rxready;				// sync point for getchar, see threads.h
static PORT txready;				// sync point for putchar


#undef getchar
int getchar(void)				// read a single char from serial console
        {
	int tmp;
        if(!uart_kbhit())
                {
                wait(&rxready);
                }
        tmp = uart_inchar();
	if(tmp=='\r')return '\n';
	else return tmp;
        }

#undef putchar
int putchar(int c)				// put a single char to the serial console
        {
        if(!uart_txready())wait(&txready);
	if(c=='\n')
		{
		uart_outchar('\r');
        	if(!uart_txready())wait(&txready);
		}
	uart_outchar(c);

	return c;
        }


void uart_poll(void)
	{
	if(uart_kbhit())
		{
		wake(0, &rxready);
		}

	//uart_txready is usually true when console is idle, so check txready PORT first

	if(txready && uart_txready())		// if a thread is waiting at txready and the uart is ready to transmit
		{
		wake(0,&txready);		// wait the transmit thread (passing a null arg)
		}
	}



int puts(const char *s)				// put a string to the console
	{
	PRINTBUF_LOCK;
	while(*s)putchar(*s++);
	putchar('\n');
	PRINTBUF_UNLOCK;

	return 1;
	}



int printf(const char *fmt, ...)		// print a formatted string to the serial console
	{
	va_list args;
	int ret;
	char *p;

	va_start(args, fmt);

	PRINTBUF_LOCK;
	ret = vsnprintf(printbuf,MAXPRINTF,fmt,args);

	p = printbuf;
	while(*p)putchar(*p++);
	PRINTBUF_UNLOCK;

	return ret;
	}


						// like printf, except spins on transmitter ready
int printk(const char *fmt, ...)		// only used for powerup and termination (assert) messages
	{					// when thread switching and background polling may not be working
	va_list args;
	int ret;
	char *p;

	va_start(args, fmt);

	ret = vsnprintf(printbuf, MAXPRINTF, fmt, args);

	p = printbuf;
	while (*p)
		{
		while(!uart_txready());
		if (*p=='\n')
			{
			uart_outchar('\r');
			while(!uart_txready());
			}
		uart_outchar(*p++);
		}

	return ret;
	}
