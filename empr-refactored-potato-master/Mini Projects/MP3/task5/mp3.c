#include <stdio.h>
#include "lpc17xx_gpio.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "LPC17xx.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_i2c.h"
#include "serial.c"
#include "string.h"

void i2c_init(void);

void lcd_setup(void);

void lcd_send(void);

void lcd_clear(void);

void kbd_read(void);

void kbd_write(void);

char kbd_get_key(void);


char buff[30];
int buff_index = 0;
char lcd_buff[30];
int lcd_buff_index = 0;

char kbd_key_lut[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
};

char kbd_buff[1];
char lcd_key_lut[256];

float distance_gradient = 16.173626;
float distance_offset = 0.370696;

void main() {
    int i;
    char str[100];

    FlagStatus status;
    uint16_t data = 10;
    float voltage;

    int index;
    char key;

    for (index = 0; index < 10; index++)
        lcd_key_lut['0' + index] = 0b10110000 + index;

    for (index = 0; index < 26; index++)
        lcd_key_lut['A' + index] = 0b11000001 + index;

    for (index = 0; index < 26; index++)
        lcd_key_lut['a' + index] = 0b01100001 + index;

    lcd_key_lut[46] = 0b10101110;
    //lcd_key_lut['V'] = 0b11010110;
    //lcd_key_lut[' '] = 0xa0;

    i2c_init();
    I2C_Init(LPC_I2C1, 100000);
    I2C_Cmd(LPC_I2C1, ENABLE);

    lcd_setup();

    serial_init();
    write_usb_serial_blocking("Starting\n", 10);

    PINSEL_CFG_Type PinCfg;

    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 24;
    PINSEL_ConfigPin(&PinCfg);

    ADC_Init(LPC_ADC, 10000);              // Set clock rate
    ADC_ChannelCmd(LPC_ADC, 1, ENABLE);    // Enable ADC

    while (1) {
        key = kbd_get_key();

        if (key == '1') {
            measure_distance();
        }

        else if (key == '2') {
            calibrate();
        }

    }
}

void calibrate() {
    int i, index;
    char str[100];
    char key;


    uint16_t data = 10;
    float v1, v2;

    write_usb_serial_blocking("Calibrating\n", strlen("Calibrating\n"));

    for (index = 0; index < 10; index++)
        lcd_key_lut['0' + index] = 0b10110000 + index;

    for (index = 0; index < 26; index++)
        lcd_key_lut['A' + index] = 0b11000001 + index;

    for (index = 0; index < 26; index++)
        lcd_key_lut['a' + index] = 0b01100001 + index;

    lcd_key_lut[46] = 0b10101110;

    sprintf(str, "Calibrate 10cm");
    lcd_buff[lcd_buff_index++] = 0x40;
    for (i = 0; i < strlen(str); i++)
        lcd_buff[lcd_buff_index++] = lcd_key_lut[str[i]];
    lcd_send();

    while (key != '2') {
        key = kbd_get_key();
    }

    ADC_StartCmd(LPC_ADC, ADC_START_NOW);

    while (ADC_ChannelGetStatus(LPC_ADC, 1, 1) != SET);

    data = ADC_ChannelGetData(LPC_ADC, 1);
    v1 = (float) data / 4095 * 3.3;

    write_usb_serial_blocking("Calibrating 30cm\n", strlen("Calibrating 30cm\n"));

    lcd_clear();

    for (index = 0; index < 10; index++)
        lcd_key_lut['0' + index] = 0b10110000 + index;

    for (index = 0; index < 26; index++)
        lcd_key_lut['A' + index] = 0b11000001 + index;

    for (index = 0; index < 26; index++)
        lcd_key_lut['a' + index] = 0b01100001 + index;

    lcd_key_lut[46] = 0b10101110;

    sprintf(str, "Calibrate 30cm");
    lcd_buff[lcd_buff_index++] = 0x40;
    for (i = 0; i < strlen(str); i++)
        lcd_buff[lcd_buff_index++] = lcd_key_lut[str[i]];
    lcd_send();

    key = 0;
    while (key != '2') {
        key = kbd_get_key();
    }

    ADC_StartCmd(LPC_ADC, ADC_START_NOW);

    while (ADC_ChannelGetStatus(LPC_ADC, 1, 1) != SET);

    data = ADC_ChannelGetData(LPC_ADC, 1);
    v2 = (float) data / 4095 * 3.3;

    distance_offset = (1.5 * v2) - (0.5 * v1);
    distance_gradient = 30 * v2 - 30 * distance_offset;

    sprintf(str, "V1: %f\nV2: %f\n", v1, v2);
    write_usb_serial_blocking(str, strlen(str));

    sprintf(str, "Offset  : %f\nGradient: %f\n", distance_offset, distance_gradient);
    write_usb_serial_blocking(str, strlen(str));

}

