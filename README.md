


# HorrorScope / ScaCheapier

This repository contains all the firmware source, scripts and notebooks for the ScaCheapier workshop given at 35c3. 

We gave a talk about this work at Troopers 2019 NGI, with a different perspective. You can find the slides in the presentation directory.

## Abstract 
From: https://events.ccc.de/congress/2018/wiki/index.php/Session:ScaCheapier

Want to recover cryptographic keys by just looking at a chip's power usage, for the price of a few beers?

In this workshop we will show you how to break AES with just ~5 euro of equipment (plus your laptop and a bit of code).

Following in the tradition of the ChipWhisperer, but with even cheaper hardware, come and join the world of hardware side channel attacks. With a simple custom PCB (schematics provided), a cheap off-the-shelf MCU and a handful of supporting components, you'll have no problem stealing the keys. 

## Presentation

During the workshop we partially re-used a presentation created by Ilya Kizhvatov. The original slides can be found here:  
https://www.slideshare.net/phdays/ss-35302416  
A recording of Ilya himself giving the talk can be found here: https://vimeo.com/100108647  

If you saw our talk at Troopers, you missed out on all the theory; you can watch Ilya's presentation to get it!

## Code repository

The code in the repository, in particular the FI and Scope FW parts, should be considered PoC code. In fact, don't take anything here too seriously. Have fun!

The repository is organized as follows:

Scope   - Scope and Glitcher Firmware (firmware in lufa\*/Projects/HorrorScope  
Acq     - Python acquisition scripts including the Scope GUI  
FI      - Python FI scripts/ including plotting  
julia   - The julia jupyter notebook  
presentation - The presentation for the workshop (and the troopers talk) 
PCB     - PCB files  

#### Patched LUFA
The Scope folder contains an entire copy of LUFA http://fourwalledcubicle.com/LUFA.php  

To maximize the amount of SRAM available as sample memory LUFA has been patched to minimize the number and size of USB buffers.   

The patch has been applied in Drivers/USB/Core/XMEGA/Endpoint_XMEGA.h  

## Installation


