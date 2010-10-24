/*******************************************************************************
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * This file is part of not-an-OS for the Cortex-M3.
 * BSD licensed, see README.txt
 *
 * Author: Jonathan Engdahl jrengdahl@ra.rockwell.com
 ******************************************************************************/


#ifndef LOCAL_H
#define LOCAL_H

// this is a catch-all for platform specific definitions

#define isxdigit(c) ('0'<=(c)&&(c)<='9' || 'a'<=(c)&&(c)<='f' || 'A'<=(c)&&(c)<='F')
#define isdigit(c) ('0'<=(c)&&(c)<='9')

#define MAXPRINTF 128
#define INBUFLEN 64
#define NHISTORY 4	// must be a power of two

#ifdef getchar
#undef getchar
#endif
#ifdef putchar
#undef putchar
#endif

#ifdef __cplusplus
// placement new support
inline void * operator new(unsigned x, void *location)
	{
	return location;
	}
#endif

#ifdef __cplusplus
extern "C" {
#endif

void dump(void *p, int size);
void getline(char *buf,int size);
int printk(const char *fmt, ...);

#ifdef __cplusplus
	}
#endif

#include "mutex.h"
extern mutex printlock;
#define PRINTBUF_LOCK mutex_lock(&printlock)
#define PRINTBUF_UNLOCK mutex_unlock(&printlock)


#endif
