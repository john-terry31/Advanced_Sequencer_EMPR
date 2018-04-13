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


#define M_PI acos(-1.0)
#define RELOAD_VALUE 9600000
#define KEYPAD_I2C_ADDRESS   (0x21)
#define LCD_I2C_ADDRESS (0x3b)
#define STCTRL      (*( ( volatile unsigned long *) 0xE000E010 ))
#define STRELOAD    (*( ( volatile unsigned long *) 0xE000E014 ))
#define STCURR (*( ( volatile unsigned long *) 0xE000E018 ))
#define SBIT_ENABLE     0
#define SBIT_TICKINT    1
#define SBIT_CLKSOURCE  2
#define VREF       3.3 //Reference Voltage at VREFP pin, given VREFN = 0V(GND)
#define ADC_CLK_EN (1<<12)
#define SEL_AD0_0  (1<<0) //Select Channel AD0.0 
#define CLKDIV     1 //ADC clock-divider (ADC_CLOCK=PCLK/CLKDIV+1) = 12.5Mhz @ 25Mhz PCLK
#define PWRUP      (1<<21) //setting it to 0 will power it down
#define START_CNV  (1<<24) //001 for starting the conversion immediately
#define ADC_DONE   (1U<<31) //define it as unsigned value or compiler will throw #61-D warning
#define ADCR_SETUP_SCM ((CLKDIV<<8) | PWRUP)


void EMPR_Print2Console(char Message[190]);
void EMPR_LCD_DispStr(char Str[30], int line);
char *int2bin(int a, char *buffer, int buf_size);
void KeypadSetup(void);
int hexFromPin(int pin);
int pinFromLed(int led);
void ledOn(int led);
void ledOff(int led);
void transferData(I2C_M_SETUP_Type setup, char data[200], int length);
int EMPR_GetChar(char chr);
void EMPR_WriteToLine(int line);
void EMPR_LCD_Clear(void);
void initialiseLCD(void);
void changePinToGPIO();
void changePinToUART();
void dmx_init(void);

int charOnScreenCount;
char KEYPAD_LETTERS0[5] = "123A";
char KEYPAD_LETTERS1[5] = "456B";
char KEYPAD_LETTERS2[5] = "789C";
char KEYPAD_LETTERS3[5] = "*0#D";
int globalTime = 0;
int timeCounter = 0;
int RITCounter = 0;
float wavecounter = 0;
int keyPressed = 0;
int timer5 = 0;



void delay_ms(unsigned int ms)
{
    timer5 = 0;
    while(timer5 <= ms);
} 


#define SBIT_CNTEN     0 
#define SBIT_PWMEN     2

#define SBIT_PWMMR0R   1

#define SBIT_LEN0      0
#define SBIT_LEN1      1
#define SBIT_LEN2      2
#define SBIT_LEN3      3
#define SBIT_LEN4      4

#define SBIT_PWMENA1   9
#define SBIT_PWMENA2   10
#define SBIT_PWMENA3   11
#define SBIT_PWMENA4   12


#define PWM_1          0 //P2_0 (0-1 Bits of PINSEL4)
#define PWM_2          2 //P2_1 (2-3 Bits of PINSEL4)
#define PWM_3          4 //P2_2 (4-5 Bits of PINSEL4)
#define PWM_4          6 //P2_3 (6-7 Bits of PINSEL4)



void EMPR_PWM_CYCLE(void)
{
    int dutyCycle; 

    /* Enable Counters,PWM module */ 
    LPC_PWM1->TCR = (1<<SBIT_CNTEN) | (1<<SBIT_PWMEN);

    LPC_PWM1->PR  =  0x0;               /* No Prescalar */
    LPC_PWM1->MCR = (1<<SBIT_PWMMR0R);  /*Reset on PWMMR0, reset TC if it matches MR0 */

    LPC_PWM1->MR0 = 100;                /* set PWM cycle(Ton+Toff)=100) */
    PWM_MatchUpdate(LPC_PWM1, 2, 50, PWM_MATCH_UPDATE_NOW);
   
    /* Trigger the latch Enable Bits to load the new Match Values */
    LPC_PWM1->LER = (1<<SBIT_LEN0) | (1<<SBIT_LEN1) | (1<<SBIT_LEN2) | (1<<SBIT_LEN3) | (1<<SBIT_LEN4); 


    for(dutyCycle=0; dutyCycle<100; dutyCycle++)
    {
        PWM_MatchUpdate(LPC_PWM1, 2, dutyCycle, PWM_MATCH_UPDATE_NOW);  /* Increase the dutyCycle from 0-100 */

        delay_ms(25);
    }

    for(dutyCycle=100; dutyCycle>0; dutyCycle--)
    {
        PWM_MatchUpdate(LPC_PWM1, 2, dutyCycle, PWM_MATCH_UPDATE_NOW);  
      
        delay_ms(25);
    }
} 

