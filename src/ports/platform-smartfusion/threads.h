/*******************************************************************************
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * This file is part of not-an-OS for the Cortex-M3.
 * BSD licensed, see README.txt
 *
 * Author: Jonathan Engdahl jrengdahl@ra.rockwell.com
 ******************************************************************************/

/*
 * defines for thread switching
 */

#ifndef THREADS_H
#define THREADS_H

// threading support

typedef void THREADFUNCTION(unsigned);
typedef unsigned STACK;
typedef STACK * volatile PORT;
typedef unsigned ARGT;

#ifdef __cplusplus
extern "C" {
#endif

void spawn(unsigned arg,THREADFUNCTION *code, STACK *stack);			// create a new thread
ARGT wait(PORT *port);								// wait for an event
void wake(ARGT arg, PORT *port);						// wake a thread which is waiting

#ifdef __cplusplus
	}
#endif

#define THREAD(name,arg) void name(arg)						// define a thread

// spawn a thread
// input:
//   -- name of thread
//   -- size of stack (in bytes)
//   -- startup arg passed to thread on innvocation

// fills the stack with the thread name
// high 16 bytes of stack are not used (for stack collision checking)

#define SPAWN(name,size,arg)							\
	do									\
		{								\
		char *stk;							\
		const char *uname = #name;					\
		int i;								\
		int j=0;							\
		stk = (char *)malloc(size);					\
		for(i=0;i<size;i++)						\
			{							\
			stk[i]=uname[j++];					\
			if(j>=strlen(#name))j=0;				\
			}							\
		spawn(arg,&name,(STACK *)&stk[size-16]);			\
		}								\
	while(0)


#endif
