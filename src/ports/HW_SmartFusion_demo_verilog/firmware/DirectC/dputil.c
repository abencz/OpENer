/* ************************************************************************ */
/*                                                                          */
/*  IAP 	       Copyright (C) Actel Corporation 2010                 */
/*  Version 1.0	       Release date February  17, 2010                      */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dputil.c                                                */
/*                                                                          */
/*  Description:    Contains initialization and data checking functions.    */
/*                                                                          */
/****************************************************************************/

#include "dpuser.h"
#include "dpdef.h"
#include "dputil.h"
#include "dpalg.h"
#include "dpcom.h"

#define NB_NIBBLES_IN_INT		8

int int_to_hex_int(int value, uint8_t * p_result, int result_size)
{
    int nibble_idx, nb_nibbles;
    uint8_t conv_array[NB_NIBBLES_IN_INT];
    unsigned int uvalue;
    nibble_idx = 0;
    uvalue = (unsigned int)value;
    
    do {	
        int nibble = uvalue & 0x0F;
        
        if ( nibble < 10 )
            conv_array[nibble_idx] = nibble + '0';
        else
        conv_array[nibble_idx] = nibble  - 10 + 'A';
        uvalue = (uvalue >> 4);
        nibble_idx++;
    } while ( ( nibble_idx < NB_NIBBLES_IN_INT ) && ( uvalue > 0 ) );
    
    nb_nibbles = nibble_idx;
    for ( nibble_idx = 0; (nibble_idx < nb_nibbles) && (nibble_idx < result_size) ;nibble_idx++ )
    {
        p_result[nibble_idx] = conv_array[nb_nibbles - nibble_idx - 1];
    }
    return nibble_idx;
}

int int_to_dec_int(int value, uint8_t * p_result, int result_size)
{
    
    uint8_t conv_array[NB_NIBBLES_IN_INT];
    unsigned int uvalue;
    unsigned int remainder;
    unsigned int digit_idx,nb_digits;
    
    uvalue = (unsigned int)value;
    digit_idx=0;
    if (uvalue)
    {
        while (uvalue)
        {
            remainder = uvalue % 10;
            conv_array[digit_idx] = remainder + '0';
            uvalue /= 10;
            digit_idx++;
        }
    }
    else 
    {
        conv_array[digit_idx] = '0';
        digit_idx++;
    }
    
    
    nb_digits = digit_idx;
    for ( digit_idx = 0; (digit_idx < nb_digits); digit_idx++ )
    {
        p_result[digit_idx] = conv_array[nb_digits - digit_idx - 1];
    }
    return digit_idx;
}

void dp_flush_global_buf1(void)
{
    for (global_uchar=0u; global_uchar<global_buf_SIZE; global_uchar++)
    {
        global_buf1[global_uchar] = 0u;
    }
    return;
}

void dp_flush_global_buf2(void)
{
    for (global_uchar=0u; global_uchar<global_buf_SIZE; global_uchar++)
    {
        global_buf2[global_uchar]=0u;
    }
    return;
}

void dp_init_vars(void)
{
    device_security_flags = 0u;
    error_code = DPE_SUCCESS;
    return;
}
void dp_get_support_status(void)
{
    
    global_uint=1;
    dat_support_status = 0;
    for (global_ulong = 0;global_ulong < 16 ;global_ulong ++)
    {
        global_uchar=(DPUCHAR) dp_get_bytes(Header_ID,DAT_SUPPORT_STATUS_OFFSET+global_ulong,1);
        if (global_uchar)
            dat_support_status |= global_uint;
        global_uint <<= 1;
    }
    return;
}
void dp_get_dat_support_status(void)
{
    
    global_uint = 1u;
    dat_support_status = 0u;
    for (global_ulong = 0u;global_ulong < 16u ;global_ulong ++)
    {
        global_uchar=(DPUCHAR) dp_get_bytes( Header_ID,
        DAT_SUPPORT_STATUS_OFFSET + global_ulong,1u);
        if (global_uchar)
        {
            dat_support_status |= global_uint;
        }
        global_uint <<= 1u;
    }
    
    return;
}

