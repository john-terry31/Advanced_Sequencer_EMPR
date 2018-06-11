//
// Created by jt1268 on 06/03/18.
//

#include "LPC17xx.h"
#include "lpc17xx_uart.h"
#include "serial.h"
#include "lcd.h"
#include "EMPR.h"
#include "keypad.h"
#include "lpc_types.h"
#include "Generator.h"
#include "stdlib.h"
//#include "vector.h"

void advancedSequencer(void);
char readSerialByte(void);
int numDigits(int num);
void sendNumAsChar(int num);
void waitForInput(void);
void fillSendPacketArray(void);
void fillSendSequenceArray(void);
void savePacket(const char * data);
void outputSingleLoop(int pNo);
saveSequenceSimple(char * data);
//void saveSequence(int sNo, sections, data);
void outputSequenceSimple(int sNo);
void outputSequence(int sNo);
int getSlot(int p, int slot);
void outputSection(int sNo, int sectionStartIndex, int sectionEndIndex, int pattern, int repeats);
void outputSNormal(int sNo, int sectionStartIndex, int sectionEndIndex, int repeats);
void outputSGradual(int p, int pNext);
void outputSFade(int p, int pNext);
void outputSFadeEq(int p, int pNext);

#define IC_CHANNEL 0
#define PACKET_LENGTH 10
#define NO_OBJECTS 10
#define INIT_SIGNAL 101
#define SEQUENCE_LENGTH 10
#define CONFIRMATION_CODE 202
#define SECTION_ELEMENTS 4
#define SEQUENCE_LENGTH 10
