/*******************************************************************************
* (c) Copyright 2008 Actel Corporation.  All rights reserved.
*
* SVN $Revision: 761 $
* SVN $Date: 2009-03-24 16:39:51 +0000 (Tue, 24 Mar 2009) $
*/
#include "a2fxxxm3.h"
#include "iap_hw_interface.h"
#include "dpuser.h"
#include "dpscan.h"
#include "dpalg.h"
#include "dpcom.h"
#include "mss_iap.h"

extern unsigned char opcode;
/*------------------------------------------------------------------------------
* IAP_STATUS_IDLE_MASK:
*   IDLE field of IAP STATUS register.
*------------------------------------------------------------------------------
* IDLE indicates the IAP state machine is IDLE and is ready to process another 
* command. If asserted then an APB write transaction is guaranteed not to 
* generate wait states.
*/

void IAP_DRSCAN_in(unsigned long start_bit_index, unsigned int bits_to_shift,unsigned char* inbuf)
{
    DPUINT bits_shifted;
    DPULONG shift_in_value;
    DPUINT idx;
    
    
    bits_shifted = 0;
    shift_in_value = 0;
    for(idx=0;idx<(bits_to_shift+7)/8;idx++)
    {
        if (inbuf != DPNULL)
        {
            shift_in_value |= ((inbuf[idx] >> start_bit_index) << (8*idx-bits_shifted));
        }
        
        if ( ((idx+1)*8 - bits_shifted) == 32)
        {
            IAP_do_DRSCAN_in(32,shift_in_value);
            shift_in_value=0;
            bits_shifted+=32;
        }
    }
    if (bits_shifted != bits_to_shift)
    {
        IAP_do_DRSCAN_in(bits_to_shift-bits_shifted,shift_in_value);
    }
    
}

void IAP_DRSCAN_out(unsigned int bits_to_shift,unsigned char* inbuf,unsigned char* outbuf)
{
    DPUINT bits_shifted;
    DPULONG shift_in_value;
    DPULONG return_value;
    DPUINT idx;
    DPINT i;
    
    bits_shifted = 0;
    shift_in_value = 0;
    for(idx=0;idx<(bits_to_shift+7)/8;idx++)
    {
        if (inbuf != DPNULL)
        {
            shift_in_value |= (inbuf[idx] << (8*idx-bits_shifted));
        }
        
        if ( ((idx+1)*8 - bits_shifted) == 32)
        {
            return_value = IAP_do_DRSCAN_out(32,shift_in_value);
            for(i=idx-3;i<=idx;i++)
            {
                outbuf[i] = return_value >> (i - (idx - 3))*8;
            }
            shift_in_value=0;
            bits_shifted+=32;
        }
    }
    if (bits_shifted != bits_to_shift)
    {
        return_value = IAP_do_DRSCAN_out(bits_to_shift-bits_shifted,shift_in_value);
        for(i = idx - (idx % 4); i < idx; i++)
        {
            outbuf[i] = return_value >> (i - (idx - (idx % 4)))*8;
        }
    }
    
}

void IAP_get_and_DRSCAN_in(unsigned char Variable_ID,unsigned int total_bits_to_shift, unsigned long start_bit_index)
{
    DPULONG page_start_bit_index;
    DPUINT bits_to_shift;
    DPUINT bits_shifted;
    DPULONG shift_in_value;
    DPUINT idx;
    DPUINT current_bits_to_shift;
    
    page_start_bit_index = start_bit_index & 0x7;
    shift_in_value = 0;
    
    requested_bytes =  (page_start_bit_index + total_bits_to_shift + 7) >> 3;
    while (requested_bytes)
    {
        page_buffer_ptr = dp_get_data(Variable_ID,start_bit_index);
        
        
        if (return_bytes >= requested_bytes )
            {
            return_bytes = requested_bytes;
            bits_to_shift = total_bits_to_shift;
        }
        else bits_to_shift = (DPUCHAR) (return_bytes * 8 - page_start_bit_index);
        
        bits_shifted = 0;
        current_bits_to_shift = 0;
        
        for(idx=0;idx<(bits_to_shift+7)/8;idx++)
        {
            shift_in_value |= (page_buffer_ptr[idx] << (8*idx-bits_shifted));
            current_bits_to_shift += 8;
            
            if ( current_bits_to_shift  == 32)
            {
                if (idx<4)
                {
                    shift_in_value >>= page_start_bit_index;
                    current_bits_to_shift -= page_start_bit_index;
                }
                if (current_bits_to_shift > (bits_to_shift - bits_shifted))
                {
                    current_bits_to_shift = bits_to_shift - bits_shifted;
                }
                IAP_do_DRSCAN_in(current_bits_to_shift,shift_in_value);
                bits_shifted+=current_bits_to_shift;
                shift_in_value=0;
                current_bits_to_shift = 0;
            }
        }
        if (bits_shifted != bits_to_shift)
        {
            if (idx < 4)
            {
                shift_in_value >>= page_start_bit_index;
            }
            current_bits_to_shift = bits_to_shift - bits_shifted;
            IAP_do_DRSCAN_in(current_bits_to_shift,shift_in_value);
            bits_shifted+=current_bits_to_shift;
            shift_in_value=0;
            current_bits_to_shift = 0;			
        }
        requested_bytes = requested_bytes - return_bytes;
        total_bits_to_shift = total_bits_to_shift - bits_to_shift;
        start_bit_index += bits_to_shift;
        page_start_bit_index = start_bit_index & 0x7;
    }
}

// This is needed for NVM programming.  It is true that the start_bit_index will always start at 0.
void IAP_get_and_DRSCAN_in_out(unsigned char Variable_ID,unsigned int total_bits_to_shift, unsigned long start_bit_index, unsigned char *tdo_data)
{
    // Find out the total number of bytes needed from the image.
    requested_bytes =  (DPULONG) (total_bits_to_shift + 7) >> 3;
    
    // Get the data from the image
    page_buffer_ptr = dp_get_data(Variable_ID,start_bit_index);
    
    // If not all the bytes are available, then we have to adjust the number of buts to shift.
    if (return_bytes >= requested_bytes )
        {
        return_bytes = requested_bytes;
    }
    else
    {
        #ifdef ENABLE_DEBUG          
        dp_display_text("\r\nError: Page buffer size is not big enough...");
        #endif
    }
    
    DRSCAN_out(total_bits_to_shift, page_buffer_ptr, tdo_data); 
    return;
}
