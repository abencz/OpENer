/*******************************************************************************
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * This file is part of not-an-OS for the Cortex-M3.
 * BSD licensed, see README.txt
 *
 * Author: Jonathan Engdahl jrengdahl@ra.rockwell.com
 ******************************************************************************/

#ifndef PARM_H
#define PARM_H

// ip parameters structure and random seed stored in nflash ROM
// this is very rudimentary
// TODO -- for anything beyond a demo, add wear leveling, more error checking

struct parm
  {
    int useStatic; // 1 use static IP address, 0 use DHCP
    unsigned long ip; // my ip address
    unsigned long nm; // net mask
    unsigned long gw; // gateway ip address
  };

typedef unsigned char MACID[6];

// define the resions in flash that hold the parameters

#define NVM_PARM (*(struct parm *)(0x60000000 + 255*1024))
#define NVM_SEED (*(unsigned long *)(&NVM_PARM + 1))
#define NVM_MACID (*(MACID *)(&NVM_SEED + 1))
#define NVM_PAD0  (*(short *)(&NVM_MACID + 1))


#endif