void enablePWM(void)
{
	PINSEL_CFG_Type PinCfg;
	
    PinCfg.Funcnum     = PINSEL_FUNC_1;
    PinCfg.OpenDrain   = 0;
    PinCfg.Pinmode     = 0;
    PinCfg.Portnum     = 2; //PWM Channel 2 on pin 25 on breadboard
    PinCfg.Pinnum      = 1;
    PINSEL_ConfigPin(&PinCfg);
	PWM_TIMERCFG_Type PWMTimer;
    
	//PWMTimer.PrescaleOption = PWM_TIMER_PRESCALE_TICKVAL;
	//PWMTimer.PrescaleValue = 0x00000008;
	PWM_Cmd(LPC_PWM1, ENABLE);
	PWM_ConfigStructInit(PWM_MODE_TIMER, &PWMTimer);
	PWM_Init(LPC_PWM1, PWM_MODE_TIMER, &PWMTimer);
	PWM_ChannelConfig(LPC_PWM1, 2, PWM_CHANNEL_SINGLE_EDGE);
	PWM_ChannelCmd(LPC_PWM1, 2, ENABLE);	
}

void RIT_IRQHandler(void)
{
    switch (RITCounter)
    {
        case 0:
            RITCounter = 1;
            break;
        case 1:
            RITCounter = 0;
            break;
    }

	wavecounter++;
	globalTime++;
	LPC_RIT -> RICTRL |= 0x01; //resets the RIT timer 

	timer5++;
}

void enableRIT(void)
{
	RIT_Init(LPC_RIT);
	RIT_TimerConfig(LPC_RIT, 1); //is this at 1ms?
    LPC_RIT->RICOMPVAL = (uint32_t) 380; //should give us 4us
	NVIC_EnableIRQ(RIT_IRQn);
	RIT_Cmd(LPC_RIT, ENABLE);

}

void EMPR_sine(int amplitude, float frequency)
{
	double wavelength;
	wavelength = 1000.0 / frequency;
	float value;
	float maxamp;
	globalTime = 0;
	maxamp = (amplitude * 1024.0) / 33;  //amplitude / 3.3 * 1024 should give right number
	DAC_UpdateValue(LPC_DAC, value);
	globalTime = 0;
	
    while(globalTime <= 10000)
    {
		char buff[20];
        sprintf(buff, "%i",globalTime);
        EMPR_Print2Console(buff);
		wavecounter = 0;
		value = 0;
		while(wavecounter <= wavelength)
		{
            //multiply 2pi by how far through the wave you should be, 
            //then multiply the sin of this by the max voltage we want
			value =  (sin(2.0 * M_PI *(wavecounter / wavelength )) + 1) * maxamp; 
			DAC_UpdateValue(LPC_DAC, value);
			if(keyPressed == 1)
			{
				return;
			}
		}
	}
}


void SysTick_Handler(void)
{
	timeCounter++;
}

void enableDAC(void) //COMES OUT ON P17
{
	/* Configure Pins */
    PINSEL_CFG_Type PinCfg;

   	PinCfg.Funcnum     = PINSEL_FUNC_2;
   	PinCfg.OpenDrain   = 0;
   	PinCfg.Pinmode     = 0;
   	PinCfg.Portnum     = 0;
    PinCfg.Pinnum      = 26;
    PINSEL_ConfigPin(&PinCfg);
	
	DAC_Init(LPC_DAC);	
}

void enableADC(void)
{
	PINSEL_CFG_Type PinCfg;
  	PinCfg.Funcnum   = PINSEL_FUNC_1;
  	PinCfg.OpenDrain = 0;
   	PinCfg.Pinmode   = 0;
   	PinCfg.Portnum   = 0;
   	PinCfg.Pinnum    = 25;
	PINSEL_ConfigPin(&PinCfg);

	ADC_Init(LPC_ADC, 100);
	// Enable ADC channel 1.
   	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_2 	, ENABLE);
   
   	// Set ADC to continuously sample.
   	ADC_StartCmd (LPC_ADC, ADC_START_CONTINUOUS);
}

