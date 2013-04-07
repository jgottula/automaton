automaton
=========
`automaton` is a trip computer for your car. Utilizing breakthrough 1990s
technology, it will enable instantaeous visibility into various aspects of your
car's performance, such as engine data, fuel efficiency estimates, and speed
statistics. In addition, state-of-the-art SD card technology is now enabling the
sort of data logging and analysis that bored drivers of the past could only
dream about. `automaton` will undoubtably provide multiple hours of fun to
offset the innumerable hours its users spend driving.

status
------
The AVR firmware is under heavy development. The hardware design is still
preliminary. The PC support programs for data analysis have not been written.

The current implementation is on a breadboard. Once the hardware design is
solidified, a PCB version will probably be manufactured.

supported vehicles
------------------
Vehicles must support OBD-II. Any vehicle made after 1996 is required by law to
implement OBD-II. Only certain protocols are actually implemented in hardware.
This limitation has not been made because of any fundamental limitation, but
rather, to reduce the mountain of hardware needed to support the many mutually
incompatible protocols under the OBD-II umbrella.

The STN1110 chip is a universal transceiver, so the addition of support for more
OBD-II protocols is limited only by the developer's patience.

Supported protocols:

- SAE J1850 VPW
- CAN (ISO 15765)

Unsupported protocols:

- SAE J1850 PWM
- ISO 9141-2
- KWP (ISO 14230)

features
--------

Most of these are not yet done.

- Live data via LCD
- Deferred data analysis via SD card and Python scripts
- GPS logging
- Check Engine Light zapper

hardware
--------

All hardware is subject to change as project requirements shift.

- Microcontroller: Atmel ATmega324A @ 20MHz (32KB flash; 2KB SRAM)
- OBD transceiver: OBD Solutions STN1110
- Real-time clock: TBD
- GPS module: TBD
- Debug UART: FTDI FT230X (TBD)
- LCD: 4x20 HD44780

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
