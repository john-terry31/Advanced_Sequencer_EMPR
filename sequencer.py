import serial

comList = ["sendSequence", "sendSequenceSimple", "saveSequence", "createPacket", "editPacket"]

separator = "."
# mbed = serial.Serial('/dev/ttyACM0', 9600)


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


def startSequencer():
    print("JT's Advanced Sequencer\n")
    commandList(comList)


def sendSerial(message):
    mbed.write(message)


def concatPackets(mes, packets):
    pack = str(packets) + "-"*(10-len(str(packets)))
    mes = concatToMessage(mes, pack)
    return mes


def concatToMessage(mes, add):
    mes = mes + add + separator
    return mes


def checkInputs():
    """
    Some stuff in here
    :return:
    """


def sendSequenceSimple(comType, packets, repeats):
    """
    Call into send sequence with 0 for the last 3 arguments.
    :param comType: sequence type (int)
    :param packets: the packets to send (list)
    :param repeats: number of times to display sequence (int)
    :return: string to send
    """
    message = ""
    message = concatToMessage(message, str(comType))
    message = concatPackets(message, packets)
    message = concatToMessage(message, str(repeats))
    message = concatToMessage(message, "0")
    message = concatToMessage(message, "0")
    message = concatToMessage(message, "00")
    sendSerial(message)


''' Need to send extra data on how many sections/different transition patterns if adding that below'''


def sendSequence(comType, packets, repeats, sectionStart=0, sectionEnd=0, sectionRepeat=0):
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
    message = concatToMessage(message, str(comType))
    message = concatPackets(message, packets)
    message = concatToMessage(message, str(repeats))
    message = concatToMessage(message, str(sectionStart))
    message = concatToMessage(message, str(sectionEnd))
    message = concatToMessage(message, str(sectionRepeat))
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


def editPacket(packetNo, newValues):
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


def editPacketHelp():
    print("packetNo - ID (0-9) of the packet to edit.\n"
          "newValues - 10 new values (0-255), use '-' to leave the same.")


def createPacket(packetNo, values):
    """
    Overwrites the value in the current packet
    :param packetNo:
    :param values:
    :return:
    """

    packetStr = str(packetNo) + str(values)
    return packetStr


def createPacketHelp():
    print("packetNo - ID (0-9) of new packet. If taken, will overwrite.\n"
          "values:list - 10 slot values (0-255). If less given, will pad with 0s.")


def createSequence(comType, packets, repeats, sectionStart, sectionEnd, sectionRepeat):
    """
    Save a packet sequence to the
    :param comType:
    :param packets:
    :param repeats:
    :param sectionStart:
    :param sectionEnd:
    :param sectionRepeat:
    :return:
    """


def createSequenceHelp():
    print("comType:int - see comTypeHelp().\n"
          "packets:string - packets in the sequence.\n"
          "repeats:int - sequence repetitions.\n"
          "sectionStart:int - start of section to repeat, '-' for none.\n"
          "sectionEnd:int - end of section to repeat, '-' for none.\n"
          "sectionRepeat:int - repetitions of specified section.")
