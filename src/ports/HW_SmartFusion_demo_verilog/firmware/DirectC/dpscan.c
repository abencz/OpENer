/* ************************************************************************ */
/*                                                                          */
/*  IAP 	       Copyright (C) Actel Corporation 2010                 */
/*  Version 1.0	       Release date February  17, 2010                      */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpscan.h                                                */
/*                                                                          */
/*  Description:    Hardware Scan functions specific file                   */
/*                                                                          */
/* ************************************************************************ */

#include "dpscan.h"
#include "dpuser.h"
#include "dpjtag.h"
#include "iap_hw_interface.h"
#include "mss_iap.h"

void IRSCAN_in(void)
{
    
    #ifdef ENABLE_GPIO_SUPPORT
    if (hardware_interface == GPIO_SEL)
    {
        JTAG_IRSCAN_in();  
    }
    #endif
    #ifdef ENABLE_IAP_SUPPORT
    if (hardware_interface == IAP_SEL)
    {
        IAP_IRSCAN_in();
    }
    #endif
    return;
}

void IRSCAN_out(unsigned char* outbuf)
{
    #ifdef ENABLE_GPIO_SUPPORT
    if (hardware_interface == GPIO_SEL)
    {
        JTAG_IRSCAN_out(outbuf);
    }
    #endif
    #ifdef ENABLE_IAP_SUPPORT
    if (hardware_interface == IAP_SEL)
    {
        IAP_IRSCAN_out(outbuf);        
    }
    #endif
    return;
}

void DRSCAN_in(unsigned long start_bit_index, unsigned int bits_to_shift,unsigned char* inbuf)
{
    #ifdef ENABLE_GPIO_SUPPORT
    if (hardware_interface == GPIO_SEL)
    {
        JTAG_DRSCAN_in(start_bit_index, bits_to_shift, inbuf);
    }
    #endif
    #ifdef ENABLE_IAP_SUPPORT
    if (hardware_interface == IAP_SEL)
    {
        IAP_DRSCAN_in(start_bit_index, bits_to_shift, inbuf);      
    }
    #endif
    return;
}

void DRSCAN_out(unsigned int bits_to_shift,unsigned char* inbuf,unsigned char* outbuf)
{
    #ifdef ENABLE_GPIO_SUPPORT
    if (hardware_interface == GPIO_SEL)
    {
        JTAG_DRSCAN_out(bits_to_shift, inbuf, outbuf);
    }
    #endif
    #ifdef ENABLE_IAP_SUPPORT
    if (hardware_interface == IAP_SEL)
    {
        IAP_DRSCAN_out(bits_to_shift, inbuf, outbuf);      
    }
    #endif
    return;
}


void goto_jtag_state( unsigned char jtag_state, unsigned int cycles )
{
    #ifdef ENABLE_GPIO_SUPPORT
    if (hardware_interface == GPIO_SEL)
    {
        JTAG_goto_jtag_state(jtag_state, cycles );  
    }
    #endif
    #ifdef ENABLE_IAP_SUPPORT
    if (hardware_interface == IAP_SEL)
    {
        IAP_goto_jtag_state(jtag_state, cycles );
    }
    #endif
    
    return;
}
