/*******************************************************************************
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * This file is part of not-an-OS for the Cortex-M3.
 * BSD licensed, see README.txt
 *
 * Author: Jonathan Engdahl jrengdahl@ra.rockwell.com
 ******************************************************************************/

/*
 * stubs for things that are normally provided the an OS or libc
 */


#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

typedef void INITFUNC(void);

extern INITFUNC *__init_array_start[];
extern INITFUNC *__init_array_end[];

void __libc_init_array(void)
	{
	for(int i = 0;
	    &__init_array_start[i] < __init_array_end;
	    i++)
		{
		__init_array_start[i] ();
		}
	}


void printk(const char *fmt, ...);

void __assert_func (
		const char	*__file,
		int		__line,
		const char	*__function,
		const char	*__assertion)
	{
        printk("assert(%s) failed in function \"%s\", file %s, at line %d\n",__assertion,__function,__file,__line);
	while(1);
	}
