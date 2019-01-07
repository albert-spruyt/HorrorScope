#!/usr/bin/python3 -u
import serial,os,sys,datetime,time,traceback
from Trace import TraceSet,Trace 
import numpy as np
from scope import Scope,hexAscii

#
# Acquisition parameters
#

numberOfAttempts =10000
numberOfRepeatedInputs =10
numSamples = 2500 # max: 1024 * 3 + 512

# Configure scope
scope = Scope('/dev/ttyACM0',
        timeout=0.5,
        sampleSpeed=1,
        bits12=True,
        bias=0x444,
        gain= 0,
        delay = 3000,
        #delay = 1,
        numSamples=numSamples
        )

titleLen = 0
dataLen = 32 # input + output of AES
# End parameters

# Redirect stdout to stderr as it is not buffered
sys.stdout = sys.stderr

if len(sys.argv) != 2:
    print(("USAGE: %s <TRS OUT>"%(sys.argv[0])))
    sys.exit(-1)

outputFile = sys.argv[1]


target= serial.Serial(port='/dev/ttyUSB0',baudrate=115200,timeout=0.5)

def testTarget(target):
    print("Testing target, too many dots? Reset Target")
    while True:
        print(".", end=' ')
        target.write(b't')
        if target.read(100) == b'T\r\n':
            break
    print('')

testTarget(target)


# Open a Trace set and declare the dimensions
ts = TraceSet()
if scope.bits12:
    ts.new(outputFile,titleLen,TraceSet.CodingFloat,dataLen,int(numSamples/2),[0,0])
else:
    ts.new(outputFile,titleLen,TraceSet.CodingFloat,dataLen,int(numSamples),[0,0])

timeStamp = datetime.datetime.now()
for attempt in range(numberOfAttempts):
    dataIn = None
    while True:
        dataIn = os.urandom(16) # create random data to encrypt
        if b'\n' not in dataIn:
            break
    
    print("\nAttempt: ",attempt, "TimeSpent:", datetime.datetime.now() - timeStamp)
    timeStamp = datetime.datetime.now()

    for repeatedInput in range(numberOfRepeatedInputs):
        try:
            print(repeatedInput, end=' ')
                    
            target.write(b'a'+ dataIn)  # Ask for encrypt, but don't send the last char just yet, we want to use it as a trigger
            time.sleep(0.004)

            scope.arm() # This blocks until the scope is ready for the trigger

            target.write(b'\n') # Now that scope is armed we send the newline, i guess we're triggering on serial

            response = target.read(16) # Get the encrypted data
            if len(response) != 16:
                raise Exception("Timeout in target response: "+ response.hex() )

            print(dataIn.hex(), end=' ')
            print(response.hex(), hexAscii(response), end=' ')
            print('\n', end=' ')

            if scope.waitForCompletion() == False: 
                print("Missed the trigger, let's try again")
                time.sleep(0.01)
                continue # missed the trigger try again
            samples = scope.getSamples()

            #data = list(map(ord,dataIn+response))
            data = dataIn+response

            ts.addTrace(Trace(b'',data,samples )) # save the trace
        except Exception as e:
            print(e)
            print(traceback.format_exc()) 

            #try to re-sync target and scope
            scope.reconnect()
            testTarget(target)

scope.close()
