import serial
from Tkinter import *
import gui
import sequencer

objectNum = 10
maxPacketVal = 255
packets = [
    [1, 0, 0, 25, 0, 102, 0, 0, 0, 0],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0]
]

sequences = [
    # [sequence][mainPattern&repeats, mainSequence, subSection1, ....][Part]
    [[0, 45], [1, 0, 0, 2, 0, 3, 0, 0, 0, 0], [1, 3, 14, 0]],  # Sequence 0
    [[0, 45], [1, 0, 0, 0, 0, 0, 0, 0, 0, 0]],      # Sequence 1
    [[0, 45], [1, 0, 0, 0, 0, 0, 0, 0, 0, 0]],      # Sequence 2
    [[0, 45], [1, 0, 0, 0, 0, 0, 0, 0, 0, 0]],      # .....
    [[0, 45], [1, 0, 0, 0, 0, 0, 0, 0, 0, 0]],
    [[0, 45], [1, 0, 0, 0, 0, 0, 0, 0, 0, 0]],
    [[0, 45], [1, 0, 0, 0, 0, 0, 0, 0, 0, 0]],
    [[0, 45], [1, 0, 0, 0, 0, 0, 0, 0, 0, 0]],
    [[0, 45], [1, 0, 0, 0, 0, 0, 0, 0, 0, 0]],
    [[0, 45], [1, 0, 0, 0, 0, 0, 0, 0, 0, 0]]
]

'''## Class defining packet objects ##
class Packet(object):

    def __init__(self, slots=None):
        ## Something about isInstance()
        if slots is None:
            tempList = [0 for i in range(512)]
        else:
            tempList = [0 for i in range(512)]
            for i in range(len(slots)):
                tempList[i] = slots[i]
        self._slots = tempList



## Class defining sequence objects ##
class Sequence(object):

    def __init__(self, packetStr, transitionType): ## Maybe add fade/other options here?
        self._packetStr = packetStr
        self._transitionType = transitionType

    def createSequence(self):
        seqStr = input('Enter sequence: ')     
'''

''' When initialising need to get packet data first before anything else. To be used when editing etc.
    Needs to create a packets array and sequences array from this data.
'''
sequencer.startSequencer()


root = Tk()
app = gui.NotebookDemo(root)
root.mainloop()

