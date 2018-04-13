#include "lpc17xx_gpio.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "LPC17xx.h"
#include "lpc17xx_pwm.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_i2c.h"
#include "serial.c"
#include "string.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

PWM_TIMERCFG_Type pwmconf;
PWM_MATCHCFG_Type PWMMatchCfgDat;
int counter = 5;

double value;
unsigned int loop, loop2;
int wave_values[1000];
int timer;
int cont = 1;
char kbd_buff[1];

void pwm_init(void);
void sine_wave(float, float);

void main() {
    char str[100];
    float ampl, freq;
    int i;
    uint16_t data;

    serial_init();
    write_usb_serial_blocking("Starting\n", 10);

    adc_dac_init();
    i2c_init();
    I2C_Init(LPC_I2C1, 100000);
    I2C_Cmd(LPC_I2C1, ENABLE);


    while (cont) {
        ampl = 3.3 * ((float) rand() / (float) RAND_MAX);
        freq = 11 * ((float) rand() / (float) RAND_MAX);
        sprintf(str, "Ampl: %f    Freq: %f\n", ampl, freq);
        write_usb_serial_blocking(str, strlen(str));
        sine_wave(ampl, freq);
        cont = !(kbd_get_key());
    }

    cont = 1;

    write_usb_serial_blocking("End T2\n", 7);

    i = 1000000;
    while (i > 0) i--;

    write_usb_serial_blocking("Starting T3\n", 13);

    while (cont) {
        for (i = 1000; i > 0; i--) {

            ADC_StartCmd(LPC_ADC, ADC_START_NOW);

            while (ADC_ChannelGetStatus(LPC_ADC, 1, 1) != SET);

            data = ADC_ChannelGetData(LPC_ADC, 1);
            data >>= 2;

            DAC_UpdateValue(LPC_DAC, data);
        }
        cont = !(kbd_get_key());
    }

    write_usb_serial_blocking("End T3\n", 7);

    write_usb_serial_blocking("Starting T4\n", 13);

    pwm_init();

    counter = 250;
    SYSTICK_InternalInit(100);
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE);
}

void SysTick_Handler(void) {
    if ((counter-- % 5) == 0) {
        PWM_MatchUpdate(LPC_PWM1, 6, 2 * (counter % 50), PWM_MATCH_UPDATE_NOW);

    }
}

void pwm_init() {
    PINSEL_CFG_Type PinCfg;

    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 2;
    PinCfg.Pinnum = 5;
    PINSEL_ConfigPin(&PinCfg);

    pwmconf.PrescaleOption = PWM_TIMER_PRESCALE_TICKVAL;
    pwmconf.PrescaleValue = 1;

    PWM_Init(LPC_PWM1, PWM_MODE_TIMER, &pwmconf);
    PWM_ChannelConfig(LPC_PWM1, 6, PWM_CHANNEL_SINGLE_EDGE);

    PWM_MatchUpdate(LPC_PWM1, 0, 100, PWM_MATCH_UPDATE_NOW);

    PWMMatchCfgDat.IntOnMatch = DISABLE;
    PWMMatchCfgDat.MatchChannel = 0;
    PWMMatchCfgDat.ResetOnMatch = ENABLE;
    PWMMatchCfgDat.StopOnMatch = DISABLE;
    PWM_ConfigMatch(LPC_PWM1, &PWMMatchCfgDat);

    PWM_MatchUpdate(LPC_PWM1, 6, 50, PWM_MATCH_UPDATE_NOW);
    PWMMatchCfgDat.IntOnMatch = DISABLE;
    PWMMatchCfgDat.MatchChannel = 6;
    PWMMatchCfgDat.ResetOnMatch = DISABLE;
    PWMMatchCfgDat.StopOnMatch = DISABLE;
    PWM_ConfigMatch(LPC_PWM1, &PWMMatchCfgDat);

    PWM_ChannelCmd(LPC_PWM1, 6, ENABLE);

    PWM_ResetCounter(LPC_PWM1);
    PWM_CounterCmd(LPC_PWM1, ENABLE);


    PWM_Cmd(LPC_PWM1, ENABLE);
}

