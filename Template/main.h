#ifndef MAIN_H
#define MAIN_H

#include "gd32f10x.h"
#include "gd32f10x_it.h"
#include "gd32f10x_rcu.h"
#include "systick.h"
#include "oled/OLED.h"
#include "oled/OLED_Data.h"
#include "oled/ui.h"
#include "IR/Sender/IR_Send.h"
#include "IR/Recvier/IR_Recvier.h"
#include "Keyboard/keyboard.h"
#include <stdio.h>

extern uint32_t systemtick;

void usart_config(void);
void timer2_config(void);
void timer3_config(void);

KeyboardCode KeyProcess();

#endif /* MAIN_H */