void EMPR_OutputDAC(int voltage) //pass voltage * 10
{
	float maxamp;
	maxamp = (voltage * 1024.0) / 33;  //amplitude / 3.3 * 1024 should give right number
	char buff[20];
	sprintf(buff, "%i", voltage);
	EMPR_Print2Console(buff);
	DAC_UpdateValue(LPC_DAC, maxamp);
}

double EMPR_ReadADC(int channel, int pin)
{
	PINSEL_CFG_Type PinCfg;
  	PinCfg.Funcnum   = PINSEL_FUNC_1;
  	PinCfg.OpenDrain = 0;
   	PinCfg.Pinmode   = 0;
   	PinCfg.Portnum   = 0;
   	PinCfg.Pinnum    = 25;
	PINSEL_ConfigPin(&PinCfg);
    
	ADC_Init(LPC_ADC, 100);
	// Enable ADC channel 1.
   	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_2, ENABLE);
    
   	// Set ADC to continuously sample.
   	ADC_StartCmd (LPC_ADC, ADC_START_CONTINUOUS);
    
    
	int result;			
	result = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_2) ; //12 bit Mask to extract result
	double doubleresult = ( result * 3.3) / 4095;
	char buff[20];
	sprintf(buff, "%f", doubleresult);
	EMPR_Print2Console(buff);
	return doubleresult;
}

void EMPR_waitFor(int time)
{
	int waiter = time + 10;
	timeCounter = 0;
	while(timeCounter < waiter)
	{
        // Do nothing.
	}
}

void enableI2C(void)
{
    /* Configure Pins */
    PINSEL_CFG_Type PinCfg;

    PinCfg.Funcnum     = PINSEL_FUNC_3;
    PinCfg.OpenDrain   = 0;
    PinCfg.Pinmode     = 0;
    PinCfg.Portnum     = 0;

    PinCfg.Pinnum      = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum      = 1;
    PINSEL_ConfigPin(&PinCfg);

    // Configure I2C to run at 100000Hz
    I2C_Init(LPC_I2C1, 100000);

    // Enable the I2C device
    I2C_Cmd(LPC_I2C1, ENABLE);
}

void EMPR_Print2Console(char Message[190])
{
	char StringBuff[200];
	sprintf(StringBuff, Message);
	char* String2Print;
	String2Print = strcat(StringBuff, " \n\r");
	int stringLength;
	stringLength = strlen(String2Print) + 1;
	write_usb_serial_blocking(String2Print,stringLength);
}



void EMPR_Setup(void)
{
	GPIO_SetDir(0, 0xFFFFFFFF, 1);
	GPIO_SetDir(1, 0xFFFFFFFF, 1);
	STRELOAD = RELOAD_VALUE; // Reload value for 1ms tick
	STCTRL = (1<<SBIT_ENABLE) | (1<<SBIT_TICKINT) | (1<<SBIT_CLKSOURCE);
	serial_init();


    lcd_setup();

	KeypadSetup();
	enableDAC();

	enableADC();
	//enableRIT();

	enablePWM();
    dmx_init();
	TIMER_Setup();
}


void changePinToUART()
{
	PINSEL_CFG_Type PinCfg;				// Pin configuration for UART
	/*
     * Initialize UART pin connect
     */
	PinCfg.Funcnum = PINSEL_FUNC_1; //UART FuncNum
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 10; //Transmitter, P13 on blueboard (now p9)
	PINSEL_ConfigPin(&PinCfg);
}


void dmx_init(void)
{
    UART_CFG_Type UARTConfigStruct;			// UART Configuration structure variable
    UART_FIFO_CFG_Type UARTFIFOConfigStruct;	// UART FIFO configuration Struct variable

    changePinToUART();

    /* Initialize UART Configuration parameter structure to:
     * - Baudrate = 250000bps
     * - 8 data bit
     * - 2 Stop bits
     * - None parity
     */
    UART_ConfigStructInit(&UARTConfigStruct);

    UARTConfigStruct.Baud_rate = 250000;
    UARTConfigStruct.Stopbits = UART_STOPBIT_2;
	UARTConfigStruct.Databits = UART_DATABIT_8;
	UARTConfigStruct.Parity = UART_PARITY_NONE;


    /* Initialize FIFOConfigStruct to default state:
     * - FIFO_DMAMode = DISABLE
     * - FIFO_Level = UART_FIFO_TRGLEV0
     * - FIFO_ResetRxBuf = ENABLE
     * - FIFO_ResetTxBuf = ENABLE
     * - FIFO_State = ENABLE
     */
    UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
    // Built the basic structures, lets start the devices/
    // USB serial
    UART_Init((LPC_UART_TypeDef *)LPC_UART2, &UARTConfigStruct);		// Initialize UART1 peripheral with given to corresponding parameter
    UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART2, &UARTFIFOConfigStruct);	// Initialize FIFO for UART1 peripheral
    UART_TxCmd((LPC_UART_TypeDef *)LPC_UART2, ENABLE);			// Enable UART Transmit

}

