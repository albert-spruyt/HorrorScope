#!/usr/bin/env python
import os,sys
from Trace import TraceSet,Trace 
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
    print "USAGE: %s <TRS IN>"%(sys.argv[0])
    sys.exit(-1)

inputFile = sys.argv[1]

ts = TraceSet()
ts.open(inputFile)
print "there are no", ts.NumberOfTraces,"traces"
prevSamples = True
count=0
for trace in ts:
    samples = trace._samples.astype(float)

    print "min: ", min(samples), "max: ", max(samples)

    plt.xlabel("Time")
    plt.tight_layout()
    if count > 0:
        plt.plot(prevSamples,color='green')
    plt.plot(samples)
    prevSamples = samples
    count+=1
    plt.show()
    

