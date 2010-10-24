/*******************************************************************************
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * This file is part of not-an-OS for the Cortex-M3.
 * BSD licensed, see README.txt
 *
 * Author: Jonathan Engdahl jrengdahl@ra.rockwell.com
 ******************************************************************************/

/*
 * getline and associated routines for the debug monitor
 */

#include <stdio.h>
#include "local.h"
#include "mutex.h"
#include "parm.h"

extern char printbuf[MAXPRINTF];
char buf[INBUFLEN];

int getdec(char **p)
	{
	int x=0;

	while(1)
		{
		if('0'<=**p&&**p<='9')x = (x<<3) + (x<<1) + **p-'0';
		else return x;
		(*p)++;
		}
	}

int gethex(char **p)
	{
	int x=0;

	while(1)
		{
		if('0'<=**p&&**p<='9')     x = (x<<4) + **p-'0';
		else if('a'<=**p&&**p<='f')x = (x<<4) + **p-'a'+10;
		else if('A'<=**p&&**p<='F')x = (x<<4) + **p-'A'+10;
		else return x;
		(*p)++;
		}
	}

// read an IP address from the console
int getip(char **p)
	{
	int ip = 0;

	ip = getdec(p);
	if(*(*p)++ != '.')return -1;

	ip |= getdec(p)<<8;
	if(*(*p)++ != '.')return -1;

	ip |= getdec(p)<<16;
	if(*(*p)++ != '.')return -1;

	ip |= getdec(p)<<24;

	return ip;
	}

// read an MAC ID  from the console
int getMACID(char **p, MACID m)
	{
	int i;

	for(i=0;i<6;i++)
		{
		m[i] = gethex(p);
		if(i<5 && *(*p)++ != ':')return -1;
		}
	return 0;
	}


void skip(char **p)
	{
 	while(**p && **p!=' ')(*p)++;		// skip the arg that p points to
	while(**p && **p==' ')(*p)++;		// skip any following whitespace
	}


	
	

static void forward(int col)
	{
	printf("\033[%dC",col);
	}

static void back(int col)
	{
	if(col==0)return;
	printf("\033[%dD",col);
	}

static void blank(int n)
	{
	PRINTBUF_LOCK;
	for(int i=0;i<n;i++)putchar(' ');
	PRINTBUF_UNLOCK;
	}

#define HMASK (NHISTORY-1)
static int first=1;
static char lastline[NHISTORY][INBUFLEN];
static int lln=0;

void getline(	char *buf,						// input buffer to be filled
             	int size)						// size of buffer
	{
	int i;								// general purpose loop counter
	int c;								// current input character
	int x=0;							// cursor postion
	int n=0;							// length of line
	int tll=0;
	
	if(first)
		{
		for(i=0;i<NHISTORY;i++)lastline[i][0]=0;
		first=0;
		}
	
	while(n<size-1 && (c=getchar()) !='\n')				// repeat until buffer filled or end of line
		{
		if(c==0x1b)						// escape sequences
			{
			if((c=getchar())=='[')
				{
				c=getchar();
				if(c=='C')				// ->
					{
					if(x<n)
						{
						x++;
						forward(1);
						}
					}
				else if(c=='D')				// <-
					{
					if(x>0)
						{
						x--;
						back(1);
						}
					}
				else if(c=='A')				// up arrow
					{
					if(tll<NHISTORY)
						{
						back(x);
						blank(n);
						back(n);
						x=0;
						n=0;
						tll++;
						PRINTBUF_LOCK;
						for(i=0;lastline[(lln-tll)&HMASK][i];i++)
							{
							buf[i] = lastline[(lln-tll)&HMASK][i];
							putchar(buf[i]);
							x++;
							n++;
							}
						PRINTBUF_UNLOCK;
						buf[n]=0;
						}
					}
				else if(c=='B')				// down arrow
					{
					back(x);
					blank(n);
					back(n);
					x=0;
					n=0;
					if(tll)tll--;
					if(tll)
						{
						PRINTBUF_LOCK;
						for(i=0;lastline[(lln-tll)&HMASK][i];i++)
							{
							buf[i] = lastline[(lln-tll)&HMASK][i];
							putchar(buf[i]);
							x++;
							n++;
							}
						PRINTBUF_UNLOCK;
						buf[n]=0;
						}
					}
				else if(c=='4'&& (c=getchar())=='~')	// del
					{
					if(x<n)
						{
						PRINTBUF_LOCK;
						for(i=x;i<n;i++)
							{
							buf[i]=buf[i+1];
							putchar(buf[i]);
							}
						putchar(' ');
						PRINTBUF_UNLOCK;
						n--;
						back(n-x+1);
						}
					}
				else if(c=='2'&& (c=getchar())=='~')	// home
					{
					back(x);
					x = 0;
					}
				else if(c=='5'&& (c=getchar())=='~')	// end
					{
					forward(n-x);
					x = n;
					}
				else if(c=='3'&& (c=getchar())=='~')	// page up (erase line)
					{
					back(x);
					blank(n);
					back(n);
					x=0;
					n=0;
					}
				else					// unknown escape sequence
					{
					printf("$[%c",c);
					}
				}
			else if(c=='O'&& (c=getchar())=='P'){}
			else						// unknown escape sequence
				{	
				printf("$%c",c);
				}
			}
		else if(c==8)					// backspace
			{
			if(x>0)
				{
				x--;
				back(1);
				PRINTBUF_LOCK;
				for(i=x;i<n;i++)
					{
					buf[i]=buf[i+1];
					putchar(buf[i]);
					}
				putchar(' ');
				PRINTBUF_UNLOCK;
				n--;
				back(n-x+1);
				}
			}
		else
			{	
			for(i=n;i>x;i--)buf[i] = buf[i-1];
			buf[x]=c;
			n++;
			buf[n]=0;
			PRINTBUF_LOCK;
			for(i=x;i<n;i++)putchar(buf[i]);
			PRINTBUF_UNLOCK;
			x++;
			if(x<n)back(n-x);
			}
		}
	if(x<n)forward(n-x);
	PRINTBUF_LOCK;
	putchar('\n');
	PRINTBUF_UNLOCK;

	for(i=0;i<n;i++)lastline[lln][i] = buf[i];
	lastline[lln][n]=0;
	lln = (lln+1)&HMASK;
	}	