I2C_M_SETUP_Type manualI2CSetup(int address,int txData,int txLength,int rxData,int rxLength,int retransmissions)
{
    I2C_M_SETUP_Type i2c_m_setup;
    
    i2c_m_setup.sl_addr7bit = address;
    i2c_m_setup.tx_data = &txData;
    i2c_m_setup.tx_length = txLength;
    i2c_m_setup.rx_data = &rxData;
    i2c_m_setup.rx_length = rxLength;
    i2c_m_setup.retransmissions_max = retransmissions;
    return i2c_m_setup;
}

I2C_M_SETUP_Type I2CSetup(char* setupType)
{
    I2C_M_SETUP_Type i2c_m_setup;
    if(setupType == "LCD")
	{
        i2c_m_setup = manualI2CSetup(LCD_I2C_ADDRESS, 0x00, 1, 0x00, 1, 3);
	}
    else
	{
        i2c_m_setup = manualI2CSetup(0, 0x00, 1, 0x00, 1, 3);
	}
}


/*
* Set up the LCD display with the initial default values.
*/
void initialiseLCD(void)
{


    I2C_M_SETUP_Type i2c_m_setup;
    i2c_m_setup = manualI2CSetup(LCD_I2C_ADDRESS, 0, 12, NULL, 0, 3);

    char dataBuff[200] = {0x00, 0x34, 0x0c, 0x06, 0x37, 0x04, 0x10, 0x42, 0x9f, 0x34, 0x80, 0x02 };


    transferData(i2c_m_setup, &dataBuff, 12);
    dataBuff[1] = 0x34;
    dataBuff[2] = 0x0E;
    dataBuff[3] = 0x06;
    transferData(i2c_m_setup, &dataBuff, 4);

    dataBuff[1] = 0x01;
    //transferData(i2c_m_setup, &dataBuff, 2);
    EMPR_LCD_Clear();




}

void EMPR_LCD_Clear(void)
{

    I2C_M_SETUP_Type i2c_m_setup;
    i2c_m_setup = manualI2CSetup(LCD_I2C_ADDRESS, 0, 18, NULL, 0, 3);

    EMPR_WriteToLine(1);

    char dataBuff[200] = {0x40, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0};

    transferData(i2c_m_setup, &dataBuff, 18); //clears line 1

    EMPR_WriteToLine(2);
    transferData(i2c_m_setup, &dataBuff, 18); //clears line 2
}



void EMPR_ClearLine(line)
{
    I2C_M_SETUP_Type i2c_m_setup;
	i2c_m_setup = manualI2CSetup(LCD_I2C_ADDRESS, 0, 0, NULL, 0, 3);
    
    char spaces[200] = {0x40, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0};
    transferData(i2c_m_setup, &spaces, 18);
}

void EMPR_SelectLine(line)
{
    I2C_M_SETUP_Type i2c_m_setup;
	i2c_m_setup = manualI2CSetup(LCD_I2C_ADDRESS, 0, 0, NULL, 0, 3);
	
    char setLine[2];
	if(line == 1)
    {
        setLine[0] = 0x00;
        setLine[1] = 0x80;
	}
	else
	{
        setLine[0] = 0x00;
        setLine[1] = 0xC0;
	}
    transferData(i2c_m_setup, &setLine, 2);
}

void EMPR_WriteToLine(int line) {
    I2C_M_SETUP_Type i2c_m_setup;
    i2c_m_setup = manualI2CSetup(LCD_I2C_ADDRESS, 0, 18, NULL, 0, 3);

    char setLine[2];

	if(line == 1)
    {
        setLine[0] = 0x00;
        setLine[1] = 0x80;
	}
	else
	{
        setLine[0] = 0x00;
        setLine[1] = 0xC0;
	}
    
	transferData(i2c_m_setup, &setLine, 2);
    EMPR_ClearLine(line);
    transferData(i2c_m_setup, &setLine, 2);
}

