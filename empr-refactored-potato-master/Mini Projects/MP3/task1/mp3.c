#include "lpc17xx_gpio.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "LPC17xx.h"
#include "lpc17xx_adc.h"
#include "serial.c"
#include "string.h"

void main(){
  int i;
  char str[100];
  
  FlagStatus status;
  uint16_t data = 10;
  float voltage;

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
    ADC_StartCmd(LPC_ADC,ADC_START_NOW);  

    while (ADC_ChannelGetStatus(LPC_ADC, 1, 1) != SET) ;
        
    data = ADC_ChannelGetData(LPC_ADC, 1);
    voltage = (float)data / 4095 * 3.3;
    
    sprintf(str, "%f V\n", voltage);
    write_usb_serial_blocking(str, strlen(str));
    
    i = 1000000;
    while (i > 0)
      i--;

  }
  
}
