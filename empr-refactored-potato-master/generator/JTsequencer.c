//
// Created by jt1268 on 06/03/18.
//

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

void advancedSequencer(void);
void savePacket(int pNo, data);
void outputSingleLoop(int pNo);
void saveSequence(int sNo, sections, data);
void outputSequence(int sNo);
int getSlot(int p, int slot);
void outputSection(int sNo, int sectionStart, int sectionEnd, int pattern, int repeats);
void outputSNormal(int sNo, int sectionStart, int sectionEnd, int repeats);
void outputSGradual(int p, int pNext);
void outputSFade(int p, int pNext);
void outputSFadeEq(int p, int pNext);


#define IC_CHANNEL 0
#define PACKET_LENGTH 10
#define NO_PACKETS 10
#define INIT_SIGNAL 101

char packetArray[NO_PACKETS][PACKET_LENGTH] = {
    // some stuff about packet data
};

void advancedSequencer(void)
{
    uint32_t len, idx;
    uint8_t initReceived, buffer[10];
    // Wait for serial transmission (10 slots)
    // Serial format: sequence type, packetData

    // Wait for init signal
    while (initReceived != INIT_SIGNAL)
    {
        initReceived = UART_ReveiveByte((LPC_UART_TypeDef *)LPC_UART0);
    }

    // Send packet length

    while (true) // Exit after no. packets
    {
        // Wait for serial input
        len = UARTReceive((LPC_UART_TypeDef *)LPC_UART0, buffer, sizeof(buffer));
        idx = 0;
        while(idx<len)
        {
            // Add buffer[idx] to packet
        }
    }
    //
}

void savePacket(int pNo, data)
{
    int i;
    for (i=0; i<PACKET_LENGTH; i++)
    {
        packetArray[pNo][i] = data[i];
    }
}

void outputSingleLoop(int pNo)
{
    int i;
    for (i=0; i<PACKET_LENGTH; i++)
    {
        dmx_output_single(packetArray[pNo][i], packetArray[pNo][i+1], packetArray[pNo][i+2], IC_CHANNEL);
        // needs wrap around thingy
    }
}

void saveSequence(int sNo, sections, data)
{
    // Save according to format
}

void outputSequence(int sNo)
{
    // Work out order of sections.
    // for section in sequence:
        outputSection(sNo, sectionStart, etc)
    
}

int getSlot(int p, int slot)
{
    int s = atoi((const char *) packetArray[p][slot]);
    return s;
}

void outputSection(int sNo, int sectionStart, int sectionEnd, int pattern, int repeats)
{
    int i, j;
    switch(pattern)
    {
        case 0:
            Normal;
        case 1:
            for (i=0; i<= repeats; i++)
            {
                for (j=sectionStart; j<=sectionEnd; j++)
                {
                    Fade;
                }
            }
        case 2:
            for (i=0; i<= repeats; i++)
            {
                for (j=sectionStart; j<=sectionEnd; j++)
                {
                    Gradual;
                }
            }
        case 3:
            Flashing;
    }
}


void outputSNormal(int sNo, int sectionStart, int sectionEnd, int repeats)
{
    int i, j, p;
    for (i=0; i<=repeats; i++)
    {
        for (j=sectionStart; j<=sectionEnd; j++)
        {
            p = sequence[sNo][j];
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
        differential[i] = (getSlot(p, i)-getSlot(pNext, i))/5; // Ignore warnings, will work as inputs are
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
        cLow[i] = packetArray[p][i]/255;
        cHigh[i] = packetArray[pNext][i]/255;
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



