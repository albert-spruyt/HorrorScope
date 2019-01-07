#!/usr/bin/python -u
import sys,re

#Attempt:  1416 TimeSpent: 0:00:00.095829 length: 1 delay: 13842 TARGET RESPONDED:62cefc779d6666271f67490f7da01f4c ascii: b..w.ff'.gI.}..L

#Number of bytes in which normal and changed differ
def diffBytes(normal,changed):
    return sum( [ x!=y for x,y in zip(changed,normal) ] )

#return a string where changed bytes are marked by x and unchanged by .
def diffX(normal,changed):
    return ''.join( 'x' if x!=y else '.' for x,y in zip(changed,normal)) 

if len(sys.argv) < 2:
    print 'USAGE: ./parseLog.py <LOGFILE>'
    print ''
    print 'Small script to parse a fault log'
    print 'Can be used in a pipeline, some ideas:'
    print 'Example 1) Show when certain faults occur:'
    print './parseLog.py | sort -n'
    print 'Example 2) Give all the faulty outputs so that they can be given to a DFA program. Note this keeps duplicates'
    print '''./parseLog.py good.log | grep ' 4$' | awk '{print "aaaabbbbccccddddeeeeffff00001111 " $3}' > tracefile ; ./JeanGrey-master/dfa.py'''
    print "You can then add the correct response on top and run dfa.py"
    print ' which should give you something like the following:'
    print '                 Last round key #N found:'
    print '                 3B9F74AF9BBE1CDFD769BCE35AC4BBB2'
    print ' This can be fed to Stark'
    print '                 Round key bytes recovered:'
    print '                 3B9F74AF9BBE1CDFD769BCE35AC4BBB2'
    print ''
    print 'Stark-master/aes_keyschedule  3B9F74AF9BBE1CDFD769BCE35AC4BBB2 10'
    print ''

    sys.exit(1)

normal = "b7110b150e67ee0bae6a7bcf44493f1f".decode('hex')

with open(sys.argv[1]) as f:
    lines = f.readlines()

# the four makes the oneliners easy :s
print '000 Normal:',normal.encode('hex'),'4'
for l in lines:
    if "TARGET CORRUPTED" not in l: # not a glitched response
        continue

    m = re.match(".*delay: ([0-9]+) pulses: ([0-9]+) TARGET CORRUPTED: b'(.*)' ascii.*",l) 
    if not m:
        print "Failed to match m",l
        break

    wait =  int(m.group(1))
    data = m.group(3)
    dataBytes = data.decode('hex')
    print wait,'wait',data,diffX(normal,dataBytes),diffBytes(normal,dataBytes)

