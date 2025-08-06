#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "gd32f10x.h"
#include "systick.h"

#define KEYBOARD_PRESS_V SET

#define KEY_1_2_3_GROUP GPIOA
#define KEY_1 GPIO_PIN_10
#define KEY_2 GPIO_PIN_9
#define KEY_3 GPIO_PIN_8
#define KEY_4_5_GROUP GPIOC
#define KEY_4 GPIO_PIN_9
#define KEY_5 GPIO_PIN_8

typedef enum KEY_CODE{
    UNPRESS = 0,
    KEY_CODE_U,
    KEY_CODE_R,
    KEY_CODE_D,
    KEY_CODE_L,
    KEY_CODE_O
}KeyboardCode;

void Keyboard_Init();
KeyboardCode GetKeyboardCode(void);

#endif