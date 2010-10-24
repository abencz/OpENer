/*******************************************************************************
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * This file is part of not-an-OS for the Cortex-M3.
 * BSD licensed, see README.txt
 *
 * Author: Jonathan Engdahl jrengdahl@ra.rockwell.com
 ******************************************************************************/

	.syntax unified
        .cpu cortex-m3
        .fpu softvfp
        .thumb
        .thumb_func
        .text
	.file	"threads.S"
        .align  2
    
        
/* This is the multi-threading not-an-OS.
 * Sometimes called the "plastic fork".
 * These should all be called with interrupts off, since the stack pointer is incorrect in the middle of a context switch.
 */

	.data
pending_chain: .word 0			/* linked list of pending threads */

/*
 * Wake up a thread which is waiting at a specified location.
 *   wake(arg, address of wait location)
 * The arg will be passed to the waiting thread and returned from the "wait" call.
 * Current thread is placed on the pending_chain.
 */

	.text
	.global wake			/* wake up thread waiting at x */
	.type   wake, %function

wake:	ldr r2,[r1]			/* get pointer to new thread */
	cbz r2, 1f			/* return if nothing waiting for a wake */

	ldr r12,=pending_chain		/* get address of head of linked list */
	ldr r3,[r12]			/* get first member of list */
	push {r3-r11,lr}		/* save current context, including old link, on stack */
	str sp,[r12]			/* save current context as head of chain */

	mov sp,r2			/* switch to new stack pointer */
	pop {r3-r11,lr}			/* load new context */
	str r3,[r1]			/* unlink chain of waiting threads at port */
1:	bx lr				/* execute new context */

	.size	wake, .-wake

/*
 * Wait for an event at a specified location.
 *   wait(address of wait location)
 * Returns the arg passed by wake.
 * This version allows multiple threads to wait for the same event
 * although not many threads, due to the cost of walking the linked list.
 */

	.global wait			/* wait at location x */
	.type   wait, %function

1:	mov r0,r3			/* else follow chain */
wait:	ldr r3,[r0]			/* get the pointer to the first waiter, if any */
	cmp r3,#0			/* see if there is anyone there */
	bne 1b				/* if link is null, save to this address */

	push {r3-r11,lr}		/* save current context on stack */
	str sp,[r0]			/* save stack pointer at wait location */

	ldr r12,=pending_chain		/* get address of head of linked list */	
	ldr sp,[r12]			/* get sp of last preempted thread from chain */
	pop {r3-r11,lr}			/* load context from chain */
	str r3,[r12]			/* unlink new context from chain */
	bx lr				/* execute new context */
 
	.size	wait, .-wait+2

/*
 * Spawn a new thread.
 *   spawn(arg,code_address,stack_address)
 * Current thread is placed on the pending_chain.
 */

	.global spawn			/* create new thread */
	.type   spawn, %function

spawn:	ldr r12,=pending_chain		/* this code is similar to beginning of "wake" */
	ldr r3,[r12]			/* get first member of list */
	push {r3-r11,lr}		/* save current context, including old link, on stack */
	str sp,[r12]			/* save current context as head of chain */

	mov sp,r2			/* init new stack pointer */
	bx r1				/* transfer control to new thread */

	.size	spawn, .-spawn

	.end
