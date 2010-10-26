####################################################
# Copyright (c) 2010, Rockwell Automation, Inc.
# All rights reserved.
# This file is part of not-an-OS for the Cortex-M3.
# BSD licensed, see README.txt
#
# Author: Jonathan Engdahl jrengdahl@ra.rockwell.com
####################################################

ifndef CROSS

#if CROSS is not externally defined, 
#then it must be defined here. Unncomment one of the following definitions:

# Use this if your path points to the cross compiler's bin directory:
#CROSS=

# else modify this line to suit:
CROSS=c:/cross/softconsole-v3.2/sourcery-g++/bin

endif


PROGRAM=main
ROOT=../..
CIP=$(ROOT)/cip
ENCAP=$(ROOT)/enet_encap
LWIP=$(ROOT)/ports/lwip-1.3.2/src
PORTS=$(ROOT)/ports/lwip-1.3.2/ports
DRIVERS=$(ROOT)/ports/HW_SmartFusion_demo_verilog/firmware/drivers
DCONFIG=$(ROOT)/ports/HW_SmartFusion_demo_verilog/firmware/drivers_config
CMSIS=$(ROOT)/ports/HW_SmartFusion_demo_verilog/firmware/CMSIS
BSP=BSP
MYLIB=$(ROOT)/ports/myLib
SPRINTF=$(ROOT)/ports/sprintf

# Try to figure out of the UNIX commands are available
# The Makefile should work without them, but more info is available if they are.

ifeq (,$(findstring Windows,$(OS)))		# not Windows
HAVEUNIX=1
endif
ifneq (,$(findstring cygdrive,$(PATH)))		# Cygwin is installed
HAVEUNIX=1
endif

ifdef HAVEUNIX
NULL=/dev/null
else
NULL=NUL:
endif


PREFIX = $(CROSS)/arm-none-eabi-
CC=$(PREFIX)gcc
CP=$(PREFIX)g++
AS=$(PREFIX)as
AR=$(PREFIX)ar
LD=$(PREFIX)gcc
OBJCOPY=$(PREFIX)objcopy
OBJDUMP=$(PREFIX)objdump
NM=$(PREFIX)nm
MAKE=${shell $(CROSS)/make --version > $(NULL) 2>&1 && echo $(CROSS)/make}
RM=${shell $(CROSS)/rm --version > $(NULL) -v 2>&1 && echo $(CROSS)/rm}
ifeq (,$(MAKE))
MAKE=${shell $(CROSS)/cs-make --version > $(NULL) 2>&1 && echo $(CROSS)/cs-make}
RM=${shell $(CROSS)/cs-rm --version > $(NULL) 2>&1 && echo $(CROSS)/cs-rm}
endif
ifeq (,$(MAKE))
MAKE=make
RM=rm
endif

CPU=-mcpu=cortex-m3
THUMB=-mthumb

#normal optimization
#OPTIM=-Os
# for easier debug with a little optimization
# inhibit instruction reordering
OPTIM=-Os -fno-sched-interblock -fno-sched-spec -fno-schedule-insns -fno-schedule-insns2

DEBUG=-g3
#DEBUG=-g3 -DOPENER_WITH_TRACES -DOPENER_TRACE_LEVEL=15

DEFINES= -D SMARTFUSION_FCLK_FREQ=100000000uL

INCLUDES= \
	-I . \
	-I $(ROOT) \
	-I $(CIP) \
	-I $(ENCAP) \
	-I $(CMSIS) \
	-I $(DRIVERS) \
	-I $(DRIVERS)/mss_ethernet_mac \
	-I $(DRIVERS)/mss_uart \
	-I $(DRIVERS)/mss_gpio \
	-I $(DRIVERS)/mss_watchdog \
	-I $(DRIVERS)/mss_nvm \
	-I $(DRIVERS)/mss_i2c \
	-I $(BSP) \
	-I $(LWIP) \
	-I $(LWIP)/include \
	-I $(LWIP)/include/ipv4 \
	-I $(LWIP)/include/ipv4/lwip \
	-I $(LWIP)/include/netif \
	-I $(LWIP)/netif \
	-I $(PORTS)/smartfusion/include


FLAGS=	$(CPU) \
	$(THUMB) \
	$(OPTIM) \
	$(DEBUG) \
	$(INCLUDES) \
	$(DEFINES) \
	-fno-exceptions \
	-Wreturn-type \
	-ffunction-sections \
	-fdata-sections

CFLAGS=$(FLAGS) -std=gnu99
CPPFLAGS=$(FLAGS) -fno-implement-inlines
LDFLAGS=$(FLAGS) -nostartfiles

LDSCRIPT=$(CMSIS)/startup_gcc/debug-in-actel-smartfusion-envm.ld



