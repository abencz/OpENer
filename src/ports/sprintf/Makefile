#customization: make sure the following point to sensible places:

# The prefix of the cross compiler commands --
# Use this if your path points to the cross compiler's bin directory:
CROSS=arm-none-eabi-

# else modify this line to suit:
#CROSS=c:/cross/softconsole-v3.2/sourcery-g++/bin/arm-none-eabi-


PROGRAM=libsprintf

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


CC=$(CROSS)gcc
CP=$(CROSS)g++
AS=$(CROSS)as
AR=$(CROSS)ar
LD=$(CROSS)gcc
OBJCOPY=$(CROSS)objcopy
OBJDUMP=$(CROSS)objdump
NM=$(CROSS)nm
MAKE=${shell type make > $(NULL) 2>&1 && echo make || echo cs-make}
RM=${shell type rm > $(NULL) 2>&1 && echo rm || echo cs-rm}

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

OBJS=	sprintf.o

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

