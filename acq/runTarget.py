#!/usr/bin/python3 -u
import serial,os,sys,datetime,time,traceback
from Trace import TraceSet,Trace 
import numpy as np
from scope import Scope,hexAscii

#
# Acquisition parameters
#

numberOfRepeatedInputs =50

# End parameters

# Redirect stdout to stderr as it is not buffered
sys.stdout = sys.stderr

target= serial.Serial(port='/dev/ttyUSB0',baudrate=115200,timeout=0.5)

def testTarget(target):
    print("Testing target, too many dots? Reset Target")
    while True:
        print(".",end=" ")
        target.write(b't')
        if target.read(100) == b'T\r\n':
            break
    print('')

testTarget(target)

timeStamp = datetime.datetime.now()
while True:
    dataIn = None
    while True:
        dataIn = os.urandom(16) # create random data to encrypt
        if b'\n' not in dataIn:
            break
    
    print("\nTimeSpent:", datetime.datetime.now() - timeStamp)
    timeStamp = datetime.datetime.now()

    for repeatedInput in range(numberOfRepeatedInputs):
        try:
            print(repeatedInput,end=' ')
            target.write(b'a' + dataIn)  # Ask for encrypt, but don't send the last char just yet, we want to use it as a trigger
            time.sleep(0.03)
            target.write(b'\n') 

            response = target.read(16) # Get the encrypted data
            if len(response) != 16:
                raise Exception("Timeout in target response: "+ response.encode('hex'))

            print( dataIn.hex(), response.hex(), hexAscii(response),'\r',end='')

        except Exception as e:
            print(e)
            print(traceback.format_exc() )

            #try to re-sync target 
            testTarget(target)

