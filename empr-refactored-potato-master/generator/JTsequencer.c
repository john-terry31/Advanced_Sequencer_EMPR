//
// Created by jt1268 on 06/03/18.
//

#include "stdint.h"
#include "JTsequencer.h"
#include "LPC17xx.h"
#include "lpc17xx_uart.h"
#include "serial.h"
#include "lcd.h"
#include "EMPR.h"
#include "keypad.h"
#include "lpc_types.h"
#include "Generator.h"
#include "stdlib.h"
#include "vector.h"
#include "sequences.h"


#define IC_CHANNEL 0
#define PACKET_LENGTH 10
#define NO_OBJECTS 10
#define INIT_SIGNAL 101
#define SEQUENCE_LENGTH 10
#define CONFIRMATION_CODE 202
#define SECTION_ELEMENTS 4


// //////////////////////////////////////////////////////////////////////////////////////////////////////// //
char packetArray[NO_OBJECTS][PACKET_LENGTH] = {
        {10,146,241,72,65,72,003,8,255,255},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {}
};

char sendPackets[NO_OBJECTS*PACKET_LENGTH];
int sendPacketsNext = 0;

char basicSequenceArray[NO_OBJECTS][2][SEQUENCE_LENGTH] = {
        {{0,45},{1, 0, 0, 2, 0, 3, 0, 0, 0, 0}},
        {{0,45},{1, 0, 0, 2, 0, 3, 0, 0, 0, 0}},
        {{0,45},{1, 0, 0, 2, 0, 3, 0, 0, 0, 0}},
        {{0,45},{1, 0, 0, 2, 0, 3, 0, 0, 0, 0}},
        {{0,45},{1, 0, 0, 2, 0, 3, 0, 0, 0, 0}},
        {{0,45},{1, 0, 0, 2, 0, 3, 0, 0, 0, 0}},
        {{0,45},{1, 0, 0, 2, 0, 3, 0, 0, 0, 0}},
        {{0,45},{1, 0, 0, 2, 0, 3, 0, 0, 0, 0}},
        {{0,45},{1, 0, 0, 2, 0, 3, 0, 0, 0, 0}},
        {{0,45},{1, 0, 0, 2, 0, 3, 0, 0, 0, 0}}
};

char sendSequences[NO_OBJECTS*(2+SEQUENCE_LENGTH)];
int sendSequencesNext = 0;

void advancedSequencer(void)
{
    uint32_t len, idx;
    uint8_t initReceived, buffer[10], returnVal;
    // Sequence sequences[NO_OBJECTS];
    initReceived = readSerialByte();

    if (initReceived == INIT_SIGNAL)
    {
        fillSendPacketArray();
        write_usb_serial_blocking((char *)sendPacketsNext, strlen((char *)sendPacketsNext));
        returnVal = readSerialByte();
        write_usb_serial_blocking(sendPackets, sendPacketsNext-1);
        returnVal = readSerialByte();

        fillSendSequenceArray();
        write_usb_serial_blocking((char *)sendSequencesNext, strlen((char *)sendSequencesNext));
        returnVal = readSerialByte();
        write_usb_serial_blocking(sendSequences, sendSequencesNext-1);
        returnVal = readSerialByte();
    }

    while (1)
    {
        waitForInput();
    }
}

void sendConfCode(void)
{
    write_usb_serial_blocking((char *)CONFIRMATION_CODE, strlen((char *)CONFIRMATION_CODE));
}

void waitForInput(void)
{
    char lenType[2];
    read_usb_serial_none_blocking(lenType, 2);
    // wait for length
    sendConfCode();
    char data[(int) lenType[0]];
    read_usb_serial_none_blocking(data, (int) lenType[0]);
    switch ((int) lenType[1])
    {
        default:
            return;
        case 10:
            savePacket(data);
            break;
        case 11:
            savePacket(data);
            outputPacket(packetArray[(int) data[0]]);
            break;
        case 12:
            savePacket(data);
            outputSingleLoop((int) packetArray[(int) data[0]]);
            break;
        case 20:
            saveSequenceSimple(data);
            break;
        case 21:
            saveSequenceSimple(data);
            outputSequenceSimple((int) data[0]);
    }
}

uint8_t readSerialByte(void)
{
    return UART_ReveiveByte((LPC_UART_TypeDef *)LPC_UART0);
}

void fillSendPacketArray(void)
{
    int i, j;
    for (i=0; i<NO_OBJECTS; i++)
    {
        for (j=0; j<PACKET_LENGTH; j++)
        {
            sendPackets[sendPacketsNext] = packetArray[i][j];
            sendPacketsNext++;
        }
    }
}

void fillSendSequenceArray(void)
{
    int i, j;
    for (i=0; i<NO_OBJECTS; i++)
    {
        sendSequences[sendSequencesNext] = basicSequenceArray[i][0][0];
        sendSequencesNext++;
        sendSequences[sendSequencesNext] = basicSequenceArray[i][0][1];
        sendSequencesNext++;

        for (j = 0; j < SEQUENCE_LENGTH; j++)
        {
            sendSequences[sendSequencesNext] = basicSequenceArray[i][1][j];
            sendSequencesNext++;
        }
    }
}

