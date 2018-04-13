//
// Created by sls569 on 29/01/18.
//

#include "lcd.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "LPC17xx.h"
#include "leds.h"

char lcd_buff[30];
int lcd_buff_index = 0;
char lcd_key_lut[256];

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
    int index;

    i2c_init();
    I2C_Init(LPC_I2C1, 100000);
    I2C_Cmd(LPC_I2C1, ENABLE);


    lcd_buff[lcd_buff_index++] = 0b00000000;    // Control byte: write to IR
    lcd_buff[lcd_buff_index++] = 0b00110100;    // Function set 2 lines, basic instruction set
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


    lcd_clear();

    lcd_buff[lcd_buff_index++] = 0x00; // Control byte inst reg
    lcd_buff[lcd_buff_index++] = 0x80; // Set DDRAM address to 0

    lcd_send();

    lcd_key_lut[' '] = 0xa0;
    for (index = 0; index < 10; index++)
        lcd_key_lut['0'+index] = 0b10110000 + index;
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

void rebuild_key_lut() {
    int index;
    lcd_key_lut[' '] = 0xa0;
    for (index = 0; index < 10; index++)
        lcd_key_lut['0'+index] = 0b10110000 + index;

    for (index = 0; index < 26; index++)
        lcd_key_lut['A'+index] = 0b11000001 + index;
}

void lcd_line_2() {
    lcd_buff[lcd_buff_index++] = 0x00; // Control byte inst reg
    lcd_buff[lcd_buff_index++] = 0b11000000; // Set DDRAM address to 0x40
    lcd_send();
}

void lcd_line_1() {
    lcd_buff[lcd_buff_index++] = 0x00; // Control byte inst reg
    lcd_buff[lcd_buff_index++] = 0x80; // Set DDRAM address to 0
    lcd_send();
}

void lcd_write(char* str) {

    int i = 0;
    lcd_clear();

    rebuild_key_lut();

    lcd_buff[lcd_buff_index++] = 0x40;

 
    for(i=0; i<strlen(str); i++){
        lcd_buff[lcd_buff_index++] = lcd_key_lut[str[i]];

        if (i == 15) {
            lcd_send();
            lcd_line_2();
            lcd_buff[lcd_buff_index++] = 0x40;
        }
    }
    lcd_send();

}

