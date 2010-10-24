This is not-an-OS for the Cortex-M3
A bare-metal framework for lightweight firmwares
================================================

The software in this directory provides powerup, thread switching, I/O,
event handling, and other services in support of the "opENer"
Ethernet/IP Communication Stack.

/*
 * Copyright (c) 2010, Rockwell Automation, Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 *  * Neither the name of Rockwell Automation, nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * (This is the BSD license, see http://opensource.org/licenses/bsd-license.php)
 *
 * Contributors:
 * 2010.01.11 Jonathan Engdahl jrengdahl@ra.rockwell.com
 *
 */

The following files are components of not-an-OS, and are not components of
the Ethernet/IP Communication Stack. These are copyright by Rockwell
Automation, Inc. and covered by the adapted BSD license above:

Makefile
dump.c
getline.c
interp.c
local.h
lwipinc.h
main.c
malloc.c
mutex.h
parm.h
sbrk.S
sbrk.h
serial.h
stdio.c
stubs.c
threads.S
threads.h


The following files in this directory are components of or derived from
the Ethernet/IP Communication Stack, and are subject to the terms
described in its license.txt:

CIPApp.c
config.h
networkhandler.c
networkhandler.h
opener_user_conf.h
report.h
typedefs.h


Other contents of this directory may be copyright and licensed by others,
or may be derived from multiple sources and covered by multiple copyrights
and licenses.
