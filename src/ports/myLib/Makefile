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


PROGRAM=libmyLib


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
OPTIM=-Os
# for easier debug with a little optimization
# inhibit instruction reordering
#OPTIM=-Os -fno-sched-interblock -fno-sched-spec -fno-schedule-insns -fno-schedule-insns2

DEBUG=-g3

INCLUDES= -I.

DEFINES=


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

OBJS=	memcmp.o \
	memcpy.o \
	memset.o \
	strcat.o \
	strchr.o \
	strcmp.o \
	strcpy.o \
	strlen.o \
	strncat.o \
	strncmp.o \
	strncpy.o \
	strnlen.o \
	strrchr.o \
	strstr.o \
	sbrk.o \
	malloc.o \
	calloc.o

all: $(PROGRAM).a

$(PROGRAM).a: $(OBJS) Makefile
	@echo "  [AR]	" ${<}
	@$(AR) -cr $(PROGRAM).a $(OBJS)
	@echo "  DONE"


%.o: %.c Makefile
	@echo "  [CC]	" $*.c
	@$(CC) -c $(CFLAGS) $< -o $@

%.o: %.S Makefile
	@echo "  [AS]	" $*.S
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@echo "  CLEAN"
	@-$(RM) -f $(OBJS) $(PROGRAM).a  2> $(NULL)

