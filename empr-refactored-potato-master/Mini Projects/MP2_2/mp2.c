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
#include "serial.c"
#include "string.h"
#include <stdlib.h>

void setInt(int);
void i2c_init(void);
void lcd_setup(void);
void lcd_send(void);
void kbd_test(void);
void kbd_read(void);
void kbd_write(void);
char kbd_get_key(void);

char lcd_buff[30];
int lcd_buff_index = 0;

char kbd_buff[1];

char kbd_key_lut[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

  
char lcd_key_lut[256];


void main(void){
  char string[100];
  serial_init();
  write_usb_serial_blocking("Starting\n", 10);

  i2c_init();
  I2C_Init(LPC_I2C1, 100000);
  I2C_Cmd(LPC_I2C1, ENABLE);

  lcd_setup();
  
  lcd_buff[lcd_buff_index++] = 0x40; // Control byte data reg
  lcd_buff[lcd_buff_index++] = 0b11001000; // H
  lcd_buff[lcd_buff_index++] = 0b11100101; // e
  lcd_buff[lcd_buff_index++] = 0b11101100; // l
  lcd_buff[lcd_buff_index++] = 0b11101100; // l
  lcd_buff[lcd_buff_index++] = 0b11101111; // o

  lcd_send();

  lcd_buff[lcd_buff_index++] = 0x00; // Control byte inst reg
//  lcd_buff[lcd_buff_index++] = 0b10110001; // Set DDRAM address to 41
  lcd_buff[lcd_buff_index++] = 0b11000000; // Set DDRAM address to 0x40

  lcd_send();

  lcd_buff[lcd_buff_index++] = 0x40; // Control byte data reg
  lcd_buff[lcd_buff_index++] = 0b11110111; // W
  lcd_buff[lcd_buff_index++] = 0b11101111; // o
  lcd_buff[lcd_buff_index++] = 0b11110010; // r
  lcd_buff[lcd_buff_index++] = 0b11101100; // l
  lcd_buff[lcd_buff_index++] = 0b11100100; // d 

  lcd_send();
  
  int sleep_counter = 10000000;
  while (sleep_counter > 0) {
    sleep_counter--;
  }

  lcd_clear(); 

  // Keyboard

  char key;

  int index;
  for (index = 0; index < 10; index++)  
    lcd_key_lut['0'+index] = 0b10110000 + index;

  lcd_key_lut['A'] = 0b10101011;
  lcd_key_lut['B'] = 0b10101101;
  lcd_key_lut['C'] = 0b10101010;
  lcd_key_lut['D'] = 0b10101111;
  lcd_key_lut['#'] = 0b10111101;
  
  kbd_test();

  int res;
  char output[20];
  int num[2];
  num[0] = 0;
  num[1] = 1;
  int num_index = 0;
  char op;
  
  while (1) {
    key = kbd_get_key();

    if (key != 0) {
      lcd_buff[lcd_buff_index++] = 0x40; // Control byte: Data
      lcd_buff[lcd_buff_index++] = lcd_key_lut[key];
      lcd_send();
      
      // Check if key is digit
      if( (key >= 0b10110000) && (key <= (0b10110000 + 10)) ) {
	num[num_index] *= 10;
	num[num_index] += atoi(key);
      }

      // Equals
      else if (key == '#') {
        switch (op) {
	case 'A': res = num[0] + num[1]; break;
	case 'B': res = num[0] - num[1]; break;
	case 'C': res = num[0] * num[1]; break;
	case 'D': res = num[0] / num[1]; break;
	}
	
        
	lcd_buff[lcd_buff_index++] = 0x00; // Control byte inst reg
        lcd_buff[lcd_buff_index++] = 0b11000000; // Set DDRAM address to 0x40

	lcd_send();

	lcd_buff[lcd_buff_index++] = 0x40; // Control byte: Data
	sprintf(lcd_buff+1, "%i", res);
	
	sprintf(output, "%i", res); // Just to get length
	lcd_buff_index += strlen(output);

	lcd_send();
	
      }

      // If operator (but not equal)
      else {
	num_index += 1;
	op = key;
      }
      
    }
    
    int sleep_counter = 100000;
    while (sleep_counter > 0) {
      sleep_counter--;
    }
  }
}


void kbd_test() {
  I2C_M_SETUP_Type transferCfg;
  transferCfg.tx_data = kbd_buff;
  transferCfg.tx_length = 1;
  transferCfg.rx_data = NULL;
  transferCfg.rx_length = 0;

  transferCfg.sl_addr7bit = 0x21;

  Status i2c_status = I2C_MasterTransferData(LPC_I2C1, &transferCfg, I2C_TRANSFER_POLLING);

  if (i2c_status == SUCCESS)
    write_usb_serial_blocking("OK\n", 4);
  else
    write_usb_serial_blocking("Fail\n", 6);

  kbd_buff[0] = 0x7f;
  kbd_write();

  kbd_read();
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

  char string[30];
  sprintf(string, "Row: %2i Col: %2i\n", row, col);
  write_usb_serial_blocking(string, strlen(string));

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
