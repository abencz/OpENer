/*******************************************************************************
* (c) Copyright 2009 Actel Corporation.  All rights reserved.
*
* SVN $Revision:$
* SVN $Date:$
*/
#ifndef IAP_H_
#define IAP_H_

void IAP_IRSCAN_in(void);
void IAP_IRSCAN_out(unsigned char* outbuf);
void IAP_DRSCAN_in(unsigned long start_bit_index, unsigned int bits_to_shift,unsigned char* inbuf);
void IAP_DRSCAN_out(unsigned int bits_to_shift,unsigned char* inbuf,unsigned char* outbuf);
void IAP_get_and_DRSCAN_in(unsigned char Variable_ID,unsigned int total_bits_to_shift, unsigned long start_bit_index);
void IAP_get_and_DRSCAN_in_out(unsigned char Variable_ID,unsigned int total_bits_to_shift, unsigned long start_bit_index, unsigned char *tdo_data);

#endif /*IAP_H_*/
