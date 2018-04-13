//
// Created by jt1268 on 13/04/18.
//
#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "serial.h"
#include "EMPR.h"
#include "lcd.h"
#include "keypad.h"

void TIMER_Setup(void);
void specificWait(int waitTime);
void shortWait(void);
void generator_main_screen(void);
void generator1(void);
void outputPacket(char *packetArray);
void outputByte(int value);
void outputFrom2DArray(int packet);
void generator2(void);
int gen2_getPktSelection(void);
void gen2_setSlotValue(int pktSelection, int slot, int value);
int gen2_getSlot(void);
char gen2_getCurrentByte(int pktSelection, int slot);
int gen2_readInput(char inputType);
void generator3(void);
void gen3_makeSequence(void);
void gen3_displaySequence(void);
char gen3_chooseRepeat(void);
void gen3_sendSequence(int repetitions);
void dmx_output_single(int val1, int val2, int val3, int slot);