/* ************************************************************************ */
/*                                                                          */
/*  IAP 	       Copyright (C) Actel Corporation 2010                 */
/*  Version 1.0	       Release date February  17, 2010                      */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpcom.h                                                 */
/*                                                                          */
/*  Description:    Contains functions prototypes needed to access the data */
/*	from external flash or other means of communication		    */
/*                                                                          */
/****************************************************************************/

#ifndef INC_DPUTIL_H
#define INC_DPUTIL_H

void dp_flush_global_buf1(void);
void dp_flush_global_buf2(void);
void dp_init_vars(void);
//! Function used to identify which block is supported in the dat file and their encryption status.
void dp_get_dat_support_status(void);
void dp_compute_crc(void);
void dp_check_image_crc(void);
void dp_check_dat_support_version(void);
int int_to_hex_int
(
int value,
uint8_t * p_result,
int result_size
);
int int_to_dec_int
(
int value,
uint8_t * p_result,
int result_size
);

#endif /* INC_DPUTIL_H */


