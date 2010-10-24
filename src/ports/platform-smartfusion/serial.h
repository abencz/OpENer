/*******************************************************************************
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * This file is part of not-an-OS for the Cortex-M3.
 * BSD licensed, see README.txt
 *
 * Author: Jonathan Engdahl jrengdahl@ra.rockwell.com
 ******************************************************************************/

#ifndef SERIAL_H
#define SERIAL_H

// gasket for SmartFusion uart

#include "mss_uart.h"

static inline int uart_txready(void)
	{
	return g_mss_uart0.hw_reg_bit->LSR_THRE;
	}

static inline void uart_outchar(int c)
	{
	g_mss_uart0.hw_reg->THR = c;
	}

static inline int uart_kbhit(void)
	{
	return g_mss_uart0.hw_reg_bit->LSR_DR;
	}

static inline int uart_inchar(void)
	{
	return g_mss_uart0.hw_reg->RBR;
	}

static inline void uart_init(void)
	{
	MSS_UART_init(	&g_mss_uart0,
        		MSS_UART_115200_BAUD,
        		MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
	}


#endif
