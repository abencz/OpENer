/* ************************************************************************ */
/*                                                                          */
/*  IAP 	       Copyright (C) Actel Corporation 2010                 */
/*  Version 1.0	       Release date February  17, 2010                      */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         urow_iap.h                                              */
/*                                                                          */
/*  Description:    Contains the function prototypes.                       */
/*                                                                          */
/* ************************************************************************ */

#ifndef INC_UROW_IAP_H
#define INC_UROW_IAP_H
extern DPUCHAR actual_iap_urow[16];

void init_urow_nvm(void);
void check_envm_status(void);
void program_urow_nvm(void);
void read_urow_nvm(void);
void do_program_urow_nvm(void);
void do_read_urow_nvm(void);
void dp_compute_urow_crc(DPUCHAR value);
void do_program_urow_crc_nvm(void);
void dp_display_urow_nvm(void);
#endif
