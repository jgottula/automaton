#!/bin/sh

avr-gdb -ex "target remote :14141" out/automaton.elf
