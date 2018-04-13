//
// Created by dgc513 on 30/01/18.
//


#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "LPC17xx.h"
#include "string.h"

void kbd_read(void);
void kbd_write(void);
char kbd_get_key(void);

char kbd_buff[1];

char kbd_key_lut[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
};

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

char kbd_get_key() {
    int col = 0;
    int row = -1;
    for (col = 3; col >= 0; col--) {
        kbd_buff[0] = (0x01 << (7-col))  ^ 0xff;
        kbd_write();

        kbd_read();
        kbd_buff[0] &= 0x0f;

        switch(kbd_buff[0]) {
            case 0b00000111: row = 0; break;
            case 0b00001011: row = 1; break;
            case 0b00001101: row = 2; break;
            case 0b00001110: row = 3; break;
            case 0b00001111: row = -1; break;
        }

        if (row != -1) {
            while (1) {
                kbd_buff[0] = (0x01 << (7-col))  ^ 0xff;
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


    if ((col != -1) && (row != -1))
        return kbd_key_lut[row][col];

    return 0;
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