/* ************************************************************************ */
/*                                                                          */
/*  IAP 	       Copyright (C) Actel Corporation 2010                 */
/*  Version 1.0	       Release date February  17, 2010                      */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         gpio.c						    */
/*                                                                          */
/*  Description:    user specific file containing JTAG interface functions  */
/*                  and delay function                                      */
/*                                                                          */
/****************************************************************************/

#include "dpuser.h"
#include "dpalg.h"
#include "dpdef.h"
#include "dputil.h"
#include <assert.h>
#include "a2fxxxm3.h"
#include "gpio_hw_interface.h"


/*
* User attention:
* jtag_port_reg: 	8 bit Static variable to keep track of the state of all the JTAG pins 
* 					at all times during the programming operation.
* Note: User check the variable size to make sure it can cover the hardware IO register. 
* 
*/
static DPUCHAR jtag_port_reg;
/*
* User attention: 
* Module: jtag_inp
* 		purpose: report the logic state of tdo jtag pin
* Arguments: None
* Return value: 8 bit value
* 		0, 0x80
* 
*/
DPUCHAR jtag_inp(void)
{
    uint32_t tdo = 0;
    uint8_t i = 0;
    
    tdo = (GPIO->GPIO_IN);
    tdo = tdo >> 11;
    
    if (tdo != 0)
        i++;
    
    if ( (tdo&TDO) == 0)
        return 0x00;
    else
    return 0x80;
}
/*
* User attention: 
* Module: jtag_outp
* 		purpose: Set the JTAG port (all JTAG pins)
* Arguments: 8 bit value containing the new state of all the JTAG pins
* Return value: None
* 
*/
void jtag_outp(DPUCHAR outdata)
{
    uint32_t gpio_data;
    //we'll shift everything by 11 since TCK is on GPIO_11 and add trst
    gpio_data = outdata;
    gpio_data = gpio_data << 11;
    GPIO->GPIO_OUT = gpio_data;
}

/*
* No need to change this function
* Module: dp_jtag_init
* 		purpose: Set tck and trstb pins to logic level one
* Arguments:
* 		None
* Return value: None
* 
*/
void dp_jtag_init(void)
{
    jtag_port_reg = TCK | TRST; 
    jtag_outp(jtag_port_reg);
}

/*
* No need to change this function
* Module: dp_jtag_tms
* 		purpose: Set tms pin to a logic level one or zero and pulse tck.
* Arguments: 
* 		tms: 8 bit value containing the new state of tms
* Return value: None
* Constraints: Since jtag_outp function sets all the jtag pins, jtag_port_reg is used 
* 				to modify the required jtag pins and preseve the reset.
* 
*/
void dp_jtag_tms(DPUCHAR tms)		 
{	
    jtag_port_reg &= ~(TMS | TCK);
    jtag_port_reg |= (tms ? TMS : 0u);
    jtag_outp(jtag_port_reg);
    jtag_port_reg |= TCK;
    jtag_outp(jtag_port_reg);
}

/*
* No need to change this function
* Module: dp_jtag_tms_tdi
* 		purpose: Set tms amd tdi pins to a logic level one or zero and pulse tck.
* Arguments: 
* 		tms: 8 bit value containing the new state of tms
* 		tdi: 8 bit value containing the new state of tdi
* Return value: None
* Constraints: Since jtag_outp function sets all the jtag pins, jtag_port_reg is used 
* 				to modify the required jtag pins and preseve the reset.
* 
*/
void dp_jtag_tms_tdi(DPUCHAR tms, DPUCHAR tdi)
{
    jtag_port_reg &= ~(TMS | TCK | TDI);
    jtag_port_reg |= ((tms ? TMS : 0u) | (tdi ? TDI : 0u));
    jtag_outp(jtag_port_reg);
    jtag_port_reg |= TCK;
    jtag_outp(jtag_port_reg);
}

/*
* No need to change this function
* Module: dp_jtag_tms_tdi_tdo
* 		purpose: Set tms amd tdi pins to a logic level one or zero, 
* 				 pulse tck and return tdi level
* Arguments: 
* 		tms: 8 bit value containing the new state of tms
* 		tdi: 8 bit value containing the new state of tdi
* Return value: 
* 		ret: 8 bit variable ontaining the state of tdo.
* Valid return values: 
* 		0x80: indicating a logic level high on tdo
* 		0: indicating a logic level zero on tdo
* Constraints: Since jtag_outp function sets all the jtag pins, jtag_port_reg is used 
* 				to modify the required jtag pins and preseve the reset.
* 
*/
DPUCHAR dp_jtag_tms_tdi_tdo(DPUCHAR tms, DPUCHAR tdi)
{
    DPUCHAR ret = 0x80u;
    jtag_port_reg &= ~(TMS | TCK | TDI);
    jtag_port_reg |= ((tms ? TMS : 0u) | (tdi ? TDI : 0u));
    jtag_outp(jtag_port_reg);
    ret = jtag_inp();
    jtag_port_reg |= TCK;
    jtag_outp(jtag_port_reg);
    return ret;
}