int EMPR_GetChar(char chr) //changed this to just fetch the value for the char given
{
	char UpperChr;
	UpperChr = toupper(chr);
	int CharAscii = (int)UpperChr;
	
	switch(CharAscii)  
    //messy case statement, takes ascii value and assigns val2Print the 
    //correct value for the controller for that character
	{
		//character set r
		case 64: //@
			return 0b00001000;
		case 65: //A
			return 0b11000001;
		case 66: //B
			return 0b11000010;
		case 67: //C
			return 0b11000011;
		case 68: //D
			return 0b11000100;
		case 69: //E
			return 0b11000101;
		case 70: //F
			return 0b11000110;
		case 71: //G
			return 0b11000111;
		case 72: //H
			return 0b11001000;	
		case 73: //I
			return 0b11001001;	
		case 74: //J
			return 0b11001010;	
		case 75: //K
			return 0b11001011;
		case 76: //L
			return 0b11001100;		
		case 77: //M
			return 0b11001101;		
		case 78: //N
			return 0b11001110;	
		case 79: //O
			return 0b11001111;
		case 80: //P
			return 0b11010000;
		case 81: //Q
			return 0b11010001;
		case 82: //R
			return 0b11010010;
		case 83: //S
			return 0b11010011;
		case 84: //T
			return 0b11010100;
		case 85: //U
			return 0b11010101;
		case 86: //V
			return 0b11010110;
		case 87: //W
			return 0b11010111;
		case 88: //X
			return 0b11011000;
		case 89: //Y
			return 0b11011001;
		case 90: //Z
			return 0b11011010;
		case 91: //[
			return 0b10001010;
		case 94: //
			return 0b10101111;
		case 95: //_
			return 0b01011010;
		case 32: //space
			return 0b10100000;
		case 33: //!
			return 0b10100001;
		case 34: //"
			return 0b10100001;
		case 35: //#
			return 0b10100011;
		case 37: //%
			return 0b10100101;
		case 38: //&
			return 0b10100110;
		case 39: //'
			return 0b10100111;
		case 40: //(
			return 0b10101000;
		case 41: //)
			return 0b10101001;
		case 42: //*
			return 0b10101010;
		case 43: //+
			return 0b10101011;
		case 44: //,
			return 0b10101100;
		case 45: //-
			return 0b10101101;
		case 46: //_
			return 0b01011010;
		case 47: ///
			return 0b10101111;
		case 48: //0
			return 0b10110000;
		case 49: //1
			return 0b10110001;
		case 50: //2
			return 0b10110010;
		case 51: //3
			return 0b10110011;
		case 52: //4
			return 0b10110100;
		case 53: //5
			return 0b10110101;
		case 54: //6
			return 0b10110110;
		case 55: //7
			return 0b10110111;
		case 56: //8
			return 0b10111000;
		case 57: //9
			return 0b10111001;
		case 58: //:
			return 0b10111010;
		case 59: //;
			return 0b10111011;
		case 60: //<
			return 0b10111100;
		case 61: //=
			return 0b10111101;
		case 62: //>
			return 0b10111110;
		case 63: //?
			return  0b10111111;
		default:
			EMPR_Print2Console("ERROR: unknown character"); //displays error in console
			return  0b00010011; //prints a black box if unknown character
	}
}

void EMPR_LCD_DispStr(char Str[30], int line)
{
	if(line == 1)
    {
		EMPR_WriteToLine(1);
	}
	else
    {
		EMPR_WriteToLine(2);
	}
    
	char dataBuff[200];
	dataBuff[0] = 0x40;
	I2C_M_SETUP_Type i2c_m_setup;                                    
    i2c_m_setup = manualI2CSetup(LCD_I2C_ADDRESS, 0x00, 1, 0x00, 0, 3); 
    
	int i; 
	for(i = 0; i < strlen(Str); i++)
	{
		dataBuff[i + 1] = EMPR_GetChar(Str[i]);
	}
	transferData(i2c_m_setup, &dataBuff, i + 1);
}

void transferData(I2C_M_SETUP_Type setup, char data[200], int length)
{
	setup.tx_length = length;
	setup.tx_data = data;
	Status i2c_status = I2C_MasterTransferData(LPC_I2C1, &setup, I2C_TRANSFER_POLLING);
}

