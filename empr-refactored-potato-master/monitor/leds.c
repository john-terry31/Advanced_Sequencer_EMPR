#include "lpc17xx_gpio.h"

void setLedInt(int number) {
    GPIO_ClearValue(1, 0xB40000);
    switch (number) {
        case 1:
            GPIO_SetValue(1, 0x800000);
            break;
        case 2:
            GPIO_SetValue(1, 0x200000);
            break;
        case 3:
            GPIO_SetValue(1, 0xA00000);
            break;
        case 4:
            GPIO_SetValue(1, 0x100000);
            break;
        case 5:
            GPIO_SetValue(1, 0x900000);
            break;
        case 6:
            GPIO_SetValue(1, 0x300000);
            break;
        case 7:
            GPIO_SetValue(1, 0xB00000);
            break;
        case 8:
            GPIO_SetValue(1, 0x40000);
            break;
        case 9:
            GPIO_SetValue(1, 0x840000);
            break;
        case 10:
            GPIO_SetValue(1, 0x240000);
            break;
        case 11:
            GPIO_SetValue(1, 0xA40000);
            break;
        case 12:
            GPIO_SetValue(1, 0x140000);
            break;
        case 13:
            GPIO_SetValue(1, 0x940000);
            break;
        case 14:
            GPIO_SetValue(1, 0x340000);
            break;
        case 15:
            GPIO_SetValue(1, 0xB40000);
            break;
    }
}