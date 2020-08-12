import sys
from random import randrange

reference_string = []
page_frames = 0

#check command line
if len(sys.argv) != 2:
    print("Error, Usage: python3 main.py page_frames")
    sys.exit()

#get command line input
page_frames = int(sys.argv[1])

if page_frames < 1 or page_frames > 30:
    print("Error, Page frames must be within range of 1-30")
    sys.exit()

print("Page frames: ", page_frames)

#Generate reference string
for i in range(0,100) :
    reference_string.append(int((randrange(50))))

#------- First In, First Out (FIFO) --------#
frame = []
faults = 0
top = 0
#i = 1

for i in reference_string:
    if i not in frame:
        #page fault
        if len(frame)<page_frames:
            frame.append(i)
        else:
            frame[top] = i
            top = (top+1)%page_frames
        faults += 1

print("Total Page faults (FIFO): ",faults)

#------- Least Recently Used (LRU) ---------#

frame = []
stack = []
faults = 0
top = 0

for i in reference_string:
    if i not in frame:
        #page fault
        if len(frame)<page_frames:
            frame.append(i)
            stack.append(len(frame)-1)
        else:
            index = stack.pop(0)
            frame[index] = i
            stack.append(index)
        faults += 1
    else:
        stack.append(stack.pop(stack.index(frame.index(i))))

print("Total Page faults (LRU ): ", faults)
#------- Optimal (OPT) ---------------------#
frame = []
faults = 0
hits = [None for i in range(page_frames)]
i=1

while i < len(reference_string):
    if reference_string[i] not in frame:
        #page fault
        if len(frame)<page_frames:
            frame.append(reference_string[i])
        else:
            for j in range(len(frame)):
                if frame[j] not in reference_string[i+1:]:
                    frame[j] = reference_string[i]
                    break

                hits[j] = reference_string[i+1:].index(frame[j])
            else:
                frame[hits.index(max(hits))] = reference_string[i]
        faults += 1
    i+=1
print("Total Page faults (OPT ): ", faults)
