This directory contains the Actel SmartFusion port of opENer.

The SmartFusion is a System-on-Chip (SoC) with the following features:

-- 100 MHz Cortex-M3 processor
-- 256K flash ROM
-- 64K RAM
-- 200K gate on-chip FPGA fabric
-- Ethernet
-- 12 bit A/D and D/A with smart analog sampling engine
-- other typical SoC I/O (timers, DMA, UARTs, SPI, I2C, etc.)

This port of opENer uses lwIP in raw mode, and does not have an RTOS.
There is a primitive thread switcher called not-an-OS. The thread
switcher has three primitives which together take 27 assembly
instructions. The system is driven by a background polling loop. There
is a 1 ms timer tick which simply increments a counter. There is one
thread to drive lwIP, and another which implemts a simple command line
processor with a few debug commands, plus commands for setting things
like the MAC ID and IP address..

This port takes 56K of flash and 37K of RAM. With some tuning of lwIP's
buffer pool the RAM requirements can be less than 32K.

The CIP application sends outputs to the LEDs on the SmartFusion eval board.

The SmartFusion evaluation board costs around $100 from Mouser or Arrow.
The FPGA and software development tools are free downloads from the
Actel website. The software development suite is a special edition of
Sourcery G++.

See:
http://www.actel.com/products/SmartFusion/default.aspx
http://www.actel.com/products/hardware/devkits_boards/smartfusion_eval.aspx


This port of opENer uses lwIP in raw mode, and does not have an RTOS.
There is a primitive thread switcher called not-an-OS. The thread
switcher has three primitives which together take 27 assembly
instructions. The system is driven by a background polling loop. There
is a 1 ms timer tick which simply increments a counter. There is one
thread to drive lwIP, and another which implemts a simple command line
processor with a few debug commands, plus commands for setting things
like the MAC ID and IP address..

This port takes 56K of flash and 37K of RAM. With some tuning of lwIP's buffer
pool the RAM requirements can be less than 32K.

The CIP application sends outputs to the LEDs on the SmartFusion eval board.

The SmartFusion evaluation board costs around $100 from Mouser or Arrow.
The FPGA and software development tools are free downloads from the
Actel website. The software development suite is a special edition of
Sourcery G++.


==================
UNPACK AND INSTALL
==================

After downloading and unpacking the opENer distribution, unzip the file
HW_SmartFusion_demo_verilog.zip, found in src/ports, in place. It will
add the contents of the Libero hardware project to the HW_SmartFusion_demo_verilog
directory. Initially this directory contains only the SmartFusion chip
driver library. If you have WinZip or WinRar, simply right click the zip
file and select "Extract to here".

You will need to download and install the latest version of Libero (the FPGA
tools) and SoftConsole (the software tools) from the Actel website.


====================
HARDWARE PROGRAMMING
====================

Start Libero. Open the hardware project from the
HW_SmartFusion_demo_verilog directory. This hardware project is the the
same as Actel's uIP webserver demo, except more GPIOs were added to
drive the LEDs and user I/O points (GPIO 10-15). The LEDs will display
the outputs from the Logix controller, and the user I/Os are used for
real-time debug. GPIO 10 is driven by the background polling loop to
indicate CPU run/idle time.

The following instructions will rebuild and flash the entire project:

Open Top_Webserver_Demo. This is the Cortex-M3 MSS. Clicl on the MSS to
open the MSS editor. Click Design->Generate. The error message "Error:
Parameter 'MAC_OTHER_CONNECTION_FAB_IS_USED' not found in the spirit
definition of instance 'MSS_FIO_0'" can be ignored. Close the MSS
editor.

Right click in the whitespace at the top level of the design and select
"Generate Design".

In the Project Flow view click Synthesis. This will open Synplify Pro.
Click Run. When it completes close Synplify.

In Project Flow click Place & Route. "Designer" will start. There will
be several popups throughout this process. Click OK for each one:

Audit Status -- OK.
Import Source Files -- OK.
SDC Import Options -- OK.
Click "Compile".
Compile Options -- OK.
When the compile completes click "Layout".
Layout Options -- OK.
When Layout completes click "FlashPro Data File".
Warning: TOP_Webserver_Demo.fdb already exists, replace it? -- Yes.
When this step completes, close Designer.
Designer: Save changes to TOP_Webserver_Demo.adb? -- Yes.

In Project Flow click Programming FlashPro. This starts the FlashPro
programming tool. Move JP10 on the eval board to the "FPGA" position.
Click "Program". When FlashPro reports "RUN PASSED" close it.

Warning: project TOP_Webserver_Demo has been modified, save it? -- Yes.

Move JP10 on the eval board to the "M3" position.

Close Libero. Hardware programming is complete.


========================
SOFTWARE BUILD AND FLASH
========================

The software uses a Makefile project. You can build the software from a
shell under Linux or Windows, with or without Cygwin. If you use Cygwin
the Makefile's display will be a bit cleaner, and it will report the size
of .text, .data, and .bss. You can also build the software from within
SoftConsole. If you want the SoftConsole build to use Cygwin, make sure
that "c:\cygwin\bin" is added to the Project's PATH.

You will have to use SoftConsole to program and debug the firmware.

You may have to modify the Makefiles in;

src/ports/platform-smartfusion.
src/ports/MyLib
src/ports/sprintf

If SoftConsole's bin file is already on the path, uncomment the line:
"CROSS=arm-none-eabi-". Otherwise uncomment and modify the line:
"CROSS=c:/cross/softconsole-v3.2/sourcery-g++/bin/arm-none-eabi-".


Build from a shell window
-------------------------

"cd" to src/ports/platform-smartfusion.
Type "make realclean" or "make clean" if needed.
"Realclean" cleans myLib and sprintf as well as platform-smartfusion.
Type "make"

OR

Build from SoftConsole
----------------------

Start SoftConsole.
Set the workspace to C:\tmp\opener-Actel\src\ports.
File->New->C Project
Project Name: type in platform-smartfusion
(ignore the warning: "Directory with specified name already exists.")
Makefile Project -> Empty Project
Finish
Project->Build All
(click on Console to watch what's happening)

Run debug configuration "platform-smartfusion". This will program the firmware
into the chip.

Start a terminal window for the serial/USB console. You may have to figure out
which port is being used using Device Manager. The serial configuration is
115200-N-8-1.


====================
RUNNING THE SOFTWARE
====================

Run the firmware. (Run->Resume). The OLED will display "opENer". The serial console
will display "hello, world!" followed by other information.

FIXME -- my board will not reliably start the firmware from powerup or reset.
I have to use the debugger.

The first time you powerup you will need to configure the MAC ID and IP info using
the serial console. Type ? for a help display. You can choose to get an IP address
automatically using DHCP, or enter a static IP address, netmask, and gateway. For
static IP information, use hte i, m, and g commands to enter the parameters, then
use the st command to save the parameters to flash.

FIXME -- Ideally, when changing parameters, the board should switch to using the new
info automatically. For now, you may have to restart the board.


Add a generic Ethernet I/O device with the following connection parameters:

Input assembly instance:         769 size: 32
Output assembly instance:        770 size: 32
Configuration assembly instance: 771 size: 10

Set address/host name to the IP address displayed on the OLED screen.

Assign a module name.

Set the RPI to something reasonable, like 100 ms.

Program the PLC ladder program to increment a counter four times a second and move
the accumulator to first data word of the module's output array. The low byte of the
counter will be displayed on the top line of the OLED and in the LEDs.

Implementing inputs is left as an exercise for the reader.










