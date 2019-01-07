#!/usr/bin/python -u
import serial,os,sys,datetime,time,traceback

# If you see "invalid command" messages, switch these
target= serial.Serial(port='/dev/ttyACM0',baudrate=115200,timeout=0.5)

samples = 1024 * 3
target.write('n' + chr(samples>>8) + chr(samples &0xff) )
print target.read(10)

prevResponse=None
for i in range(1000):
        target.write('s')
        received =target.read(samples)
        if prevResponse != None and prevResponse not in received:
            print "\nDidnt received correct response as previous time"
        prevResponse = received
        print '\r',i,
        if len(received)!= samples:
            print("\nDidnt receive the correct amount of samples",len(received))
print ''
