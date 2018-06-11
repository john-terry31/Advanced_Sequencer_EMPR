import serial
import struct
import AdvancedSequencer as AdvSeq

comList = ["sendSequence", "sendSequenceSimple", "saveSequence", "savePacket", "editPacket"]
separator = "."
mbed = serial.Serial('/dev/ttyACM0', 9600, timeout=2)  # This is the linux port name
conPattern = [AdvSeq.getConPattern()]
_savePacketCode = 10
_saveOutputPacketCode = 11
_saveLoopPacketCode = 12
_saveSequenceCode = 20
_saveOutputSequenceCode = 21

'''
Type: Specifies command
10 - Save packet
11 - Send packet
12 - Send packet loop
20 - Save sequence
21 - Output sequence

Sequence type ideas: 
Single packet cycle    ## DO INSIDE C FILE
Fade                   ## Go towards 0, then back up to next value
Gradual Transition     ## Divide difference into chunks and send periodically
Flashing gradual       ## Flash between off and the transition chunks
'''
# TODO : Document methods etc


def sendInitSignal():
    initSequenceArray = [AdvSeq.getInitPattern()]
    #sendSerial(initSequenceArray)
    mbed.write('e')
    print('Sent\n')

    # Wait for length of init data
    print('Read')
    packetLen = mbed.read(3)
    print('PacketLen: ' + packetLen)
    #print('UnpackLen: ' + struct.unpack("B", packetLen))

    packetLen = chr(int(packetLen))
    sendSerial(packetLen)
    packetData = mbed.read(ord(packetLen))
    sendSerial(conPattern)
    sequenceLen = mbed.read(1)
    sendSerial(sequenceLen)
    sequenceData = mbed.read(sequenceLen)

    decodeInitData(packetData, sequenceData)


def decodePackets(packetString):
    if len(packetString) != (AdvSeq.getObjectNum() * AdvSeq.getPacketLength()):
        print 'Invalid packet init data - booting with default'
        # TODO : Some default stuff
        return

    for i, c in enumerate(packetString):
        AdvSeq.setPacketSlot(pNo=i // AdvSeq.getPacketLength(), slot=i % AdvSeq.getPacketLength(), newVal=c)
    AdvSeq.printPackets()


def decodeSequences(seqString):
    sequence = 0
    section = []
    lastWasSeparator = False
    for c in seqString:
        if c == separator and not lastWasSeparator:
            AdvSeq.appendSequenceSection(sNo=sequence, section=section)
            section += []
        elif c == separator and lastWasSeparator:
            sequence += 1
        else:
            section.append(c)


def decodeInitData(packets, sequences):   # Need to wipe packet and sequence arrays first
    decodePackets(packets)
    decodeSequences(sequences)


def startSequencer():
    print("JT's Advanced Sequencer\n")
    commandList(comList)
    sendInitSignal()


def sendSerial(messageArray):
    for i in messageArray:
        #byte = struct.pack(">B", i)
        #print(byte)
        mbed.write(str(i))


def sendMessage(messageType, stringArray):
    sendLength = len(stringArray)
    lenTypeArray = [sendLength, messageType]

    # Send main info
    sendSerial(lenTypeArray)
    # Wait for response
    read = mbed.read(1)

    # Send message if correct response received
    if read == conPattern:
        sendSerial(stringArray)


def checkInputs():
    """
    Some stuff in here
    :return:
    """


def commandList(lst):
    """
    Helper function. Display list of commands to terminal.
    :return:
    """
    print("Command list: \n")
    for i in range(len(lst)):
        print(str(i) + "." + lst[i] + ", ")
    print("\nFor help use 'commandHelp(comNumber)'")


def commandHelp(com):
    """
    Calls the help function for the associated command.
    :param com:
    :return:
    """
    if isinstance(com, int):
        comName = commandList[com]
    else:
        comName = com

    helpCommand = comName + "Help"  # Not sure about this bit, giving warnings, might be a problem
    globals()[helpCommand]()


def makePacketArr(packetNo, valuesLst):
    packetArr = [packetNo]
    for v in valuesLst:
        packetArr.append(v)
    return packetArr


def savePacket(packetNo, valuesLst):
    sendMessage(_savePacketCode, makePacketArr(packetNo, valuesLst))


def outputPacket(packetNo, valuesLst):
    sendMessage(_saveOutputPacketCode, makePacketArr(packetNo, valuesLst))


def loopPacket(packetNo, valuesLst):
    sendMessage(_saveLoopPacketCode, makePacketArr(packetNo, valuesLst))


def makeSeqArr(seqNo):
    seqLength = AdvSeq.getSequenceLength()
    seqLst = AdvSeq.getSequence(seqNo)
    # Add first subsection (main pattern and repeats)
    sequenceArr = [seqLst[0][0], seqLst[0][1], separator]
    # Append the main sequence
    for v in range(seqLength):
        sequenceArr.append(seqLst[1][v])

    if len(seqLst) > 2:
        # Go through each extra section
        for section in range(len(seqLst)-2):
            sequenceArr.append(separator)
            # Subsections each have 4 values
            for sVal in range(4):
                sequenceArr.append(seqLst[section+2][sVal])

    return sequenceArr


def saveSequence(seqNo):
    sendMessage(_saveSequenceCode, makeSeqArr(seqNo))


def outputSequence(seqNo):
    sendMessage(_saveOutputSequenceCode, makeSeqArr(seqNo))


