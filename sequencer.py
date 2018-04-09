import serial
import AdvancedSequencer as AdvSeq

comList = ["sendSequence", "sendSequenceSimple", "saveSequence", "savePacket", "editPacket"]
conPattern = AdvSeq.getConPattern()
separator = "."
mbed = serial.Serial('/dev/ttyACM0', 9600)


'''
Type: Specifies command
10 - Create packet
20 - Edit packet
30 - Create sequence
40 - Edit sequence
50 - Send sequence simple
51 - Send sequence normal
52 - Send sequence fade
53 - Send sequence transition



Sequence type ideas: 
Single packet cycle    ## DO INSIDE C FILE
Fade                   ## Go towards 0, then back up to next value
Gradual Transition     ## Divide difference into chunks and send periodically
Flashing gradual       ## Flash between off and the transition chunks
'''


def sendInitSignal(signal):
    sendSerial(signal)

    # Wait for length of init data
    packetLen = mbed.read(1)

    sendSerial(packetLen)
    packetData = mbed.read(packetLen)

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
        AdvSeq.packets[i//AdvSeq.getPacketLength()].append(c)


def decodeSequences(seqString):
    sequence = 0
    section = []
    lastWasSeparator = False
    for c in seqString:
        if c == separator and not lastWasSeparator:
            AdvSeq.sequences[sequence].append(section)
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
    sendInitSignal(AdvSeq.getInitPattern())


def sendSerial(messageArray):
    for byte in messageArray:
        if isinstance(byte, str):
            mbed.write(byte)
        elif isinstance(byte, int):
            mbed.write(str(byte))
        else:
            print('Invalid type')


def sendMessage(messageType, stringArray):
    sendLength = len(stringArray)
    lenTypeArray = [sendLength, separator, messageType]

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


def sendSequence(seqNo, sequenceData):
    """
    :param comType:
    :param packets:
    :param repeats:
    :param sectionStart:
    :param sectionEnd:
    :param sectionRepeat:
    :return: string to send
    """
    message = ""
    sendSerial(message)


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


def sendPacket(packetNo, newValues):
    """
    Edit individual packets. Up to 10 slots per, for simplicity.
    Use '-' for leave unedited maybe?
    :param packetNo:
    :param newValues:
    :return:
    """

    # Get packet
    # Display current packet to user

    packetStr = str(packetNo) + str(newValues)
    return packetStr

    # Needs to get packet first??


def sendPacketLoop(packetNo, newValues):
    """
    Edit individual packets. Up to 10 slots per, for simplicity.
    Use '-' for leave unedited maybe?
    :param packetNo:
    :param newValues:
    :return:
    """

    # Get packet
    # Display current packet to user

    packetStr = str(packetNo) + str(newValues)
    return packetStr

    # Needs to get packet first??


def getPacket(packetNo):
    """

    :param packetNo:
    :return:
    """
    # Get the packet from the packets array


def savePacket(packetNo, values):
    """
    Overwrites the value in the current packet
    :param packetNo:
    :param values:
    :return:
    """

    packetStr = str(packetNo) + separator + str(values)
    # Notify mbed of packetSave
    # wait for response
    sendSerial(packetStr)


def savePacketHelp():
    print("packetNo - ID (0-9) of new packet. If taken, will overwrite.\n"
          "values:list - 10 slot values (0-255). If less given, will pad with 0s.")


def saveSequence(seqNo, sequenceData):
    """

    :param seqNo:
    :param sequenceData:
    :return:
    """


def saveSequenceHelp():
    print("comType:int - see comTypeHelp().\n"
          "packets:string - packets in the sequence.\n"
          "repeats:int - sequence repetitions.\n"
          "sectionStart:int - start of section to repeat, '-' for none.\n"
          "sectionEnd:int - end of section to repeat, '-' for none.\n"
          "sectionRepeat:int - repetitions of specified section.")


