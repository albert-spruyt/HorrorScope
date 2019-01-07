'''
Open/Write an INS raceSet

v0.8 05-01-2017 (Baris)
    * Overwriting trace file issue fixed

v0.7b 06-07-2016 (Nikita)
    * Using Numpy arrays to speed up trace writing DRAMATICALLY

v0.7a 25-11-2016 (Baris)
    * Added X and Y scales and labels to the trace file

v0.6 05-12-2014 (Job)
    * Added writing trace fileSize

v0.5 12-11-2013 (Ilya)
    * Fixed the format typo; a bit of cleanup

v0.4 12-11-2013 (Ilya)
    * Fixed the sample reading bug in general

v0.3 11-09-2013 (Erik)
    * Fixed sample reading bug in getTrace() as pointed out by Ilya

v0.2 5-3-2013 (Erik)
    * Fixed __iter__ method and removed the no longer necesarry getTraces() method
    * Moved header constants inside TraceSet class
    * Removed debug code
'''

import os
import sys
import struct
import numpy as np

class Trace():
    def __init__(self, title, data, samples):
        self._title = title
        self._data = data
        self._samples = samples

class TraceSet():
    #header definitions
    NumberOfTraces          = 0x41
    NumerOfSamplesPerTrace  = 0x42
    SampleCoding            = 0x43
    DataSpace               = 0x44
    TitleSpace              = 0x45
    Description             = 0x47    
    Xlabel                  = 0x49
    Ylabel                  = 0x4A
    Xscale                  = 0x4B
    Yscale                  = 0x4C
    TraceBlock              = 0x5F #Trace block, a flat memory space

    # enum for sampe coding
    CodingByte  = 0x01
    CodingShort = 0x02
    CodingInt   = 0x04
    CodingFloat = 0x14

    def __init__(self):
        self._handle = None
        self._traceBlockOffset = None
        self._numberOfTraces = None
        self._numberOfSamplesPerTrace = None
        self._sampleCoding = None
        self._sampleCodingByteSize = None
        self._titleSpace = 0
        self._dataSpace = 0
        self._fileName = None
        self._xlabel = 115
        self._ylabel = 86
        self._xscale = 0
        self._yscale = 0

        #properties
        self._sampleSpace = None
        self._traceSpace = None
        self._traceBlockSpace = 0

        self._iterIndex = 0

    def __iter__(self):
        for i in range(self._numberOfTraces):
            yield self.getTrace(i)

    def _readUINT8(self):
        return struct.unpack("B",self._handle.read(1))[0];

    def _readUINT16(self):
        return struct.unpack("H",self._handle.read(2))[0];

    def _readUINT32(self):
        return struct.unpack("I",self._handle.read(4))[0];

    def _readFLOAT(self):
        return struct.unpack("f",self._handle.read(4))[0];

    def _writeUINT8(self, val):
        #print "UINT8: %02x"%(val&0xFF)
        return self._handle.write(struct.pack("B",(val&0xFF)));

    def _writeUINT16(self, val):
        #print "UINT16: %04x"%(val&0xFFFF)
        return self._handle.write(struct.pack("H",(val&0xFFFF)));

    def _writeUINT32(self, val):
        #print "UINT32: %08x"%(val&0xFFFFFFFF)
        return self._handle.write(struct.pack("I",(val&0xFFFFFFFF)));

    def _writeFLOAT(self, val):
        return self._handle.write(struct.pack("f",val));

    def _writeTitleSpace(self, val):
        self._writeUINT8(self.TitleSpace)
        self._writeUINT8(1)
        self._writeUINT8(val)
        self._titleSpace = val

    def _writeNumberOfTraces(self, val):
        self._writeUINT8(self.NumberOfTraces)
        self._writeUINT8(4)
        self._writeUINT32(val)
        self._numberOfTraces = val

    def _updateNumberOfTraces(self, val):
        self.findtag(self.NumberOfTraces)
        self._writeUINT8(self.NumberOfTraces)
        self._writeUINT8(4)
        self._writeUINT32(val)
        self._numberOfTraces = val

    def _writeDataSpace(self, val):
        self._writeUINT8(self.DataSpace)
        self._writeUINT8(2)
        self._writeUINT16(val)
        self._dataSpace = val

    def _writeNumerOfSamplesPerTrace(self, val):
        self._writeUINT8(self.NumerOfSamplesPerTrace)
        self._writeUINT8(4)
        self._writeUINT32(val)
        self._numberOfSamplesPerTrace = val

    def _writeSampleCoding(self, val):
        self._writeUINT8(self.SampleCoding)
        self._writeUINT8(1)
        self._writeUINT8(val)
        self._sampleCoding = val

    def _writeLabelsXY(self):
        self._writeUINT8(self.Xlabel)
        self._writeUINT8(1)
        self._writeUINT8(self._xlabel)
        self._writeUINT8(self.Ylabel)
        self._writeUINT8(1)
        self._writeUINT8(self._ylabel)   

    def _writeScalesXY(self , xscale , yscale):   
        self._writeUINT8(self.Xscale)
        self._writeUINT8(4)
        self._writeFLOAT(xscale)
        self._writeUINT8(self.Yscale)
        self._writeUINT8(4)
        self._writeFLOAT(yscale)

    def _writeTraceBlock(self):
        self._writeUINT8(self.TraceBlock)
        self._writeUINT8(0)

    def open(self, fileName):
        self._handle = open(fileName,'rb')
        f = self._handle
        f.seek(0,2)

        fileSize = f.tell()
        f.seek(0)
        offset = 0

        while (offset < fileSize - self._traceBlockSpace):
            tag = ord(f.read(1))
            length = ord(f.read(1))
            addLen = 0

            if ((length & 0x80) != 0): #length is encoded in more then 1 byte
                addLen = length & 0x7F #how many byte the length is actually encoded in.
                length = 0
                for i in range(addLen):
                    length = length + (ord(f.read(1)) << (i * 8))

            if tag == self.TraceBlock:
                self._sampleSpace = self._numberOfSamplesPerTrace * self._sampleCodingByteSize
                self._traceSpace = self._sampleSpace + self._dataSpace + self._titleSpace
                self._traceBlockSpace = self._numberOfTraces * self._traceSpace

                self._traceBlockOffset = f.tell() #get current pos
                f.seek(self._traceBlockOffset + self._traceBlockSpace) # XXX: why this?
            elif tag == self.TitleSpace:
                self._titleSpace = self._readUINT8()
            elif tag == self.NumberOfTraces:
                self._numberOfTraces = self._readUINT32()
            elif tag == self.DataSpace:
                self._dataSpace = self._readUINT16()
            elif tag == self.NumerOfSamplesPerTrace:
                self._numberOfSamplesPerTrace = self._readUINT32()
            elif tag == self.SampleCoding:
                self._sampleCoding = self._readUINT8()
                #compensate for float sample coding tag
                if self._sampleCoding == self.CodingFloat: #float
                    self._sampleCodingByteSize = 4
                else:
                    self._sampleCodingByteSize = self._sampleCoding
            elif tag == 0 or length == 0:
                print ("Unknown tag: %x len: %d" % (tag, length)) # TODO: support other optional tags
                break
            else:
                print ("Unknown tag: %x len: %d" % (tag, length)) # TODO: support other optional tags
                f.read(length)

            print ("Offset",offset)
            print ("TraceBlockSpace: ",self._traceBlockSpace)
            print ("Tag: %x len: %d" % (tag, length)) # TODO: support other optional tags
            offset = offset + 2 + addLen + length

    def findtag(self,searchTag):
        if (self._handle == None):
            return 0
        f = self._handle
        f.seek(0,2)
        fileSize = f.tell()
        f.seek(0)
        offset = 0

        while (offset < fileSize - self._traceBlockSpace):
            tag = ord(f.read(1))
            length = ord(f.read(1))
            addLen = 0

            if ((length & 0x80) != 0): #length is encoded in more then 1 byte
                addLen = length & 0x7F #how many byte the length is actually encoded in.
                length = 0
                for i in range(addLen):
                    length = length + (ord(f.read(1)) << (i * 8))
            if (tag == searchTag):
                f.seek(offset)
                return 1
            f.read(length)
            offset = offset + 2 + addLen + length
        return 0

    def new(self, fileName, titleSpace, sampleCoding, dataSpace, numberOfSamples, XYscales):
        self._fileCounter = 1
        self._tempFileName = fileName
        while(os.path.isfile(self._tempFileName)):
            self._tempFileName = fileName[:-4] + ('(%d)' % self._fileCounter) + fileName[-4:] 
            self._fileCounter += 1
        self._fileName = self._tempFileName
        self._handle = open(self._tempFileName,'wb')
        self._handle.close()
        self._handle = open(self._tempFileName,'r+b')

        self._writeTitleSpace(titleSpace)
        self._writeSampleCoding(sampleCoding)
        if self._sampleCoding == self.CodingFloat: #float
            self._sampleCodingByteSize = 4
        else:
            self._sampleCodingByteSize = self._sampleCoding
        self._writeDataSpace(dataSpace)
        self._writeNumerOfSamplesPerTrace(numberOfSamples)
        self._writeNumberOfTraces(0)
        self._sampleSpace = self._numberOfSamplesPerTrace * self._sampleCodingByteSize
        self._traceSpace = self._sampleSpace + self._dataSpace + self._titleSpace
        self._traceBlockSpace = self._numberOfTraces * self._traceSpace
        self._writeLabelsXY()
        self._writeScalesXY(XYscales[0] , XYscales[1])
        self._writeTraceBlock()
        self._traceBlockOffset = self._handle.tell() #get current pos


    def getTrace(self, traceIndex):
        if (self._handle == None):
            return None
        f = self._handle
        f.seek(self._traceBlockOffset + traceIndex * self._traceSpace)
        
        title = f.read(self._titleSpace)
        data = map(ord,f.read(self._dataSpace))

        if self._sampleCoding == self.CodingByte: #byte
            fmt = 'int8'
        elif self._sampleCoding == self.CodingShort: #short
            fmt = 'int16'
        elif self._sampleCoding == self.CodingInt: # int
            fmt = 'int32'
        elif self._sampleCoding == self.CodingFloat: # float
            fmt = 'float32'
        else:
            raise "Incorrect coding"

        samples = np.fromfile(f, fmt, self._numberOfSamplesPerTrace)     

        '''
        samples = f.read(self._numberOfSamplesPerTrace * self._sampleCodingByteSize)

        if self._sampleCoding == self.CodingByte: #byte
            fmt = 'b'
            samples = map(lambda x:struct.unpack(fmt,x)[0], samples)
        else:
            if self._sampleCoding == self.CodingShort: #short
                fmt = 'h'
            elif self._sampleCoding == self.CodingInt: # int
                fmt = 'i'
            elif self._sampleCoding == self.CodingFloat: # float
                fmt = 'f'
            
            tmp = []
            for i in xrange(0,len(samples)/self._sampleCodingByteSize):
                index = i*self._sampleCodingByteSize
                #WARNING: does not keep endianess in mind, little endian by default (aka x86)
                tmp.append(struct.unpack(fmt,samples[index:index+self._sampleCodingByteSize])[0])
            samples = tmp
        '''
        return Trace(title, data, samples)

    def addTrace(self, trace):
        if (self._handle == None):
            return
        f = self._handle
        f.seek(0,2)

        # we only accept exact same size inputs
        if (len(trace._title) != self._titleSpace):
            print("Wrong title size! %08x/%08x"%(len(trace._title),self._titleSpace))
            return
        f.write(trace._title)
        if (len(trace._data) != self._dataSpace):
            print("Wrong data size!")
            return
        for i in trace._data:
            self._writeUINT8(i)
        if self._sampleCoding == self.CodingByte:
            trace._samples.astype('int8').tofile(self._handle)
#            for i in trace._samples:
#                self._writeUINT8(i)
        elif self._sampleCoding == self.CodingShort:
            trace._samples.astype('int16').tofile(self._handle)
#            for i in trace._samples:
#                self._writeUINT16(i)
        elif self._sampleCoding == self.CodingInt:
            trace._samples.astype('int32').tofile(self._handle)
#            for i in trace._samples:
#                self._writeUINT32(i)
        elif self._sampleCoding == self.CodingFloat:
            trace._samples.astype('float32').tofile(self._handle)
#            for i in trace._samples:
#                self._writeFLOAT(i)
        self._numberOfTraces+=1
        # update the trace count
        self._updateNumberOfTraces(self._numberOfTraces)
        # go to the end again
        f.seek(0,2)

    def close(self):
        self._handle.close()
