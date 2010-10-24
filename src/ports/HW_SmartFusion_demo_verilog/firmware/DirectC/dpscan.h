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
#ifndef INC_DPSCAN_H
#define INC_DPSCAN_H

void IRSCAN_in(void);
void IRSCAN_out(unsigned char* outbuf);
void DRSCAN_in(unsigned long start_bit_index, unsigned int bits_to_shift,unsigned char* inbuf);
void DRSCAN_out(unsigned int bits_to_shift,unsigned char* inbuf,unsigned char* outbuf);
void goto_jtag_state( unsigned char jtag_state, unsigned int cycles );

#endif /* INC_DPSCAN_H */
