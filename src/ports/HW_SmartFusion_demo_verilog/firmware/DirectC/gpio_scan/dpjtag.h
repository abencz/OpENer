/* ************************************************************************ */
/*                                                                          */
/*  IAP 	       Copyright (C) Actel Corporation 2010                 */
/*  Version 1.0	       Release date February  17, 2010                      */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpjtag.h                                                */
/*                                                                          */
/*  Description:    Definitions of JTAG constants, types, and functions     */
/*                                                                          */
/* ************************************************************************ */

#ifndef INC_DPJTAG_H
#define INC_DPJTAG_H

/****************************************************************************/
/* Function prototypes                                                      */
/****************************************************************************/
void dp_wait_cycles(unsigned char cycles);
void JTAG_goto_jtag_state(unsigned char target_state,unsigned char cycles);
void dp_shift_in(unsigned long start_bit, unsigned int num_bits, unsigned char tdi_data[],unsigned char terminate);
void dp_shift_in_out(unsigned int num_bits, unsigned char tdi_data[],unsigned char tdo_data[]);
void dp_get_and_shift_in(unsigned char Variable_ID,unsigned int total_bits_to_shift, unsigned long start_bit_index);
void dp_get_and_shift_in_out(unsigned char Variable_ID,unsigned char total_bits_to_shift, unsigned long start_bit_index,unsigned char* tdo_data);
void JTAG_IRSCAN_in(void);
void JTAG_IRSCAN_out(unsigned char* outbuf);
void JTAG_DRSCAN_out(unsigned int bits_to_shift,unsigned char* inbuf,unsigned char* outbuf);
void JTAG_DRSCAN_in(unsigned long start_bit_index, unsigned int bits_to_shift,unsigned char* inbuf);
void JTAG_get_and_DRSCAN_in(unsigned char Variable_ID,unsigned int total_bits_to_shift, unsigned long start_bit_index);
void JTAG_get_and_DRSCAN_in_out(unsigned char Variable_ID,unsigned int total_bits_to_shift, unsigned long start_bit_index,unsigned char* tdo_data);

extern unsigned char * page_buffer_ptr;
extern unsigned char global_jtag_i;
extern unsigned char current_jtag_state;
extern unsigned char idx;
extern unsigned char data_buf;
extern unsigned char bit_buf;
extern unsigned char error_code;

#endif /* INC_DPJTAG_H */
