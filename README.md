automaton
=========
`automaton` is a trip computer for your car. Utilizing breakthrough 1990s
technology, it will enable instantaeous visibility into various aspects of your
car's performance, such as engine data, fuel efficiency estimates, and speed
statistics. In addition, state-of-the-art SD card technology is now enabling the
sort of data logging and analysis that bored drivers of the past could only
dream about. `automaton` will undoubtably provide multiple hours of fun to
offset the innumerable hours its users spend driving.

what is it
----------
**Hardware** currently consists of a professionally fabricated PCB, mounted in a
sturdy project box with an RGB-backlit 96x64 graphics LCD for output and a
series of pushbuttons for input. IO connections on the board include **OBD-II**
(of course), **MicroSD** (for data logging), **MiniUSB** (for debug logging),
**JTAG** (for programming/debugging), and **PDI** (also for
programming/debugging).

**Firmware** consists of the AVR Xmega firmware program written in C that
operates the primary microcontroller.

**Software** will consist of PC-based Python scripts that can analyze logged
data to produce graphs and other statistical eye candy.

project status
--------------
**Hardware** is in its third version: first came the breadboard version that
used an AVR ATmega and the STN1110 OBD chip; second came a largely similar, but
more permanent, iteration built on perfboard; and now, the hardware is on a PCB
as described in the previous section. Hardware is essentially finalized, but a
second revision of the PCB may be produced if enough errata are found in the
first.

**Firmware** is where development is currently taking place. This is where most
of the work will be done, and a great deal of code needs to be ported over from
the older version.

**Software** is currently vaporware. Once the firmware is producing data, then
the software can be developed to analyze it.

supported vehicles
------------------
Vehicles must support OBD-II. Any vehicle made after 1996 is required by law to
implement OBD-II. However, the geniuses behind this standard decided that no
less than a half-dozen different physical layer implementations needed to be
developed to transmit the exact same information.

This means that vehicle support is limited by the type of bus in use. Vehicles
made after 2008 are required to support CAN (ISO 15765), so that is what the
current version of `automaton` supports. Future versions could feasibly support
more bus standards, pending developer motivation.

planned features
----------------

- Live data via LCD
- Deferred data analysis via micro SD card and Python scripts
- GPS waypoint logging
- Check Engine Light zapper
- Undocumented OBD-II PID sweeper

hardware and chips
------------------

Hardware is subject to change, but it's pretty locked down at this point.

- Microcontroller: Atmel ATxmega128A3U @ 32MHz (128KB flash; 8KB SRAM)
- CAN transceiver: Microchip MCP2562
- CAN controller: Microchip MCP2515
- Real-time clock: Maxim DS1390U
- GPS module: GlobalTop FGPMMOPA6H
- Debug UART: FTDI FT231XS
- LCD: 96x64 ST7565 graphic RGB-backlit LCD

compiling
---------
This project uses Avery Pennarun's implementation of D. J. Bernstein's `redo`
build system.

To build the `automaton` AVR firmware program, install `redo` and simply run
`redo all`. To clean the project directory, run `redo clean`.

To load the firmware onto the microcontroller, obtain a hardware programmer and
run `redo load`. You will almost certainly need to modify the build script for
this to work.

The build script may need to be modified for your system configuration. To do
so, you need only edit the `default.do` file in the project's root directory. As
`redo` uses posix shell scripts, a basic understanding of shell scripting is
sufficient to modify the build script to your purposes.

license
-------
This project is licensed under the terms of the simplified (2-clause) BSD
license. For more information, see the `LICENSE` file contained in the project's
root directory.
