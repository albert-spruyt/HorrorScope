#!/bin/bash
set -e

make -j$(nproc) >/dev/null
avr-size -C HorrorScope.elf
avrdude -p atxmega32a4u -cavrispmkii  -U flash:w:HorrorScope.hex:i -P usb
