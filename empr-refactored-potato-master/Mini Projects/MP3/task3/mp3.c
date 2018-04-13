#include "lpc17xx_gpio.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "LPC17xx.h"
#include "lpc17xx_adc.h"
#include "serial.c"
#include "string.h"
#include <math.h>
#include <stdlib.h>

void adc_dac_init(void);

void main(){
  char str[100];
  uint16_t data;
  
  serial_init();
  write_usb_serial_blocking("Starting\n", 10);
  
  adc_dac_init();

  while (1) {
    ADC_StartCmd(LPC_ADC,ADC_START_NOW);  

    while (ADC_ChannelGetStatus(LPC_ADC, 1, 1) != SET) ;
        
    data = ADC_ChannelGetData(LPC_ADC, 1);
    data >>= 2;
    
    DAC_UpdateValue(LPC_DAC, data);

  }
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
