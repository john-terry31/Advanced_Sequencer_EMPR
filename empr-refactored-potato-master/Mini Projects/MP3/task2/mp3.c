#include "lpc17xx_gpio.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "LPC17xx.h"
#include "lpc17xx_adc.h"
#include "serial.c"
#include "string.h"
#include <math.h>
#include <stdlib.h>

void sine_wave(float, float);

double value;
unsigned int loop, loop2;
int wave_values[1000];
int timer;

void main(){
  char str[100];
  float ampl, freq;
  
  serial_init();
  write_usb_serial_blocking("Starting\n", 10);
  
  PINSEL_CFG_Type PinCfg;

  PinCfg.Funcnum = 2;
  PinCfg.OpenDrain = 0;
  PinCfg.Pinmode = 0;
  PinCfg.Portnum = 0;
  PinCfg.Pinnum = 26;
  PINSEL_ConfigPin(&PinCfg);

  DAC_Init(LPC_DAC);

  while (1) {
    ampl = 3.3 * ((float)rand() / (float)RAND_MAX);
    freq = 11 * ((float)rand() / (float)RAND_MAX);
    sprintf(str, "Ampl: %f    Freq: %f\n", ampl, freq);
    write_usb_serial_blocking(str, strlen(str));
    sine_wave(ampl, freq);
  }

}

void sine_wave(float ampl, float freq){
  loop= 0;
  value = 0;
  
  while (value < 2*3.141592653589) {
    wave_values[loop] = (ampl * 512.0 / 3.5) * sin(value) + (ampl * 512.0 / 3.5);
    value += (freq / 105.0);
    loop++;
  }

  loop2 = 0;
  timer = 5000000;
  
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
