#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

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

/*void setInt(int);
void i2c_init(void);
void lcd_setup(void);
void lcd_send(void);
void kbd_test(void);
void kbd_read(void);
void kbd_write(void);
char kbd_get_key(void);
void lcd_clear();*/

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
//{blue , red , green

char packets[10][512] = //512 not 1536?
        {
                //{0}
                {0, 250,0,160,103,145,45,77,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170},
                {255,255,255},
                {255}
        };

char gen3Sequence[10];
int gen3SequenceLength = 0;


extern int RITCounter;
int waiter = 0;

/*char lcd_buff[30];
int lcd_buff_index = 0;*/


char lcd_key_lut[256];

void TIMER_Setup(void)
{
    //Configure timer interrupt to trigger every 100us.
    TIM_TIMERCFG_Type tim_cfg;
    //TIM_ConfigStructInit(TIM_TIMER_MODE, &tim_cfg);
    tim_cfg.PrescaleOption = TIM_PRESCALE_USVAL;
    tim_cfg.PrescaleValue = 100;

    TIM_MATCHCFG_Type match_cfg;
    match_cfg.IntOnMatch = ENABLE;
    match_cfg.MatchChannel = 0;
    match_cfg.ResetOnMatch = ENABLE;
    match_cfg.StopOnMatch = DISABLE;
    match_cfg.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE;
    match_cfg.MatchValue = 1;
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &tim_cfg);
    TIM_ConfigMatch(LPC_TIM0, &match_cfg);

    NVIC_EnableIRQ(TIMER0_IRQn);
}


/*
/ Timer interrupt - handles reading each bit.
/ Interrupt generated every 4 us.
/
*/
void TIMER0_IRQHandler(void) {
    waiter++;
    //setLedInt(2);
    //EMPR_Print2Console("timer");

    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
}

void shortWait(void)
{
    TIM_Cmd(LPC_TIM0, ENABLE);
    waiter = 0;
    while (waiter < 900)
    {
        //wait
    }
    TIM_Cmd(LPC_TIM0, DISABLE);
}

void specificWait(int waitTime)
{
    TIM_Cmd(LPC_TIM0, ENABLE);
    waiter = 0;
    while (waiter < waitTime)
    {
        //wait
    }
    TIM_Cmd(LPC_TIM0, DISABLE);
}

void main(void)
{
    EMPR_Setup();

    generator_main_screen();
}

/*
/ Changes the force break register value to allow signal changes
/ on the UART line.
/
/ param: UARTx - this will always be UART1 as it is the only one we use
*/
void UART_StopBreak(LPC_UART_TypeDef* UARTx)
{
    UARTx->LCR &= 0b10111111;
    //((LPC_UARTx_TypeDef *)UARTx)->LCR &= !UART_LCR_BREAK_EN;
}


/*
/ Outputs a single packet of data via UART. This includes the break and MAB -
/ plus all data bytes in the input parameter array.
/ 
/ param: packetArray - pointer to the array of values to be output.
*/
void outputPacket(char * packetArray)
{
    TIM_Cmd(LPC_TIM0, ENABLE); //enable timer
    //ledOn(3);
    while(UART_CheckBusy(LPC_UART2) == SET)
    {
        //wait
    }
    UART_ForceBreak(LPC_UART2);
    waiter = 0;
    while (waiter < 90)
    {
        //wait
    }


    //TIM_Cmd(LPC_TIM0, DISABLE); //disable timer

    UART_StopBreak(LPC_UART2);

    //TIM_Cmd(LPC_TIM0, ENABLE); //enable timer
    waiter = 0;
    while (waiter < 5)
    {
        //wait
    }
    TIM_Cmd(LPC_TIM0, DISABLE); //disable timer
    waiter = 0;

    // Start code
    outputByte(0);
    UART_Send(LPC_UART2, packetArray, 512, BLOCKING);
}

/*
/ Outputs a the given byte via UART. The start bit and stop bits are built
/ in to the UART protocol.
/
/ param: value - the byte to be output.
*/
void outputByte(int value)
{
    UART_SendByte(LPC_UART2, value);
}

void generator_main_screen(void)
{

    char GenReader;
    while(1) {
        EMPR_LCD_Clear();
        dmx_output_single(0,0,0,0);
        EMPR_LCD_DispStr("Generator Mode", 1);
        GenReader = EMPR_ReadKeypad();


        switch (GenReader) {
            case '1' :
                ledOn(1);
                EMPR_LCD_DispStr("MODE 1", 1);
                generator1();
                break;
            case '2' :
                ledOn(2); //need to fix the bounce here, auto selects packet 2

                generator2();
                break;
            case '3' :
                ledOn(3);
                generator3();
                break;
        }
    }
}

void dmx_output_single(int val1, int val2, int val3, int slot)
{
    //potentially add a time to this too and wait inside it? will be useful for ICs
    char packet[512] = {};
    packet[slot] = val1;
    packet[slot + 1] = val2;
    packet[slot + 2] = val3;
    outputPacket(&packet);

}

