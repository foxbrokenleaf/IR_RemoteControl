#ifndef __OLED_GUI_H__
#define __OLED_GUI_H__

#include "OLED.h"
#include "OLED_Data.h"
#include "systick.h"
#include "gd32f10x_it.h"
#include "Keyboard/keyboard.h"
#include <stdio.h>

typedef enum GUI_UID{
    IR_SEND_UI = 0,
    IR_RECVIE_UI,
    MENU_UI,
    KEYBOARD_CHECK_UI,
    DISPLAY_CHECK_UI,
    IR_CHECK_UI,
    SETTING_UI
}gui_uid;

// 定义模式枚举
typedef enum {
    MODE_AUTO,    // 自动
    MODE_COOL,    // 制冷
    MODE_DRY,     // 除湿
    MODE_FAN,     // 送风
    MODE_HEAT,     // 制热
    MODE_NONE
} OperationMode;

// 定义扫风模式枚举
typedef enum {
    SWING_OFF,    // 关闭
    SWING_VERT,   // 上下扫风
    SWING_HORIZ,   // 左右扫风
    SWING_HORIZ_VERT,     // 上下&左右扫风
    SWING_NONE
} SwingMode;

// 定义扫风模式枚举
typedef enum {
    MUTE_OFF,    // 关闭
    MUTE_AUTO_MUTE,   // 自动 & 静音
    MUTE_MUTE,   // 静音
    MUTE_NONE
} MuteMode;

// 定义扫风模式枚举
typedef enum {
    HEALTH_OFF,    // 关闭
    HEALTH_H,   // 健康
    HEALTH_V,   // 换气
    HEALTH_HV, // 健康 & 换气
    HEALTH_NONE
} HealthMode;

typedef struct timer
{
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
}Timer;


// 当前模式变量
extern uint8_t Temperature;
extern OperationMode currentMode;
extern SwingMode currentSwing;
extern MuteMode currentMute;
extern HealthMode currentHealth;
extern gui_uid now_gui;
extern uint8_t FanSpeed;
extern uint8_t Super;
extern uint8_t Sleep;
extern uint8_t Light;
extern uint8_t Heat;
extern uint8_t Dry;
extern uint8_t Eco;
extern Timer timerNowTime;
extern Timer timerRingTime;

void GUI_Process(void);
void GUI_IRSendUI_Process(void);
void GUI_IRRecvUI_Process(void);
void GUI_MenuUI_Process(void);
void GUI_KeyboardCheckUI_Process(void);
void GUI_DisplayCheckUI_Process(void);
void GUI_IRCheckUI_Process(void);
void GUI_SettingUI_Process(void);

#endif