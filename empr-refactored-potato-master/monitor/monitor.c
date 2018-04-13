#include "string.h"
#include "stdlib.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"
#include "lpc_types.h"
#include "lpc17xx_exti.h"
#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "serial.c"
#include "lcd.c"
//#include "leds.h"
#include "keypad.c"


int mode = 0;

int counter = 0;
int bits_read = 0;
char frames[513];
char frame = 0;
char got_packet = 0;
int channel = 0;

void init(void);

void EINT3_IRQHandler(void);

void TIMER0_IRQHandler(void);

char read_frame(void);


void main(void) {
    char key;
    int i;

    init();

    serial_write_blocking(
            "Select mode:\n 1.Mode 1 - 4 bytes real-time \n 2.Mode 2 - Capture packet using key\n 3.Mode 3 - Capture packet using trigger condition \n",
            0);
    lcd_write("SELECT MODE");


    while (1) {
        //rebuild_key_lut();

        setLedInt(0);


        switch (mode) {
            case 0:

                key = kbd_get_key();

                if (key != 0) {
                    switch (key) {

                        case '1':
                            serial_write_blocking("Mode 1 selected\n", 0);
                            mode = 1;
                            mode1();
                            break;

                        case '2':
                            mode = 2;
                            serial_write_blocking("Mode 2 selected\n", 0);
                            mode2();
                            break;

                        case '3':
                            mode = 3;
                            serial_write_blocking("Mode 3 selected\n", 0);
                            mode3();
                            break;

                    }

                }

                break;
        }

        switch (mode) {
            case 1:
                //mode1();
                /*while(1){
                    NVIC_EnableIRQ(EINT3_IRQn);
                    GPIO_IntCmd(0, 0x20000, 0);
                    NVIC_EnableIRQ(TIMER0_IRQn);
                }*/

                break;
            case 2:
                mode2();
                /*while(kbd_get_key == 0);
                NVIC_EnableIRQ(EINT3_IRQn);
                GPIO_IntCmd(0, 0x20000, 0);
                NVIC_EnableIRQ(TIMER0_IRQn); */
                break;
            case 3:
                //mode3();
                break;
        }
    }
}

void mode1() {
    while (1) {
        NVIC_EnableIRQ(EINT3_IRQn);
        GPIO_IntCmd(0, 0x20000, 0);
        NVIC_EnableIRQ(TIMER0_IRQn);
    }
}

void mode2() {

    counter = 0;
    got_packet = 0;
    NVIC_EnableIRQ(EINT3_IRQn);
    GPIO_IntCmd(0, 0x20000, 0);
    NVIC_EnableIRQ(TIMER0_IRQn);

    while (!(got_packet)) {
        //serial_write_blocking("Mode 2 waiting\n", 0);
        //counter = 0;
        //SYSTICK_Cmd(DISABLE);
        //NVIC_DisableIRQ(EINT3_IRQn);
    };


}

void mode3() {
    char str[50];
    char key;
    int channel = 0;
    int update_lcd = 1;
    char channel_val;

    while (1) {
        if (update_lcd) {
            sprintf(str, "INPUT CHANNEL   %d", channel);
            lcd_write(str);
            update_lcd = 0;
        }

        key = kbd_get_key();

        if ((key >= '0') && (key <= '9')) {
            if (channel * 10 + key - 48 <= 512) {
                channel *= 10;
                channel += key - 48;
                update_lcd = 1;
            }
        }

        else if (key == 'D')
            break;

        else if (key == 'C') {
            channel = 0;
            update_lcd = 1;
        }

    }
    counter = 0;
    got_packet = 0;
    NVIC_EnableIRQ(EINT3_IRQn);
    GPIO_IntCmd(0, 0x20000, 0);
    NVIC_EnableIRQ(TIMER0_IRQn);

    while(!got_packet) {};

    channel_val = frames[channel];

    while (1) {
        counter = 0;
        got_packet = 0;
        NVIC_EnableIRQ(EINT3_IRQn);
        GPIO_IntCmd(0, 0x20000, 0);
        NVIC_EnableIRQ(TIMER0_IRQn);

        while (!got_packet) {};

        if (channel_val != frames[channel]) {
            display_captured_frame();
            break;
        }
    }

}

void int_to_usb(int a) {
    char str[20];
    sprintf(str, "%.3u ", a);
    serial_write_blocking(str, strlen(str));
}

void int_to_lcd(int a) {
    char str[10];
    int index;
    sprintf(str, "%.3u ", a);
    for (index = 0; index < strlen(str); index++) {
        lcd_buff[lcd_buff_index++] = lcd_key_lut[str[index]];
    }
}

