#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_rit.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pwm.h"
#include "lpc_types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void SysTick_Handler(void);
void EMPR_waitFor(int time);
void enableDAC(void);
void enableI2C(void);
void enatbleADC(void);
void EMPR_Print2Console(char Message[200]);
void EMPR_Setup(void);
I2C_M_SETUP_Type manualI2CSetup(int address,int txData,int txLength,int rxData,int rxLength,int retransmissions);
I2C_M_SETUP_Type I2CSetup(char* setupType);
void initialiseLCD(void);
int EMPR_GetChar(char chr); //changed this to just fetch the value for the char given
void EMPR_LCD_DispStr(char Str[30], int line);
void transferData(I2C_M_SETUP_Type setup, char data[200], int length);
void ledOn(int led); //Pass the pin number (i.e) 18.
void ledOff(int led); // turns off specified Led.
int hexFromPin(int pin); // gets the hex format for the specific pin
int pinFromLed(int led); // returns the pin number for specified led
void EMPR_LCD_Clear(void);
char EMPR_ReadKeypad(void);
void EMPR_sine(int amplitude, float frequency);
float EMPR_ReadADC(int channel, int pin);
void enableRIT(void);
void EMPR_OutputDAC(int voltage);
char *int2bin(int a, char *buffer, int buf_size);