void measure_distance() {
    int index;
    int i;
    char str[100];

    FlagStatus status;
    uint16_t data = 10;
    float voltage, distance;


    while (1) {
        lcd_clear();
        ADC_StartCmd(LPC_ADC, ADC_START_NOW);

        while (ADC_ChannelGetStatus(LPC_ADC, 1, 1) != SET);

        data = ADC_ChannelGetData(LPC_ADC, 1);
        voltage = (float) data / 4095 * 3.3;

        distance = - (distance_gradient) / (distance_offset - voltage);

        lcd_buff[lcd_buff_index++] = 0x40;

        for (index = 0; index < 10; index++)
            lcd_key_lut['0'+index] = 0b10110000 + index;

        lcd_key_lut[46] = 0b10101110;
        lcd_key_lut['V'] = 0b11010110;

        sprintf(str, "%f V", voltage);
        for (i = 0; i < strlen(str); i++) {
            lcd_buff[lcd_buff_index++] = lcd_key_lut[str[i]];
        }
        //write_usb_serial_blocking("\n",1);
        lcd_send();

        lcd_buff[lcd_buff_index++] = 0x00; // Control byte inst reg
        lcd_buff[lcd_buff_index++] = 0b11000000; // Set DDRAM address to 0x40

        lcd_send();

        lcd_buff[lcd_buff_index++] = 0x40;
        sprintf(str, "%f cm\n", distance);
        for (i = 0; i < strlen(str); i++)
            lcd_buff[lcd_buff_index++] = lcd_key_lut[str[i]];

        lcd_send();

        write_usb_serial_blocking(str, strlen(str));

        i = 10000000;
        while (i > 0)
            i--;
        //lcd_key_lut[' '] = 0xa0;

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

void lcd_setup() {
  lcd_buff[lcd_buff_index++] = 0b00000000;    // Control byte: write to IR
  lcd_buff[lcd_buff_index++] = 0b00110100;    // Function set 2 lines, basic insturction set
  lcd_buff[lcd_buff_index++] = 0b00001111;
  lcd_buff[lcd_buff_index++] = 0b00000110;    // Entry_mode_set
  lcd_buff[lcd_buff_index++] = 0x35; // Extended instruction set
  lcd_buff[lcd_buff_index++] = 0x04; // Screen conf
  lcd_buff[lcd_buff_index++] = 0x10;  // Standard instruction set
  lcd_buff[lcd_buff_index++] = 0x42; // Set CGRAM address
  lcd_buff[lcd_buff_index++] = 0x9f; // Set DDRAM address
  lcd_buff[lcd_buff_index++] = 0x34; // Standard instruction set
  lcd_buff[lcd_buff_index++] = 0x02; // Return home

  lcd_send();

  //lcd_buff[lcd_buff_index++] = 0x00; // Control byte inst reg
  //lcd_buff[lcd_buff_index++] = 0x01; // clr_scr

  //lcd_send();
  lcd_clear();

  //int sleep_counter = 10000000;
  //while (sleep_counter > 0) {
  //  sleep_counter--;
  //}

  lcd_buff[lcd_buff_index++] = 0x00; // Control byte inst reg
  lcd_buff[lcd_buff_index++] = 0x80; // Set DDRAM address to 0

  lcd_send();
}

void lcd_send() {
  I2C_M_SETUP_Type transferCfg;
  transferCfg.tx_data = lcd_buff;
  transferCfg.tx_length = lcd_buff_index;
  transferCfg.rx_data = NULL;
  transferCfg.rx_length = 0;

  transferCfg.sl_addr7bit = 0x3b;

  Status i2c_status = I2C_MasterTransferData(LPC_I2C1, &transferCfg, I2C_TRANSFER_POLLING);

  lcd_buff_index = 0;

}

void lcd_clear() {
  lcd_buff[0] = 0x40; // Control byte data reg

  for (lcd_buff_index = 1; lcd_buff_index < 0x80; lcd_buff_index++)
    lcd_buff[lcd_buff_index] = 0xa0;

  lcd_send();

  lcd_buff_index = 0;

  lcd_buff[lcd_buff_index++] = 0x00; // Control byte: Instruction reg
  lcd_buff[lcd_buff_index++] = 0x02; // Return home

  lcd_send();

  lcd_buff_index = 0;
}

char kbd_get_key() {
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
        return kbd_key_lut[row][col];

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