//Gets called on interrupt (rising-edge) on GPIO pin 12
void EINT3_IRQHandler(void) {
    int frame_index = 0;
    int i = 1;
    GPIO_ClearInt(0, 0x20000);
    //NVIC_DisableIRQ(EINT3_IRQn);
    //GPIO_ClearInt(0, 0x20000);
    //Check if BREAK detected
    if (counter > 5) {
        NVIC_DisableIRQ(EINT3_IRQn);
        setLedInt(1);

        for (frame_index = 0; frame_index < 513; frame_index++) {
            i = 1;
            frame = 0;
            bits_read = 0;
            //Wait until after MAB
            while (i != 0) {
                i = GPIO_ReadValue(0) & (1 << 17);
            };
            //i = 5;
            //while (i > 0) {i--;} //delay the read

            TIM_ResetCounter(LPC_TIM0);
            TIM_Cmd(LPC_TIM0, ENABLE); //enable timer - generates interrupt every 4us
            //interrupt handler handles the reads
            while (bits_read < 8) {}; //wait until 8 bits have been read
            TIM_Cmd(LPC_TIM0, DISABLE); //disable timer
            frames[frame_index] = frame; //store frame in array

        }

        //**OUTPUT**
        char str[50];
        char temp[5];
        int offset = 0;
        char key;

        switch (mode) {


            case 1:

                strcpy(str, "");

                for (frame_index = 0 + offset; frame_index < 8 + offset; frame_index++) {
                    sprintf(temp, "%.3u ", frames[frame_index]);
                    strcat(str, temp);
                }

                lcd_write(str);
                break;

                lcd_write(str);

            case 2:
                display_captured_frame();

            case 3:
                break;

        }

        if (mode == 2) {
            for (frame_index = 0; frame_index < 24; frame_index++) {
                int_to_usb(frames[frame_index]);

            }
            setLedInt(4);
            serial_write_blocking("\n\n", 3);
        }

        got_packet = 1;
    }

    counter = 0;

}


//increments counter every 1ms - used to detect BREAK
void SysTick_Handler(void) {
    counter++;
}

//timer interrupt - handles reading each bit
//interrupt generated every 4 us
void TIMER0_IRQHandler(void) {
    //read value on GPIO port, mask it for pin 12, shift the bit to the correct position (LSB)
    frame += (GPIO_ReadValue(0) & (1 << 17)) >> (17 - bits_read);
    bits_read++;
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
}

void init() {
    serial_init();

    lcd_setup();

    // Port 0 Pin 17 as input

    GPIO_SetDir(0, 0x20000, 0);
    setLedInt(2);
    // LED as output

    GPIO_SetDir(1, 0xB40000, 1);


    // Enable interupt on rising edge for P0.17
    NVIC_EnableIRQ(EINT3_IRQn);
    GPIO_IntCmd(0, 0x20000, 0);

    SYSTICK_InternalInit(1);
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE);

    setLedInt(0);

    /*   PINSEL_CFG_Type PinCfg;

       PinCfg.Funcnum = 3;
       PinCfg.OpenDrain = 0;
       PinCfg.Pinmode = 0;
       PinCfg.Portnum = 1;
       PinCfg.Pinnum = 28;
       PINSEL_ConfigPin(&PinCfg); */

    TIM_TIMERCFG_Type tim_cfg;
    //TIM_ConfigStructInit(TIM_TIMER_MODE, &tim_cfg);
    tim_cfg.PrescaleOption = TIM_PRESCALE_USVAL;
    tim_cfg.PrescaleValue = 1;

    TIM_MATCHCFG_Type match_cfg;

    match_cfg.IntOnMatch = ENABLE;
    match_cfg.MatchChannel = 0;
    match_cfg.ResetOnMatch = ENABLE;
    match_cfg.StopOnMatch = DISABLE;
    match_cfg.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE;
    match_cfg.MatchValue = 3;

    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &tim_cfg);
    TIM_ConfigMatch(LPC_TIM0, &match_cfg);

    //   TIM_ResetCounter(LPC_TIM0);
    //   TIM_UpdateMatchValue(LPC_TIM0, 0, 10000);
    NVIC_SetPriority(EINT3_IRQn, 1);
    NVIC_SetPriority(TIMER0_IRQn, 0);
    NVIC_EnableIRQ(TIMER0_IRQn);
    //  TIM_Cmd(LPC_TIM0, ENABLE);
}

//deprecated
char read_frame(void) {
    char frame = 0;
    int counter = 200;
    int i;

    // Wait until after start bit
    //while (counter > 0)
    //    counter--;

    for (i = 17; i > 9; i--) {
        frame += (GPIO_ReadValue(0) & (1 << 17)) >> i;
        counter = 20;
        while (counter > 0)
            counter--;
    }
    return frame;
}


void display_captured_frame(void) {
    int frame_index;
    int offset = 0;
    char str[50];
    char temp[5];
    char key;

    for (frame_index = 0 + offset; frame_index < 8 + offset; frame_index++) {

        sprintf(temp, "%.3u ", frames[frame_index]);
        strcat(str, temp);

    }

    lcd_write(str);

    while (1) {
        key = kbd_get_key();
        //serial_write_blocking("EINT3 while loop\n", 0);

        strcpy(str, "");

        if (key == 'B') {
            offset += 8;
            for (frame_index = 0 + offset; frame_index < 8 + offset; frame_index++) {

                sprintf(temp, "%.3u ", frames[frame_index]);
                strcat(str, temp);

            }

            lcd_write(str);
        }
        if (key == 'A') {
            offset -= 8;
            for (frame_index = 0 + offset; frame_index < 8 + offset; frame_index++) {

                sprintf(temp, "%.3u ", frames[frame_index]);
                strcat(str, temp);

            }

            lcd_write(str);
        }


        if (key == '2')
            break;
    }
}