void generator1(void)
{
    //gonna make it cycle between the 3 primary colours
    while(1)
    {
        dmx_output_single(255,0,0,0); // Red
        specificWait(9000);

        dmx_output_single(0,255,0,0); // Green
        specificWait(9000);

        dmx_output_single(0,0,255,0); // Blue
        specificWait(9000);

        dmx_output_single(0,0,0,0); // Off
        specificWait(9000);
    }
}


/*
/ Reads the user's packet selection (in range 0-9).
/
/ returns: The keypad selection of the packet.
*/
int gen2_getPktSelection(void)
{
    specificWait(2000); // Small wait to avoid bounce.
    int pkt = ' '; // Change to " " to remove weird hovering 0?

    while (pkt == ' ')
    {
        pkt = gen2_readInput('p'); // Read user input and pass the
                                   // type p so it only reads 1 char.
    }
    return pkt;
}

/*
/ Allows data packets to be edited in memory.
/ 
/ param: void
/ returns: void
*/
void generator2(void)
{
    EMPR_LCD_DispStr("Choose packet", 1);
    // Wait for input of which packet to edit
    int pktSelection = gen2_getPktSelection();


    char packet[5];
    sprintf(packet, "P %i", pktSelection);
    // Display selected packet on the LCD.
    EMPR_LCD_DispStr(packet, 1);

    int slot;
    slot = gen2_getSlot();
    char slotandpacket[10];
    int loop = 0;
    char currentByte;

    while (loop == 0)
    {
        /////////////////////Recomment the next few lines JT ////////////////////////////////

        //Display packet/slot selection on L1 and the corresponding byte value on L2

        sprintf(slotandpacket, "P %i S %i", pktSelection, slot);
        EMPR_LCD_DispStr(slotandpacket, 1);
        currentByte = gen2_getCurrentByte(pktSelection, slot);
        char number[4];
        sprintf(number, "%i", currentByte);
        EMPR_Print2Console(number);
        EMPR_LCD_DispStr(number, 2);

        int newValue;
        newValue = gen2_getSlot();

        if (newValue < 256) // Special case for 'confirm' or 'back' input.
        {
            switch (newValue)
            {
                case -1 : //Press C to complete, is value 256
                        while(1){
                            outputPacket(packets[pktSelection]);
                            EMPR_ReadKeypad();
                            return;
                        }


                    return;
                case -2 : //press * to go back one slot
                    slot -= 1;
                    if (slot < 0){
                        slot = 512;
                    }
                    break;
                default:
                    gen2_setSlotValue(pktSelection, slot, newValue);
                    slot += 1;
                    if (slot > 512){
                        slot = 0;
                    }
                    break;
            }
        }
        else // Normal 'setting' of a slot value.
        {
            newValue = 0;
        }
    }
}

/*
/ Sets the selected slot to the specified value.
/ 
/ param: pktSelection - the selected packet
/        slot - the selected slot
/        value - the input value
/ returns: void
*/
void gen2_setSlotValue(int pktSelection, int slot, int newValue)
{
    packets[pktSelection][slot] = newValue;
}

/*
/ Gets the slot number the user has input. If the value is > 512, cycles
/ through until a valid input is given. 
/
/ returns: The slot value
*/
int gen2_getSlot(void)
{
    int slot = -10; // Used as a check against slot no. higher than 512.

    while (slot == -10)
    {
        slot = gen2_readInput('s'); // s for slot

        if (slot > 512)
        {
            slot = -10;
        }

    }
    return slot;
}

/*
/ Gets the current byte value for a specified packet and slot.
/ 
/ param: pktSelection - The selected packet.
/        slot - The selected slot.
/ returns: The byte value at packet[slot]
*/
char gen2_getCurrentByte(int pktSelection, int slot)
{
    return packets[pktSelection][slot];
}

/*
/ Generic number read for the 2nd generator task. Exits early for packet
/ selection as this can only be 1 digit.
/
/ param: inputType is the current input the system is waiting for. 'p' for packet 
/        and 'n' for any others.
/ returns: The number/key the user has input
*/
int gen2_readInput(char inputType)
{
    int total = 0;           // The total cumulative value from the keypad.
    int prevValue = 0;       // The value of 'total' on the previous loop.
    char dispStr[3];          // Array to display the input on the LCD.

    while ('true')
    {

        if (prevValue != total)
        {

            sprintf(dispStr, "%i", total);
            EMPR_LCD_DispStr(dispStr, 2);
            prevValue = total;
        }

        char inputKey = EMPR_ReadKeypad();
        shortWait();
        switch (inputKey)
        {
            case '0' :
                total *= 10;
                break;
            case '1' :
                total *= 10;
                total += 1;
                break;
            case '2' :
                total *= 10;
                total += 2;
                break;
            case '3' :
                total *= 10;
                total += 3;
                break;
            case '4' :
                total *= 10;
                total += 4;
                break;
            case '5' :
                total *= 10;
                total += 5;
                break;
            case '6' :
                total *= 10;
                total += 6;
                break;
            case '7' :
                total *= 10;
                total += 7;
                break;
            case '8' :
                total *= 10;
                total += 8;
                break;
            case '9' :
                total *= 10;
                total += 9;
                break;
            case 'D' : // Return the current value when 'D' is pressed.
                return total;
            case 'C' :
                return -1;
            case '*' :
                return -2;
        }

        if (inputType == 'p')
        {
            // Valid packet numbers are only 1 digit so the 
            // function should return after 1 key is entered.
            return total;
        }
        else if (inputType == 's' && total > 512){
            total = 0;

        }
        else if (inputType == 'v' && total > 255)
        {
            // None of the inputs should be more than 3 digits.
            total = 0;
        }
    }
}

