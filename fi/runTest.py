#!/usr/bin/python3 -u
import serial,os,sys,datetime,time,binascii,traceback
from random import randint
from scope import Scope,hexAscii

#
##  Glitch parameters
#

timeout = 60000 # number of loop iterations to run before timeout... 
numberOfAttempts = 100000
minWait = 000
maxWait = 64000 # max: 65535
minGlitch = 1 # number of glitcher clocks between power down/up (min 4
maxGlitch = 1 # Currently set to max 3
maxMutesInARow = 1 # If we get a lot of mutes in a row we need to reset the target for a long time

#
## End Glitch parameters
#

# If you see "invalid command" messages, switch these
glitcher = Scope('/dev/ttyACM0', timeout=0.5 )


for attempt in range(numberOfAttempts):
    try:
        glitch_length = randint(minGlitch,maxGlitch)
        glitch_delay  = randint(minWait,maxWait)

        glitcher.setDelay(glitch_delay)
        glitcher.setGlitchLength(glitch_length)

        glitcher.glitchArm() # Arm the glitcher
        glitcherTimeout = glitcher.waitForGlitcher()

        print(attempt,end=' ')

        if glitcherTimeout:
            print('@@@@@@@@@@@ The glitcher timed out @@@@@@@@@@@')

    except Exception as e:
        traceback.print_exception(e)
        glitcher._testScope()
        testTarget(target)