void adc_dac_init() {
    PINSEL_CFG_Type PinCfg;

    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 26;
    PINSEL_ConfigPin(&PinCfg);

    DAC_Init(LPC_DAC);

    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 24;
    PINSEL_ConfigPin(&PinCfg);

    ADC_Init(LPC_ADC, 10000);              // Set clock rate
    ADC_ChannelCmd(LPC_ADC, 1, ENABLE);    // Enable ADC
}

void sine_wave(float ampl, float freq) {
    loop = 0;
    value = 0;

    while (value < 2 * 3.141592653589) {
        wave_values
[loop] = (ampl * 512.0 / 3.5) * sin(value) + (ampl * 512.0 / 3.5);
        value += (freq / 105.0);
        loop++;
    }

    loop2 = 0;
    timer = 2000000;

    while (timer-- > 0) {
        /*
        value = (double)loop;
        value = 511.0 * sin(value / 10) + 512;
        DAC_UpdateValue(LPC_DAC, (int)value);
        loop++; */
        DAC_UpdateValue(LPC_DAC, wave_values[loop2++]);
        loop2 = loop2 % loop;
    }
}

void i2c_init(void) {
    PINSEL_CFG_Type PinCfg;

    PinCfg.Funcnum = 3;
    PinCfg.OpenDrain = 1;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum++;
    PINSEL_ConfigPin(&PinCfg);

}

int kbd_get_key() {
    int col = 0;
    int row = -1;
    for (col = 3; col >= 0; col--) {
        kbd_buff[0] = (0x01 << (7 - col)) ^ 0xff;
        kbd_write();

        kbd_read();
        kbd_buff[0] &= 0x0f;

        switch (kbd_buff[0]) {
            case 0b00000111:
                row = 0;
                break;
            case 0b00001011:
                row = 1;
                break;
            case 0b00001101:
                row = 2;
                break;
            case 0b00001110:
                row = 3;
                break;
            case 0b00001111:
                row = -1;
                break;
        }

        if (row != -1) {
            while (1) {
                kbd_buff[0] = (0x01 << (7 - col)) ^ 0xff;
                kbd_write();

                kbd_read();
                kbd_buff[0] &= 0x0f;

                kbd_buff[0] &= 0x0f;
                kbd_buff[0] &= 0x01 << (3 - row);

                if (kbd_buff[0] > 0)
                    break;
            }
            break;
        }

    }

    //char string[30];
    // sprintf(string, "Row: %2i Col: %2i\n", row, col);
    //write_usb_serial_blocking(string, strlen(string));

    if ((col != -1) && (row != -1))
        return 1;

    return 0;
}

void kbd_read() {
    I2C_M_SETUP_Type transferCfg;
    transferCfg.tx_data = NULL;
    transferCfg.tx_length = 0;
    transferCfg.rx_data = kbd_buff;
    transferCfg.rx_length = 1;

    kbd_buff[0] = 0xff;

    transferCfg.sl_addr7bit = 0x21;

    Status i2c_status = I2C_MasterTransferData(LPC_I2C1, &transferCfg, I2C_TRANSFER_POLLING);

    char string[10];

    //sprintf(string, ""BYTE_TO_BINARY_PATTERN"\n",  BYTE_TO_BINARY(kbd_buff[0]));
    //write_usb_serial_blocking(string, 10);

}

void kbd_write() {
    I2C_M_SETUP_Type transferCfg;
    transferCfg.tx_data = kbd_buff;
    transferCfg.tx_length = 1;
    transferCfg.rx_data = NULL;
    transferCfg.rx_length = 0;

    transferCfg.sl_addr7bit = 0x21;

    Status i2c_status = I2C_MasterTransferData(LPC_I2C1, &transferCfg, I2C_TRANSFER_POLLING);
}