void ledOn(int led)
{
	GPIO_SetValue(1, hexFromPin(pinFromLed(led)));
}

/*
* Turns of the given LED.
*
* param led to turn off
* return void
*/
void ledOff(int led)
{
	GPIO_ClearValue(1, hexFromPin(pinFromLed(led)));
}

int pinFromLed(int led)
{
	switch(led)
	{
		case 1:
		{
			return(18);
		}
		case 2:
		{
			return(20);
		}
		case 3:
		{
			return(21);
		}
		case 4:
		{
			return(23);
		}
	}
	return(0);
}

int hexFromPin(int pin)
{
	switch(pin)
	{
		case 18:
		{
			return(0x00040000);
		}
		case 20:
		{
			return(0x00100000);
		}
		case 21:
		{
			return(0x00200000);
		}
		case 23:
		{
			return(0x00800000);
		}
	}
	return(0);
}


void KeypadSetup(void){

   uint8_t data_out = 0x0F;
   uint8_t data_in = 0x00;

   I2C_M_SETUP_Type i2c_m_setup;

   i2c_m_setup.sl_addr7bit = KEYPAD_I2C_ADDRESS;
   i2c_m_setup.tx_data = &data_out;
   i2c_m_setup.tx_length = sizeof(data_out);
   i2c_m_setup.rx_data = &data_in;
   i2c_m_setup.rx_length = sizeof(data_in);
   i2c_m_setup.retransmissions_max = 3;

   Status i2c_status = I2C_MasterTransferData(LPC_I2C1, &i2c_m_setup, I2C_TRANSFER_POLLING);
}
char key_lut[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
};

char EMPR_ReadKeypad(void) //reworked this to allow it to work for MP3, now has a 1 second window when called to read an input, returns 1 if input 0 if not
{
	uint8_t data_out = 0x0F;
   	uint8_t data_in = 0x00;
	I2C_M_SETUP_Type i2c_m_setup;  
	i2c_m_setup = manualI2CSetup(KEYPAD_I2C_ADDRESS, 0x00, 1, 0x00, 0, 1); 
	i2c_m_setup.tx_data = &data_out;
	i2c_m_setup.rx_data = &data_in;
    while(1) {
        int i;
        for (i = 0; i < 4; i++) {
            i2c_m_setup.rx_length = 0;
            i2c_m_setup.tx_length = 1;

            switch (i) {
                case 0:
                    data_out = 0b01111111;
                    break;
                case 1:
                    data_out = 0b10111111;
                    break;
                case 2:
                    data_out = 0b11011111;
                    break;
                case 3:
                    data_out = 0b11101111;
                    break;

            }


            Status i2c_status = I2C_MasterTransferData(LPC_I2C1, &i2c_m_setup, I2C_TRANSFER_POLLING);

            i2c_m_setup.tx_length = 0;
            i2c_m_setup.rx_length = 1;
            i2c_status = I2C_MasterTransferData(LPC_I2C1, &i2c_m_setup, I2C_TRANSFER_POLLING);

            data_in = data_in | 0xF0;
            data_in = data_in ^ 0xFF;

            char returner[2];
            returner[1] = "\0";
            switch (data_in) {
                case 1:

                    switch (i) {
                        case 0:
                            return '*';
                            //return key_lut[3][0];
                        case 1:

                            return key_lut[3][1];
                        case 2:

                            return key_lut[3][2];
                        case 3:

                            return key_lut[3][3];
                    }
                case 2:

                    switch (i) {
                        case 0:

                            return key_lut[2][0];
                        case 1:

                            return key_lut[2][1];
                        case 2:

                            return key_lut[2][2];
                        case 3:

                            return key_lut[2][3];
                    }
                case 4:

                    switch (i) {
                        case 0:

                            return key_lut[1][0];
                        case 1:

                            return key_lut[1][1];
                        case 2:

                            return key_lut[1][2];
                        case 3:

                            return key_lut[1][3];
                    }
                case 8:

                    switch (i) {
                        case 0:
                            return '1';
                            //return key_lut[0][0];
                        case 1:
                            return'2';
                            //return key_lut[0][1];
                        case 2:

                            return key_lut[0][2];
                        case 3:

                            return key_lut[0][3];
                    }
                    /*	case 0:

                            return 0;*/
            }
        }
    }
}

char *int2bin(int a, char *buffer, int buf_size)
{
    int i;
    buffer += (buf_size - 1);

    for (i = 3; i >= 0; i--)
    {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }
    return buffer;
}