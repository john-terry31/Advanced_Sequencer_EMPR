#include "lpc17xx_gpio.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "LPC17xx.h"
#include "lpc17xx_pwm.h"
#include "lpc17xx_systick.h"
#include "serial.c"
#include "string.h"
#include <math.h>
#include <stdlib.h>

PWM_TIMERCFG_Type pwmconf;
PWM_MATCHCFG_Type PWMMatchCfgDat;
int counter = 5;

void pwm_init(void);

void main(){
  char str[100];
  uint16_t data;
  
  serial_init();
  write_usb_serial_blocking("Starting\n", 10);
  
  pwm_init();

  counter = 250;
  SYSTICK_InternalInit(100);
  SYSTICK_IntCmd(ENABLE);
  SYSTICK_Cmd(ENABLE);
}

void SysTick_Handler(void) {
  if ((counter-- % 5) == 0){
    PWM_MatchUpdate(LPC_PWM1, 6, 2*(counter % 50), PWM_MATCH_UPDATE_NOW);

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