void dp_check_dat_support_version(void)
{
    global_uchar = (DPUCHAR) dp_get_bytes(Header_ID, HEADER_SIZE_OFFSET, 1u);
    
    /* The header size of files generated from designer V8.6 is 69 vs. 56 from datgen and version 8.5 */
    if (global_uchar == 69u)
    {
        #ifdef ENABLE_V85_DAT_SUPPORT
        #ifdef ENABLE_DEBUG
        dp_display_text("\r\nError: data file loaded is generated from Designer verion V8.6 or later.  Disable ENABLE_V85_DAT_SUPPORT compile switch in dpuser.h and try again.");
        #endif
        error_code = DPE_DAT_VERSION_MISMATCH;
        #endif
    }
    else 
    {
        #ifndef ENABLE_V85_DAT_SUPPORT
        #ifdef ENABLE_DEBUG
        dp_display_text("\r\nError: data file loaded is generated from Designer verion V8.5 or stand alone datgen utility. Enable ENABLE_V85_DAT_SUPPORT compile switch in dpuser.h and try again.");
        #endif
        error_code = DPE_DAT_VERSION_MISMATCH;
        #endif
    }
    
}
/*
* Module: dp_check_image_crc
* 		purpose: Performs crc on the entire image.  
* Return value: 
* 	DPINT: User defined integer value which reports DPE_SUCCESS if there is a match or DPE_CRC_MISMATCH if failed. 
* 
*/
/*
* Module: dp_check_image_crc
* 		purpose: Performs crc on the entire image.  
* Return value: 
* 	DPINT: User defined integer value which reports DPE_SUCCESS if there is a match or DPE_CRC_MISMATCH if failed. 
* 
*/
void dp_check_image_crc(void)
{
    DPUINT expected_crc;
    #ifdef ENABLE_DEBUG        
    dp_display_text("\r\nChecking data CRC...");
    #endif
    
    global_ulong = dp_get_bytes(Header_ID,0u,4u);
    if (global_ulong == 0x69736544)
    {
        requested_bytes = 0u;
        image_size = dp_get_bytes(Header_ID,IMAGE_SIZE_OFFSET,4);
        expected_crc = (DPUINT) dp_get_bytes(Header_ID,image_size - 2,2);
        #ifdef ENABLE_DEBUG        
        dp_display_text("\r\nExpected CRC=");
        dp_display_value( expected_crc ,HEX);
        #endif
        if (image_size == 0)
        {
            error_code = DPE_CRC_MISMATCH;
            #ifdef ENABLE_DEBUG
            dp_display_text("\r\nData file is not loaded... \r\n");
            #endif
        }
        else 
        {
            #ifdef ENABLE_DEBUG      
            dp_display_text("\r\nCalculating actual CRC...");
            #endif
            /* Global_uint is used to hold the value of the calculated CRC */
            global_uint = 0;
            /* DataIndex is used to keep track the byte position in the image that is needed per get_data_request */
            DataIndex = 0;
            requested_bytes = image_size - 2;
            while (requested_bytes)
            {
                page_buffer_ptr = dp_get_data(Header_ID,DataIndex*8);
                if (return_bytes > requested_bytes )
                    return_bytes = requested_bytes;
                for (global_ulong=0; global_ulong< return_bytes;global_ulong++)
                {
                    global_uchar = page_buffer_ptr[global_ulong];
                    dp_compute_crc();
                }
                DataIndex += return_bytes;
                requested_bytes -= return_bytes;
            }
            if (global_uint != expected_crc)
            {
                #ifdef ENABLE_DEBUG
                dp_display_text("\r\nCRC verification failed.  Expected CRC = ");
                dp_display_value(global_uint,HEX);
                dp_display_text(" Actual CRC = ");
                dp_display_value((DPUINT) dp_get_bytes(Header_ID,image_size - 2,2),HEX);
                dp_display_text("\r\n");
                #endif
                error_code = DPE_CRC_MISMATCH;
            }
        }
    }
    else
    {
        #ifdef ENABLE_DEBUG
        dp_display_text("\r\nData file is not valid. ");
        #endif
        error_code = DPE_CRC_MISMATCH;
    }
    
    
    return;
}

void dp_compute_crc(void)
{
    DPUINT idx;
    for (idx = 0; idx < 8; idx++)
    {
        device_rows = (global_uchar ^ global_uint) & 0x01;
        global_uint >>= 1;
        if (device_rows)
            global_uint ^= 0x8408;
        global_uchar >>= 1;
    }
    return;
}


