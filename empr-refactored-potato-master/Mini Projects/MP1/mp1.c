#define BYTE_TO_BINARY_PATTERN "%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')


#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"
#include "lpc_types.h"
#include "LPC17xx.h"
#include "serial.c"

void setInt(int);

int led_value = 0;
int counter = 10;


void main(void){
  serial_init();
  write_usb_serial_blocking("Starting count\n", 16);
  GPIO_SetDir(1, 0xB40000, 1);

  setInt(0);
  SYSTICK_InternalInit(100);
  SYSTICK_IntCmd(ENABLE);
  SYSTICK_Cmd(ENABLE);
}


void SysTick_Handler(void){
  char string[30]; 
  
  counter--;
  
  if (counter == 0) {
    led_value++;
    
    if (led_value == 16) {
      write_usb_serial_blocking("Finished count\n", 16);
      while (1) {}
    }
    
    setInt(led_value);

    sprintf(string, "Dec: %2d    Hex: %1X    Bin: ", led_value, led_value);
    write_usb_serial_blocking(string, 27);
    sprintf(string, ""BYTE_TO_BINARY_PATTERN"\n",  BYTE_TO_BINARY(led_value));
    write_usb_serial_blocking(string, 6);
    counter = 10;
  }
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
