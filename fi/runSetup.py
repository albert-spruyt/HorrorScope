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
maxGlitch = 100 # Currently set to max 3
maxMutesInARow = 1 # If we get a lot of mutes in a row we need to reset the target for a long time
minPulses=1
maxPulses=1

#
## End Glitch parameters
#

# If you see "invalid command" messages, switch these
glitcher = Scope('/dev/ttyACM0', timeout=0.5 )

target= serial.Serial(port='/dev/ttyUSB0',baudrate=115200,timeout=0.15)

mutesInARow =0

def testTarget(target):
    print("Testing target")
    resetTarget(target)
    while True:
        print(".",end=' ')
        target.write(b't')
        if target.read(100) == b'T\r\n':
            break

def resetTarget(target):
    glitcher.powerOff() # Turn the power off
    time.sleep(0.10)
    glitcher.powerOn() # Turn the power on
    time.sleep(0.10)

    response =  target.read(100) 
    if b'arduino' in response:
        print("Target came back up")
    else:
        print("[T] Target failed to come back up", hexAscii(response))

testTarget(target)


# First we perform an encryption without glitching to see what a normal response looks like.
# We can compare this to a possibly glitched result
dataIn = b'B'*16 # data to encrypt
target.write(b'a' + dataIn + b'\n') # Ask for encrypt
expectedResponse = target.read(16) # Get the encrypted data
print("Expected response: ",binascii.hexlify(expectedResponse), 'ascii:', hexAscii(expectedResponse))

# Initialize stats
expected = 0
glitched = 0
mute = 0
corrupted = 0

timeStamp = datetime.datetime.now()
for attempt in range(numberOfAttempts):
    try:
        glitch_length = randint(minGlitch,maxGlitch)
        glitch_delay  = randint(minWait,maxWait)
        pulses  = randint(minPulses,maxPulses)

        print("Attempt: ",attempt, "TimeSpent:", datetime.datetime.now() - timeStamp, "length:",glitch_length,"delay:",glitch_delay,'pulses:',pulses,end=' ')

        timeStamp = datetime.datetime.now()

        glitcher.setDelay(glitch_delay)
        glitcher.setGlitchLength(glitch_length)
        glitcher.setPulses(pulses)

        target.write(b'a' + dataIn) # Ask for encrypt
        time.sleep(0.004)

        glitcher.glitchArm() # Arm the glitcher

        target.write(b'\n')

        response = target.read(100) # Get the encrypted data, get too much data, maybe the target reboots
        glitcherTimeout = glitcher.waitForGlitcher()

        if glitcherTimeout:
            print('[G] The glitcher timed out',end=' ')
            resetTarget(target)
        elif expectedResponse == response:
            print("Correct response!",end=' ')
            expected+=1
            mutesInARow=0
        elif len(response) < 16 or b'arduino' in response:
            print("mute!: respLen",len(response),end=' ')
            print(response)
            mute+=1
            mutesInARow+=1
        elif len(response) == 16:
            print("TARGET CORRUPTED:",binascii.hexlify(response), 'ascii:',hexAscii(response))
            corrupted+=1
            mutesInARow+=1
        else:
            print("TARGET RESPONDED:",binascii.hexlify(response), 'ascii:',hexAscii(response))
            glitched+=1
            mutesInARow+=1

        if mutesInARow > maxMutesInARow:
            print("\nToo many mutes in row resetting",end=' ')
            mutesInARow = 0
            resetTarget(target)
        print("##### expected:",expected,"glitched:",glitched,"corrupted",corrupted,"mute:",mute)

    except Exception as e:
        traceback.print_exception(e)
        glitcher._testScope()
        testTarget(target)

