import serial
from Tkinter import *

_confirmationPattern = 202
_initSignal = 101
_packetLength = 10
_objectNum = 10
_sequenceLength = 10
_maxPacketVal = 255
_maxSeqRepeats = 99
_packets = [
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

_sequences = [
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
# TODO : Document methods etc


def printPackets():
    print(_packets)


def getPacket(pNo):
    return _packets[pNo]


def getPacketSlot(pNo, slot):
    return _packets[pNo][slot]


def setPacketSlot(pNo, slot, newVal):
    if 0 <= newVal <= 255:
        _packets[pNo][slot] = newVal
    else:
        _packets[pNo][slot] = 0
        print('Invalid slot value')


def getSequence(sNo):
    return _sequences[sNo]


def getSeqSection(sNo, sectionNo):
    return _sequences[sNo][sectionNo]


def getSeqEntry(sNo, sectionNo, objNo):
    return _sequences[sNo][sectionNo][objNo]


def appendSequenceSection(sNo, section):
    _sequences[sNo].append(section)


def removeSection(sNo, sectionNo):
    del _sequences[sNo][sectionNo]


def getMaxSeqRepeats():
    return _maxSeqRepeats


def getObjectNum():
    return _objectNum


def getMaxPacketVal():
    return _maxPacketVal


def getInitPattern():
    return _initSignal


def getConPattern():
    return _confirmationPattern


def getPacketLength():
    return _packetLength


def getSequenceLength():
    return _sequenceLength


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


def startSequencer():
    import sequencer
    sequencer.startSequencer()


def startGUI():
    import gui
    app = gui.NotebookDemo(root)


startSequencer()
root = Tk()
startGUI()
root.mainloop()

