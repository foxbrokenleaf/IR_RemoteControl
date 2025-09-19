#ifndef __UI_H__
#define __UI_H__

#include "oled/OLED.h"
#include "oled/OLED_Data.h"
#include "gd32f10x_it.h"
#include "Keyboard/keyboard.h"
#include "IR/Sender/IR_Send.h"

typedef enum{
    UI_FUNCTION_LIST = 0,
    UI_INFO,
    UI_WIFI,
    UI_UPDATE,
}UI;

typedef enum{
    LOCATION_LEFT = 0,
    LOCATION_CENTER,
    LOCATION_RIGHT,
    LOCATION_NULL
}UI_LOCATION;

typedef struct{
    uint8_t Index;
    UI_LOCATION Location;
    const uint8_t *Image;
    uint8_t temp;
}UIIcon;

extern uint8_t index;
extern uint8_t BowknotFlashFlag;

void UpdateUIIconData(AirData *ad, KeyboardCode keyCode);
void UI_Process(KeyboardCode keyCode);

#endif