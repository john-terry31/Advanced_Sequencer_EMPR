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
#include "vector.h"

void advancedSequencer(void);
uint8_t readSerialByte(void);
void fillSendPacketArray(void);
void fillSendSequenceArray(void);
void savePacket(const char * data);
void outputSingleLoop(int pNo);
saveSequenceSimple(char * data);
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
#define NO_OBJECTS 10
#define INIT_SIGNAL 101
#define SEQUENCE_LENGTH 10
#define CONFIRMATION_CODE 202
#define SECTION_ELEMENTS 4
#define SEQUENCE_LENGTH 10
