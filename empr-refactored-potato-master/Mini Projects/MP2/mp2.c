#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "LPC17xx.h"
#include "serial.c"
#include "string.h"

void setInt(int);
void i2c_init(void);
void lcd_setup(void);
void lcd_send(void);

char buff[30];
int buff_index = 0;

void main(void){
  char string[100];
  serial_init();
  write_usb_serial_blocking("Starting\n", 10);

  i2c_init();
  I2C_Init(LPC_I2C1, 100000);
  I2C_Cmd(LPC_I2C1, ENABLE);

  lcd_setup();
  
  buff[buff_index++] = 0x40; // Control byte data reg
  buff[buff_index++] = 0x64; // @ symbol

  lcd_send();
  
}

void lcd_setup() {
  buff[buff_index++] = 0b00000000;    // Control byte: write to IR
  buff[buff_index++] = 0b00110100;    // Function set 2 lines, basic insturction set
  buff[buff_index++] = 0b00001111;
  buff[buff_index++] = 0b00000110;    // Entry_mode_set
  buff[buff_index++] = 0x35; // Extended instruction set
  buff[buff_index++] = 0x04; // Screen conf
  buff[buff_index++] = 0x10;  // Standard instruction set
  buff[buff_index++] = 0x42; // Set CGRAM address
  buff[buff_index++] = 0x9f; // Set DDRAM address
  buff[buff_index++] = 0x34; // Standard instruction set
  buff[buff_index++] = 0x02; // Return home
  
  lcd_send();
  
  //buff[buff_index++] = 0x00; // Control byte inst reg
  //buff[buff_index++] = 0x01; // clr_scr

  //lcd_send();
  lcd_clear();

  //int sleep_counter = 10000000;
  //while (sleep_counter > 0) {
  //  sleep_counter--;
  //}

  buff[buff_index++] = 0x00; // Control byte inst reg
  buff[buff_index++] = 0x80; // Set DDRAM address to 0

  lcd_send();
}

void lcd_send() {
  I2C_M_SETUP_Type transferCfg;
  transferCfg.tx_data = buff;
  transferCfg.tx_length = buff_index;
  transferCfg.rx_data = NULL;
  transferCfg.rx_length = 0;

  transferCfg.sl_addr7bit = 0x3b;

  Status i2c_status = I2C_MasterTransferData(LPC_I2C1, &transferCfg, I2C_TRANSFER_POLLING);

  buff_index = 0;
  
}

void lcd_clear() {
  buff[0] = 0x40; // Control byte data reg
  
  for (buff_index = 1; buff_index < 80; buff_index++)
    buff[buff_index] = 0xa0;

  lcd_send();
}

  
//void SysTick_Handler(void){
//}
/*
void write_terminal(char* format, ){
  char  string[10];
  sprintf(string, i_string, addr)
    write_usb_serial_blocking(string, strlen(string));
    }*/

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


void setInt(int number)
{
  GPIO_ClearValue(1, 0xB40000);
  switch(number)
    {
    case 1: GPIO_SetValue(1, 0x800000);
      break;
    case 2: GPIO_SetValue(1, 0x200000);
      break;
    case 3: GPIO_SetValue(1, 0xA00000);
      break;
    case 4: GPIO_SetValue(1, 0x100000);
      break;
    case 5: GPIO_SetValue(1, 0x900000);
      break;
    case 6: GPIO_SetValue(1, 0x300000);
      break;
    case 7: GPIO_SetValue(1, 0xB00000);
      break;
    case 8: GPIO_SetValue(1, 0x40000);
      break;
    case 9: GPIO_SetValue(1, 0x840000);
      break;
    case 10: GPIO_SetValue(1, 0x240000);
      break;
    case 11: GPIO_SetValue(1, 0xA40000);
     break;
    case 12: GPIO_SetValue(1, 0x140000);
     break;
    case 13: GPIO_SetValue(1, 0x940000);
      break;
    case 14: GPIO_SetValue(1, 0x340000);
      break;
    case 15: GPIO_SetValue(1, 0xB40000);
      break;     
    }
}