int gen3_chooseRepeats()
{
    int total = 0;           // The total cumulative value from the keypad.
    int prevValue = 0;       // The value of 'total' on the previous loop.
    char dispStr[10];        // Array to display the input on the LCD.

    while ('true')
    {
        /////////// Error checking for big values /////////////////////// <0 etc
        if (prevValue != total)
        {
            if(total < 0){ //avoiding an overflow
                total = 2147483647;
            }
            sprintf(dispStr, "%i", total);
            EMPR_LCD_DispStr(dispStr, 2);
            prevValue = total;
        }

        char inputKey = EMPR_ReadKeypad();
        shortWait();
        switch (inputKey)
        {
            case '0' :
                total *= 10;
                break;
            case '1' :
                total *= 10;
                total += 1;
                break;
            case '2' :
                total *= 10;
                total += 2;
                break;
            case '3' :
                total *= 10;
                total += 3;
                break;
            case '4' :
                total *= 10;
                total += 4;
                break;
            case '5' :
                total *= 10;
                total += 5;
                break;
            case '6' :
                total *= 10;
                total += 6;
                break;
            case '7' :
                total *= 10;
                total += 7;
                break;
            case '8' :
                total *= 10;
                total += 8;
                break;
            case '9' :
                total *= 10;
                total += 9;
                break;
            case 'D' : // Return the current value when 'D' is pressed.
                return total;
        }
    }
}


void generator3(void)
{
    shortWait(); // Debounce

    EMPR_LCD_Clear();
    EMPR_LCD_DispStr("Enter Sequence", 1);
    gen3_makeSequence();
    EMPR_LCD_Clear();

    EMPR_LCD_DispStr("Enter Repetitions", 1);
    int repeats;
    shortWait(); // Debounce
    repeats = gen3_chooseRepeats();

    gen3_sendSequence(repeats);

}

/*
/ Read keypad for user packet sequence.
/
/
/ param: packetArray - The sequenced array of packet identifiers
*/
void gen3_makeSequence(void)
{
    char seqStr[10];
    int sequenceLength = 0; // Counts how many items are in the sequence up to the maximum of 10
    int keyPress = 0;
    int prevValue = 0;
    while (gen3SequenceLength < 10) // Sequences can have a max of 10 packets
    {
        if (gen3SequenceLength != 0)
        {
            EMPR_LCD_DispStr(gen3Sequence, 2);
        }

        char inputKey = EMPR_ReadKeypad();
        shortWait();

        switch (inputKey)
        {
            case 'D':
                return;
            default:
                gen3Sequence[gen3SequenceLength] = inputKey;
                gen3SequenceLength++;
                break;
        }
    }
}


void gen3_sendSequence(int repetitions)
{
    // Send the sequence and repeat the desired amount of times.
    int i, j;
    //for i -> repetitions
    //      for j -> noPackets
    //          send each packet
    //          wait

    EMPR_LCD_Clear();
    EMPR_LCD_DispStr("Running Sequence", 1);
    for (i = 0; i <= repetitions; i++)
    {
        for (j = 0; j < gen3SequenceLength; j++)
        {
            outputPacket(packets[gen3Sequence[j]]);
            shortWait();
        }
    }
}

void setInt(int number)
{
    GPIO_ClearValue(1, 0xB40000);
    switch (number)
    {
        case 1:
            GPIO_SetValue(1, 0x800000);
            break;
        case 2:
            GPIO_SetValue(1, 0x200000);
            break;
        case 3:
            GPIO_SetValue(1, 0xA00000);
            break;
        case 4:
            GPIO_SetValue(1, 0x100000);
            break;
        case 5:
            GPIO_SetValue(1, 0x900000);
            break;
        case 6:
            GPIO_SetValue(1, 0x300000);
            break;
        case 7:
            GPIO_SetValue(1, 0xB00000);
            break;
        case 8:
            GPIO_SetValue(1, 0x40000);
            break;
        case 9:
            GPIO_SetValue(1, 0x840000);
            break;
        case 10:
            GPIO_SetValue(1, 0x240000);
            break;
        case 11:
            GPIO_SetValue(1, 0xA40000);
            break;
        case 12:
            GPIO_SetValue(1, 0x140000);
            break;
        case 13:
            GPIO_SetValue(1, 0x940000);
            break;
        case 14:
            GPIO_SetValue(1, 0x340000);
            break;
        case 15:
            GPIO_SetValue(1, 0xB40000);
            break;
    }
}

