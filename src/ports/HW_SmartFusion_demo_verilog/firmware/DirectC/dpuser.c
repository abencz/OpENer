/* ************************************************************************ */
/*                                                                          */
/*  IAP 	       Copyright (C) Actel Corporation 2010                 */
/*  Version 1.0	       Release date February  17, 2010                      */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpuser.c	  	        			    */
/*                                                                          */
/*  Description:    user specific file containing JTAG interface functions  */
/*                  and delay function                                      */
/*                                                                          */
/****************************************************************************/

#include "main.h"
#include "a2fxxxm3.h"
#include "mss_uart.h"
#include "dpuser.h"
#ifdef ENABLE_IAP_SUPPORT
#include "iap_hw_interface.h"
#endif
#ifdef ENABLE_GPIO_SUPPORT
#include "dpjtag.h"
#endif
#include "dpalg.h"
#include "dpdef.h"
#include "dputil.h"

/* This variable is used to select external programming vs IAP programming */
DPUCHAR hardware_interface;

/*
* User attention: 
* Module: dp_delay
* 		purpose: Execute a time delay for a specified amount of time.
* Arguments: 
* 		microseconeds: 32 bit value containing the amount of wait time in microseconds.
* Return value: None
* 
*/
void dp_delay(DPULONG microseconds)
{
    volatile DPULONG i;
    volatile DPULONG j;
    for(i=0;i<microseconds;i++) {
            for (j=0;j<50;j++) ;
        }
}
#ifdef ENABLE_DEBUG
void dp_display_text(DPCHAR * text)
{
    
    unsigned int length;
    length = 0;
    while (text[length++]!='\0');
    
    MSS_UART_polled_tx (&g_mss_uart0, (unsigned char *)text,length);
    
    while(MSS_UART_tx_complete(&g_mss_uart0) == 0);
}

void dp_display_value( unsigned long value ,int Descriptive)
{
    
    uint8_t value_text[256];
    int text_size;
    if (Descriptive == HEX)
    {
        text_size = int_to_hex_int( value, value_text, 8);
    }
    else 
    {
        text_size = int_to_dec_int( value, value_text, 8);
    }
    
    MSS_UART_polled_tx (&g_mss_uart0, value_text, text_size);
    while(MSS_UART_tx_complete(&g_mss_uart0) == 0);
}

void dp_display_array(unsigned char * outbuf, unsigned int bytes, int Descriptive)
{
    int idx;
    for (idx=0;idx<bytes;idx++)
    {
        dp_display_value(outbuf[bytes-1-idx],Descriptive);
        dp_display_text(" ");
    }
}
#endif


