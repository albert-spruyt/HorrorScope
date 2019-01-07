#!/usr/bin/python -u
import sys,re,random
import matplotlib.pyplot as plt

#Attempt:  1416 TimeSpent: 0:00:00.095829 length: 1 delay: 13842 TARGET RESPONDED:62cefc779d6666271f67490f7da01f4c ascii: b..w.ff'.gI.}..L

#Note these are the keys from the dict, not just the axis name
# 'attempt' 'length' 'wait' 'color'
Xlabel = 'wait'
Ylabel = 'length'
normal = "b7110b150e67ee0bae6a7bcf44493f1f"
        
#Number of bytes in which normal and changed differ
def diffBytes(normal,changed):
    return sum( [ x!=y for x,y in zip(changed,normal) ] )

# Parse lines into a list of dicts
def parseLines(lines):
    attempts = []
    for l in lines:
        if "Attempt" not in l:  # Attempt is an actual response line
            continue

        m = re.match("Attempt:  ([0-9]+).*length: ([0-9]+) delay: ([0-9]+) pulses: ([0-9]+)",l)
        if not m:
            print "Failed to parse params on m: ",l
            continue

        currentAttempt = {
                    'attempt': int(m.group(1)) + random.uniform(-0.1,0.1),
                    'length' : int(m.group(2)) + random.uniform(-0.1,0.1),
                    'wait' :   int(m.group(3)) + random.uniform(-0.1,0.1),
                    'pulses':  int(m.group(4)) + random.uniform(-0.1,0.1),
        }

        if 'Correct' in l:
            currentAttempt['color'] = 'g' # too hard
        elif 'TARGET CORRUPTED' in l: # we have changed output
            m = re.match(".*TARGET .*: b'(.*)' ascii.*",l) 
            if not m:
                print "Failed to match target response: ",l
                continue 

            data = m.group(1)
            diff = diffBytes(normal.decode('hex'),data.decode('hex'))

            if diff == 4:
                currentAttempt['color'] = 'r' # just right
            elif diff < 4: 
                currentAttempt['color'] = 'b' # too late
            elif diff > 4: 
                currentAttempt['color'] = 'k' # too early
        elif 'TARGET' in l:
                currentAttempt['color'] = 'm' # too early
        else: #normal
            currentAttempt['color'] = 'y'

        attempts.append(currentAttempt)

    return attempts


if len(sys.argv) < 2:
    print "USAGE: <LOGFILE>"
    sys.exit(-1)

logFile = sys.argv[1] 

with open(logFile) as f:
    lines = f.readlines()

attempts = parseLines(lines)

legend = {
    'g' : 'No change',
    'y' : 'No response',
    'm' : 'more data',
    'k' : '>4 byte diff',
    'b' : '<4 byte diff',
    'r' : '4 byte diff',
        }

for color in ['g','y','m','k','b','r']:
    colorAttempts = [ x for x in attempts if x['color'] == color]

    X = [ x[Xlabel] for x in colorAttempts ] 
    Y = [ x[Ylabel] for x in colorAttempts ] 

    plt.scatter(X,Y,color=color,marker='o',label=legend[color])

    print color, len(colorAttempts)

plt.xlabel(Xlabel)
plt.ylabel(Ylabel)
plt.legend()
plt.tight_layout()
plt.show()