void savePacket(const char * data)
{
    int i;
    int pNo = (int) data[0];
    for (i=1; i<PACKET_LENGTH; i++)
    {
        packetArray[pNo][i-1] = (int) data[i];
    }
}

void outputSingleLoop(int pNo)
{
    int i;
    for (i=0; i<PACKET_LENGTH; i++)
    {
        dmx_output_single(packetArray[pNo][i%PACKET_LENGTH], packetArray[pNo][(i+1)%PACKET_LENGTH],
                          packetArray[pNo][(i+2)%PACKET_LENGTH], IC_CHANNEL);
    }
}

saveSequenceSimple(char * data)
{
    int i, sNo = (int) data[0];
    basicSequenceArray[sNo][0][0] = data[1];
    basicSequenceArray[sNo][0][1] = data[2];
    for (i=3; i<SEQUENCE_LENGTH+3; i++)
    {
        basicSequenceArray[sNo][1][i-3] = data[i];
    }
}

//void saveSequence()
//{
//    // Save according to format
//}

void outputSequenceSimple(int sNo)
{
    outputSection(sNo, 0, SEQUENCE_LENGTH-1, basicSequenceArray[sNo][0][0], basicSequenceArray[sNo][0][1]);
}

//void outputSequence(int sNo)
//{
//    // Work out order of sections.
//    // for section in sequence:
//        //outputSection(sNo, sectionStart, etc)
//
//}

int getSlot(int p, int slot)
{
    int s = atoi((const char *) packetArray[p][slot]);
    return s;
}

void outputSection(int sNo, int sectionStartIndex, int sectionEndIndex, int pattern, int repeats)
{
    int i, j;
    switch(pattern)
    {
        case 0:
            outputSNormal(sNo, sectionStartIndex, sectionEndIndex, repeats);
        case 1:
            for (i=0; i<= repeats; i++)
            {
                for (j=0; j<=abs(sectionStartIndex-sectionEndIndex); j++)
                {
                    outputSFade(basicSequenceArray[sNo][1][j%SEQUENCE_LENGTH],
                                basicSequenceArray[sNo][1][(j+1)%SEQUENCE_LENGTH]);
                    // Are we using this not the Eq version??gen
                }
            }
        case 2:
            for (i=0; i<= repeats; i++)
            {
                for (j=0; j<=abs(sectionStartIndex-sectionEndIndex); j++)
                {
                    //Gradual;
                }
            }
        case 3:
            //Flashing;
    }
}


void outputSNormal(int sNo, int sectionStartIndex, int sectionEndIndex, int repeats)
{
    int i, j, p;
    for (i=0; i<=repeats; i++)
    {
        for (j=sectionStartIndex; j<=sectionEndIndex; j++)
        {
            p = (int) basicSequenceArray[sNo][1][j];
            outputPacket(packetArray[p]);
            // wait 1s
        }
    }
}

void outputSGradual(int p, int pNext)
{
    int i, j, k;
    char temp[PACKET_LENGTH], differential[PACKET_LENGTH];
    outputPacket(packetArray[p]);
    for (i=0; i<PACKET_LENGTH; i++)
    {
        differential[i] = (getSlot(p, i)-getSlot(pNext, i))/5; // Ignore warnings, will work as inputs are validated
    }
    for (j=0; j<=3; j++)
    {
        for (k=0; k<PACKET_LENGTH; k++)
        {
            temp[k] = diff[k]*j;  // Does this work for p>pNext??????
        }
        outputPacket(temp);
        //wait a bit
    }
}

void outputSFade(int p, int pNext)
{
    int i, j;
    char temp[PACKET_LENGTH];
    outputPacket(packetArray[p]);
    // Decrease
    for (i=255; i>=0; i--)
    {
        for (j=0; j<PACKET_LENGTH; j++)
        {
            if (packetArray[p][j] >= i)
            {
                temp[j] = i-1; // Need an else here??????????
            }
        }
        outputPacket(temp)
    }
    // Increase
    for (i=0; i<=255; i++)
    {
        for (j=0; j<PACKET_LENGTH; j++)
        {
            if (i < packetArray[pNext][j])
            {
                temp[j] = i+1; // Else again???????
            }
        }
        outputPacket(temp)
    }
}

// In case it looks better. Needs some refinement
void outputSFadeEq(int p, int pNext)
{
    int i, c, d;
    char cHigh[PACKET_LENGTH];
    char cLow[PACKET_LENGTH];
    char temp[PACKET_LENGTH];
    for (i=0; i<PACKET_LENGTH; i++)
    {
        cLow[i] = packetArray[p][i]/255;        // Ignore warnings, division is floored so will always be in the
        cHigh[i] = packetArray[pNext][i]/255;   // correct range
    }
    // Decrease
    for (c=0; c<=255; c++)
    {
        for (d=0; d<PACKET_LENGTH; d++)
        {
            temp[d] = packetArray[p][d] - cLow[d];  
        }
        outputPacket(temp);
    }
    // Increase     (this isn't right at the moment)
    for (c=0; c<=255; c++)
    {
        for (d=0; d<PACKET_LENGTH; d++)
        {
            temp[d] = packetArray[pNext][d] - cHigh[d];
        }
        outputPacket(temp);
    }
}