OBJS=	main.o \
	threads.o \
	interp.o \
	getline.o \
	dump.o \
	stdio.o \
	stubs.o \
	CIPApp.o \
	networkhandler.o \
	$(CIP)/appcontype.o \
	$(CIP)/cipassembly.o \
	$(CIP)/cipcommon.o \
	$(CIP)/cipconnectionmanager.o \
	$(CIP)/cipethernetlink.o \
	$(CIP)/cipidentity.o \
	$(CIP)/cipmessagerouter.o \
	$(CIP)/ciptcpipinterface.o \
	$(ENCAP)/cpf.o \
	$(ENCAP)/endianconv.o \
	$(ENCAP)/encap.o \
	$(CMSIS)/system_a2fxxxm3.o \
	$(CMSIS)/startup_gcc/startup_a2fxxxm3.o \
	$(CMSIS)/core_cm3.o \
	$(DRIVERS)/mss_ethernet_mac/crc32.o \
	$(DRIVERS)/mss_ethernet_mac/mss_ethernet_mac.o \
	$(DRIVERS)/mss_ethernet_mac/phy.o \
	$(DRIVERS)/mss_uart/mss_uart.o \
	$(DRIVERS)/mss_gpio/mss_gpio.o \
	$(DRIVERS)/mss_nvm/mss_nvm.o \
	$(DRIVERS)/mss_i2c/mss_i2c.o \
	$(BSP)/oled_driver/oled.o \
	$(LWIP)/core/dhcp.o \
	$(LWIP)/core/init.o \
	$(LWIP)/core/mem.o \
	$(LWIP)/core/memp.o \
	$(LWIP)/core/netif.o \
	$(LWIP)/core/pbuf.o \
	$(LWIP)/core/raw.o \
	$(LWIP)/core/tcp.o \
	$(LWIP)/core/tcp_in.o \
	$(LWIP)/core/tcp_out.o \
	$(LWIP)/core/udp.o \
	$(LWIP)/core/ipv4/autoip.o \
	$(LWIP)/core/ipv4/icmp.o \
	$(LWIP)/core/ipv4/inet.o \
	$(LWIP)/core/ipv4/inet_chksum.o \
	$(LWIP)/core/ipv4/ip.o \
	$(LWIP)/core/ipv4/ip_frag.o \
	$(LWIP)/core/ipv4/ip_addr.o \
	$(LWIP)/netif/etharp.o \
	$(PORTS)/smartfusion/netif/smartfusionif.o

LIBDIRS=-L${MYLIB} \
	-L${SPRINTF}

LIBS=	-lmyLib \
	-lsprintf

SUBDIRS=${MYLIB} ${SPRINTF}

all: $(PROGRAM).axf

$(PROGRAM).axf: $(OBJS) ${LDSCRIPT} subdirs Makefile
	@echo "  [LD]	" $@
	@$(LD) $(LDFLAGS) $(OBJS) $(LIBDIRS) $(LIBS) -T $(LDSCRIPT) -Wl,-Map=$(PROGRAM).map -o $@
ifdef HAVEUNIX
	@$(OBJDUMP) -h $(PROGRAM).axf | awk -f size.awk
endif
	@$(OBJCOPY) -O binary -S $@ $(PROGRAM).bin
	@$(OBJDUMP) -dS $@ > $(PROGRAM).disasm
	@echo BUILD COMPLETE

# pull in dependency info for existing .o files
-include .deps/*.d

%.o: %.cpp .deps Makefile
	@echo "  [C+]	" $*.cpp
	@$(CP) -c $(CPPFLAGS) -MMD -MP -MF .deps/$(*F).d $*.cpp -o $*.o

%.o: %.c .deps Makefile
	@echo "  [CC]	" $*.c
	@$(CC) -c $(CFLAGS) -MMD -MP -MF .deps/$(*F).d $*.c -o $*.o

%.o: %.S .deps Makefile
	@echo "  [AS]	" $*.S
	@$(CC) -c $(CFLAGS) -MMD -MP -MF .deps/$(*F).d $*.S -o $*.o
		
%.o: %.s  Makefile
	@echo "  [as]	" $*.s
	@$(CC) -c $(CFLAGS) $*.s -o $*.o
	
.deps:
	mkdir .deps

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)
	
$(SUBDIRS):
	@$(MAKE) -C $@ CROSS=$(CROSS)

clean:
	@echo "  [CLEAN]"
	@-$(RM) -rf $(OBJS) *.axf *.bin *.map *.disasm .deps 2> $(NULL) 

realclean : clean
	@echo "  REALCLEAN"
	$(MAKE) -C ${MYLIB}    CROSS=$(CROSS) clean
	$(MAKE) -C ${SPRINTF}  CROSS=$(CROSS) clean
