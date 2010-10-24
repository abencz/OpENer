/*******************************************************************************
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * This file is part of not-an-OS for the Cortex-M3.
 * BSD licensed, see README.txt
 *
 * Author: Jonathan Engdahl jrengdahl@ra.rockwell.com
 ******************************************************************************/

/*
 * simple memory dump
 */

#include <stdio.h>
#include "local.h"

#define BUFLEN 80
static char buf[BUFLEN];

void dump(void *pp, int size)
	{
	int i;
	int c;
	unsigned char *p = (unsigned char *)pp;
	char *b;

	while(size>0)
		{
		b = buf;
		b += sprintf(b,"%08x: ",(int)p);
		for(i=0;i<16;i+=4)
			{
			if(i<size)sprintf(b,"%08x ",*(int *)&p[i]);
			else sprintf(b,"         ");
			b += 9;
			}
		*b++ = ' ';
		*b++ = '|';
		for(i=0;i<16;i++)
			{
			if(i<size)
				{
				c=p[i];
				if(' '<=c&&c<0x7f)*b++ = c;
				else *b++ = '.';
				}
			}
		*b++ = '|';
		*b++ = 0;
		p += 16;
		size -= 16;
		puts(buf);
		}
	puts("\n");
	}
