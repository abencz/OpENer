/* ************************************************************************ */
/*                                                                          */
/*  IAP 	       Copyright (C) Actel Corporation 2010                 */
/*  Version 1.0	       Release date February  17, 2010                      */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         gpio.h						    */
/*                                                                          */
/*  Description:    user specific file containing JTAG interface functions  */
/*                  and delay function                                      */
/*                                                                          */
/****************************************************************************/


unsigned char jtag_inp(void);
void jtag_outp(unsigned char outdata);
void dp_jtag_init(void);
void dp_jtag_tms(unsigned char tms);
void dp_jtag_tms_tdi(unsigned char tms, unsigned char tdi);
unsigned char dp_jtag_tms_tdi_tdo(unsigned char tms, unsigned char tdi);


/*************** Hardware related constants *****************************/
/*
* User Attention: 
* Bit assignments in the hardware JTAG port register 
* 
*/
#define TCK     0x01u
#define TDI     0x10u
#define TMS     0x04u
#define TRST    0x08u /* 0 Means it does not exist */
#define TDO     0x02u
/*************** End of hardware related constants ************************/

