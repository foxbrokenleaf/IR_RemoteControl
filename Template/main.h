#ifndef MAIN_H
#define MAIN_H

#include "gd32f10x.h"
#include "systick.h"
#include "oled/OLED.h"
#include "oled/OLED_Data.h"
#include "oled/gui.h"
#include "IR/Sender/IR_Send.h"
#include "Keyboard/keyboard.h"
#include <stdio.h>

extern uint32_t systemtick;

void usart_config(void);

#endif /* MAIN_H */
