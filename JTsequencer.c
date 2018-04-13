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

#define PACKETLENGTH 10
#define NO_PACKETS 10

char packetArray[NO_PACKETS][PACKETLENGTH] = {
    // some stuff about packet data
};

void advancedSequencer(void)
{
    uint32_t len, idx;
    uint8_t buffer[10];
    // Wait for serial transmission (10 slots)
    // Serial format: sequence type, packetData


    // Read sequence type

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
    for (i=0; i<PACKETLENGTH; i++)
    {
        packetArray[pNo][i] = data[i];
    }
}

void outputSingleLoop(int pNo)
{
    int i;
    for (i=0; i<PACKETLENGTH; i++)
    {
    // DMX_Output_Single(packetArray[pNo][i], packetArray[pNo][i+1], packetArray[pNo][i+2]
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
    char temp[PACKETLENGTH], differential[PACKETLENGTH];
    outputPacket(packetArray[p]);
    for (i=0; i<PACKETLENGTH; i++)
    {
        differential[i] = (getSlot(p, i)-getSlot(pNext, i))/5; // Ignore warnings, will work as inputs are
    }
    for (j=0; j<=3; j++)
    {
        for (k=0; k<PACKETLENGTH; k++)
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
    int temp[PACKETLENGTH];
    outputPacket(packetArray[p]);
    // Decrease
    for (i=255; i>=0; i--)
    {
        for (j=0; j<PACKETLENGTH; j++)
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
        for (j=0; j<PACKETLENGTH; j++)
        {
            if (i < packetArray[pNext][j])
            {
                temp[j] = i+1; // Else again???????
            }
        }
        outputPacket(temp)
    }
}

// Incase it looks better. Needs some refinement
void outputSFadeEq(int p, int pNext)
{
    int i, c, d;
    int cHigh[PACKETLENGTH];
    int cLow[PACKETLENGTH];
    int temp[PACKETLENGTH];
    for (i=0; i<PACKETLENGTH; i++)
    {
        cLow[i] = packetArray[p][i]/255;
        cHigh[i] = packetArray[pNext][i]/255;
    }
    // Decrease
    for (c=0; c<=255; c++)
    {
        for (d=0; d<PACKETLENGTH; d++)
        {
            temp[d] = packetArray[p][d] - cLow[d];  
        }
        outputPacket(temp);
    }
    // Increase     (this isn't right at the moment)
    for (c=0; c<=255; c++)
    {
        for (d=0; d<PACKETLENGTH; d++)
        {
            temp[d] = packetArray[pNext][d] - cHigh[d];
        }
        outputPacket(temp);
    }
}



