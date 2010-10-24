/* ************************************************************************ */
/*                                                                          */
/*  IAP 	       Copyright (C) Actel Corporation 2010                 */
/*  Version 1.0	       Release date February  17, 2010                      */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpuser.h                                                */
/*                                                                          */
/*  Description:    DP user specific file                                   */
/*                  users should define their own functions                 */
/*                                                                          */
/* ************************************************************************ */
#ifndef INC_DPUSER_H
#define INC_DPUSER_H

#include <stdint.h>

/*************** End of hardware related constants ************************/


/* Compiler switches */
#define ACTEL_DEBUG
#define ENABLE_DEBUG

#define ENABLE_IAP_SUPPORT
#define ENABLE_GPIO_SUPPORT

#define USE_PAGING
/*  #define CHAIN_SUPPORT */

#define CORE_SUPPORT
#define CORE_PLAIN
#define CORE_ENCRYPT
#define FROM_SUPPORT
#define FROM_PLAIN
#define FROM_ENCRYPT
#define NVM_SUPPORT
#define NVM_PLAIN
#define NVM_ENCRYPT
#define SECURITY_SUPPORT
#define SILSIG_SUPPORT
#define ENABLE_DAS_SUPPORT

/* Enable BSR_SAMPLE switch maintains the last known state of the IOs regardless 
*  of the data file setting. */
/* #define BSR_SAMPLE */
/* Nano device support is not enabled with ENABLE_V85_DAT_SUPPORT switch */
/* You must disable this compile switch if the data file generated is from 
Designer version V86 or higher */
/* #define ENABLE_V85_DAT_SUPPORT */

/* Code optimization specific compile switches */
#define ENABLE_CODE_SPACE_OPTIMIZATION 
/* #define DISABLE_CORE_SPECIFIC_ACTIONS */
/* #define DISABLE_FROM_SPECIFIC_ACTIONS */
/* #define DISABLE_NVM_SPECIFIC_ACTIONS */
/* #define DISABLE_SEC_SPECIFIC_ACTIONS */

/* This option could be used for performing progam_nvm_active_array and 
verify_nvm_active_array actions. 
Set FORCE_NVM_ACCESS to 1 to force NVM access */
#define FORCE_NVM_ACCESS 0u


/***********************************************/
/* DPCHAR    -- 8-bit Windows (ANSI) character */
/*              i.e. 8-bit signed integer      */
/* DPINT     -- 16-bit signed integer          */
/* DPLONG    -- 32-bit signed integer          */
/* DPBOOL    -- boolean variable (0 or 1)      */
/* DPUCHAR   -- 8-bit unsigned integer         */
/* DPUSHORT  -- 16-bit unsigned integer        */
/* DPUINT    -- 16-bit unsigned integer        */
/* DPULONG   -- 32-bit unsigned integer        */
/***********************************************/
typedef unsigned char  DPUCHAR;
typedef unsigned short DPUSHORT;
typedef unsigned int   DPUINT;
typedef unsigned long  DPULONG;
typedef unsigned char  DPBOOL;
typedef   signed char  DPCHAR;
typedef   signed int   DPINT;
typedef   signed long  DPLONG;

#define DPNULL ((void*)0)
#define TRUE 1U
#define FALSE 0U

#define GPIO_SEL 1
#define IAP_SEL 2

extern DPUCHAR *image_buffer;
extern DPUCHAR hardware_interface;

void dp_delay(DPULONG microseconds);

#ifdef ENABLE_DEBUG
#define HEX 0u
#define DEC 1u
#define CHR 2u

/******************************************************************************/
/* users should define their own functions to replace the following functions */
/******************************************************************************/
void dp_display_text(DPCHAR * text);
void dp_display_value(DPULONG value,int descriptive);
void dp_display_array(unsigned char * outbuf, unsigned int bytes, int Descriptive);
#endif

#endif /* INC_DPUSER_H */


