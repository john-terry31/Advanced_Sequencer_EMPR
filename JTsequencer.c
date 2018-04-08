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

#define PACKETLENGTH 10
#define NO_PACKETS 10

int packetArray[NO_PACKETS][PACKETLENGTH] = {
    // some stuff about packet data
}

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

void savePacket(pNo, data)
{
    int i;
    for (i=0; i<PACKETLENGTH; i++)
    {
        packetArray[pNo][i] = data[i];
    }
}

void outputSingleLoop(pNo)
{
    int i;
    for (i=0; i<PACKETLENGTH; i++)
    {
    // DMX_Output_Single(packetArray[pNo][i], packetArray[pNo][i+1], packetArray[pNo][i+2]
    // needs wrap around thingy
    }
}

void saveSequence(sNo, sections, data)
{
    // Save according to format
}

void outputSequence(sNo)
{
    // Work out order of sections.
    // for section in sequence:
        outputSection(sNo, sectionStart, etc)
    
}

void outputSection(sNo, sectionStart, sectionEnd, pattern, repeats)
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

void outputSNormal(sNo, sectionStart, sectionEnd, repeats)
{
    int i, j, p;
    for (i=0; i<=repeats; i++)
    {
        for (j=sectionStart; j<=sectionEnd; j++)
        {
            p = sequence[sNo][j]
            outputPacket(packetArray[p])
            // wait 1s
        }
    }
}

void outputSGradual(p, pNext)
{
    int i, j, k;
    int temp[PACKETLENGTH], differential[PACKETLENGTH];
    outputPacket(packetArray[p]);
    for (i=0; i<PACKETLENGTH; i++)
    {
        differential[i] = (packetArray[p][i]-packetArray[pNext][i])/5;
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

void outputSFade(p, pNext)
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
void outputSFadeEq(p, pNext)
{
    int temp[PACKETLENGTH], cLow[PACKETLENGTH], cHigh[PACKETLENGTH], i, c, d;
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



