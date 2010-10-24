/*******************************************************************************
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * This file is part of not-an-OS for the Cortex-M3.
 * BSD licensed, see README.txt
 *
 * Author: Jonathan Engdahl jrengdahl@ra.rockwell.com
 ******************************************************************************/

#ifndef MUTEX_H
#define MUTEX_H

/*
 * This is a limited mutex adequate to keep printf output from various threads from
 * getting intermingled on the console.
 *
 */

#include "threads.h"

typedef struct 
	{
	int flag;
	PORT mwait;
	}mutex;

static inline void mutex_lock(mutex *m)
	{
	while(m->flag)
		{
		wait(&m->mwait);
		}
	m->flag=1;
	}

static inline void mutex_unlock(mutex *m)
	{
	m->flag=0;
	if(m->mwait)
		{
		wake(0,&m->mwait);
		}
	}

#endif
