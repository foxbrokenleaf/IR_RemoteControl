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

// ����ģʽö��
typedef enum {
    MODE_AUTO,    // �Զ�
    MODE_COOL,    // ����
    MODE_DRY,     // ��ʪ
    MODE_FAN,     // �ͷ�
    MODE_HEAT,     // ����
    MODE_NONE
} OperationMode;

// ����ɨ��ģʽö��
typedef enum {
    SWING_OFF,    // �ر�
    SWING_VERT,   // ����ɨ��
    SWING_HORIZ,   // ����ɨ��
    SWING_HORIZ_VERT,     // ����&����ɨ��
    SWING_NONE
} SwingMode;

// ����ɨ��ģʽö��
typedef enum {
    MUTE_OFF,    // �ر�
    MUTE_AUTO_MUTE,   // �Զ� & ����
    MUTE_MUTE,   // ����
    MUTE_NONE
} MuteMode;

// ����ɨ��ģʽö��
typedef enum {
    HEALTH_OFF,    // �ر�
    HEALTH_H,   // ����
    HEALTH_V,   // ����
    HEALTH_HV, // ���� & ����
    HEALTH_NONE
} HealthMode;

typedef struct timer
{
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
}Timer;


// ��ǰģʽ����
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