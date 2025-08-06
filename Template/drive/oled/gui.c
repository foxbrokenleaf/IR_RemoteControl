#include "gui.h"

gui_uid now_gui = KEYBOARD_CHECK_UI;
// 当前运行模式
OperationMode currentMode = MODE_AUTO;
// 当前扫风模式
SwingMode currentSwing = SWING_OFF;
MuteMode currentMute = MUTE_OFF;
HealthMode currentHealth = HEALTH_OFF;
uint8_t FanSpeed = 0;
uint8_t Temperature = 0;
uint8_t Sleep = 0;
uint8_t Light = 0;
uint8_t Heat = 0;
uint8_t Super = 0;
uint8_t Dry = 0;
uint8_t Eco = 0;
Timer timerNowTime;
Timer timerRingTime;

void GUI_IRSendUI_Process(void) {
    OLED_Clear();

    OLED_DrawLine(0, 20, 69, 20);
    OLED_DrawLine(69, 0, 69, 20);
    OLED_DrawLine(0, 9, 128, 9);
    OLED_DrawLine(0, 20, 128, 20);
    OLED_DrawLine(0, 31, 128, 31);
    OLED_DrawLine(69, 9, 69, 54);    
    OLED_DrawLine(0, 42, 128, 42);
    OLED_DrawLine(0, 54, 128, 54);

    // 温度（除了自动都有）
    if(currentMode != MODE_AUTO){
        OLED_ShowString(0, 0, "Temp:", OLED_6X8);
        if(Temperature > 30) Temperature = 0;
        OLED_ShowNum(30, 0, Temperature, 2, OLED_6X8); // 假设温度26
        OLED_ShowString(45, 0, "C", OLED_6X8);
        OLED_DrawPoint(43, 0);
        OLED_DrawPoint(44, 0);
        OLED_DrawPoint(43, 1);
        OLED_DrawPoint(44, 1);
    }


    // 运行模式
    if(currentMode == MODE_NONE) currentMode = MODE_AUTO;
    OLED_ShowString(72, 0, "Mode:    ", OLED_6X8);
    switch(currentMode) {
        case MODE_AUTO:
            OLED_ShowString(102, 0, "Auto", OLED_6X8);
            break;
        case MODE_COOL:
            OLED_ShowString(102, 0, "Cool", OLED_6X8);
            break;
        case MODE_DRY:
            OLED_ShowString(102, 0, "Dry ", OLED_6X8);
            break;
        case MODE_FAN:
            OLED_ShowString(102, 0, "Fan ", OLED_6X8);
            break;
        case MODE_HEAT:
            OLED_ShowString(102, 0, "Heat", OLED_6X8);
            break;
    }    

    // 风速（都有）
    OLED_ShowString(72, 11, "Fan:", OLED_6X8);
    if(FanSpeed > 0 && FanSpeed <= 5) OLED_ShowNum(96, 11, FanSpeed, 1, OLED_6X8);
    else{
        OLED_ShowString(96, 11, "Auto", OLED_6X8);
        FanSpeed = 0;
    }
    
    //  超强（制冷、制热
    if(currentMode == MODE_COOL || currentMode == MODE_HEAT){
        OLED_ShowString(0, 11, "Super:", OLED_6X8);
        if(Super > 0 && Super <= 1) OLED_ShowString(36, 11, "ON", OLED_6X8);
        else{
            OLED_ShowString(36, 11, "OFF", OLED_6X8);
            Super = 0;
        }
    }

    // 显示当前扫风模式（都有）
    if(currentSwing == SWING_NONE) currentSwing = SWING_OFF;
    OLED_ShowString(72, 22, "Swing:", OLED_6X8);
    switch(currentSwing) {
        case SWING_OFF:
            OLED_ShowString(108, 22, "OFF", OLED_6X8);
            break;
        case SWING_VERT:
            OLED_ShowString(108, 22, "V  ", OLED_6X8); // 上下
            break;
        case SWING_HORIZ:
            OLED_ShowString(108, 22, "H  ", OLED_6X8); // 左右
            break;
        case SWING_HORIZ_VERT:
            OLED_ShowString(108, 22, "V&H  ", OLED_6X8); // 上下&左右
            break;
    }
    

    // 睡眠(制冷、除湿、制热)
    if(currentMode == MODE_COOL || currentMode == MODE_DRY || currentMode == MODE_HEAT){    
        OLED_ShowString(72, 33, "Sleep:", OLED_6X8);
        if(Sleep > 0 && Sleep <= 3) OLED_ShowNum(108, 33, Sleep, 1, OLED_6X8);
        else{
            Sleep = 0;
            OLED_ShowString(108, 33, "OFF", OLED_6X8);
        }
    }

    //灯光状态（都有）
    OLED_ShowString(72, 45, "Light:", OLED_6X8);
    if(Light > 0 && Light <= 1) OLED_ShowString(108, 45, "ON", OLED_6X8);
    else {
        OLED_ShowString(108, 45, "OFF", OLED_6X8);
        Light = 0;
    }

    // 静音（都有）
    OLED_ShowString(0, 22, "Mute:", OLED_6X8);
    if(currentMute == MUTE_NONE) currentMute = MUTE_OFF;
    if(currentMute == MUTE_MUTE) OLED_ShowString(30, 22, "M  ", OLED_6X8);
    if(currentMute == MUTE_OFF) OLED_ShowString(30, 22, "OFF", OLED_6X8);
    if(currentMute == MUTE_AUTO_MUTE)  OLED_ShowString(30, 22, "A&M", OLED_6X8);
    
    // 干燥（制冷、除湿）
    if(currentMode == MODE_COOL || currentMode == MODE_DRY){
        OLED_ShowString(0, 33, "Dry:", OLED_6X8);
        if(Dry > 0 && Dry <= 1) OLED_ShowString(24, 33, "ON", OLED_6X8);
        else{
            OLED_ShowString(24, 33, "OFF", OLED_6X8);
            Dry = 0;
        }        
    }
    else if(currentMode == MODE_HEAT){
        // 辅热（制热）
        OLED_ShowString(0, 33, "Heat:", OLED_6X8);
        if(Heat > 0 && Heat <= 1) OLED_ShowString(30, 33, "ON", OLED_6X8);
        else {
            OLED_ShowString(30, 33, "OFF", OLED_6X8);
            Heat = 0;
        }
    }

    // 健康/换气（都有）
    OLED_ShowString(0, 45, "Health:", OLED_6X8);
    if(currentHealth == HEALTH_NONE) currentHealth = HEALTH_OFF;
    switch (currentHealth)
    {
    case HEALTH_OFF:
        OLED_ShowString(42, 45, "OFF", OLED_6X8);
        break;
    case HEALTH_H:
        OLED_ShowString(42, 45, "H", OLED_6X8);      // 健康
        break;
    case HEALTH_V:
        OLED_ShowString(42, 45, "V", OLED_6X8);      // 换气
        break;
    case HEALTH_HV:
        OLED_ShowString(42, 45, "H&V", OLED_6X8);    // 健康&换气
        break;
    }

    // 定时关闭（都有）
    OLED_ShowString(0, 56, "Tmr:", OLED_6X8);
    OLED_ShowNum(24, 56, timerRingTime.day, 2, OLED_6X8);
    OLED_ShowChar(36, 56, '-', OLED_6X8);
    OLED_ShowNum(42, 56, timerRingTime.hour, 2, OLED_6X8);
    OLED_ShowChar(54, 56, ':', OLED_6X8);
    OLED_ShowNum(60, 56, timerRingTime.min, 2, OLED_6X8);
    // OLED_ShowChar(88, 56, ':', OLED_6X8);
    // OLED_ShowNum(96, 56, timerRingTime.min, 2, OLED_6X8);
    

    // 节能（制冷）
    if(currentMode == MODE_COOL){
        OLED_ShowString(86, 56, "Eco:", OLED_6X8);
        if(Eco > 0 && Eco <= 1) OLED_ShowString(106, 56, "ON", OLED_6X8);
        else{
            OLED_ShowString(106, 56, "OFF", OLED_6X8);
            Eco = 0;
        }        
    }


    OLED_Update();
}

void GUI_IRRecvUI_Process(void) {
    // 红外接收界面处理
    // TODO: 实现红外接收界面显示与逻辑
}

void GUI_MenuUI_Process(void) {
    // 菜单界面处理
    // TODO: 实现菜单界面显示与逻辑
}

void GUI_KeyboardCheckUI_Process(void) {

    KeyboardCode key_res = GetKeyboardCode();
    static uint8_t KeyDown_Counter = 0;
    if(key_res != 0) KeyDown_Counter++;

    OLED_Clear();

    // 按钮尺寸
    const uint8_t btn_w = 18, btn_h = 12;
    // 屏幕中心
    const uint8_t cx = 64;

    // 标题居中
    OLED_ShowString((128 - 12 * 6) / 2, 0, "Keyboard Test", OLED_6X8);

    // 分割线
    OLED_DrawLine(0, 12, 128, 12);

    // 上按钮
    uint8_t up_x = cx - btn_w / 2;
    uint8_t up_y = 16;
    OLED_DrawRectangle(up_x, up_y, btn_w, btn_h, (key_res == KEY_CODE_U) ? OLED_FILLED : OLED_UNFILLED);
    OLED_ShowString(up_x + (btn_w - 6) / 2, up_y + (btn_h - 8) / 2, "U", OLED_6X8);

    // 中间按钮
    uint8_t ok_x = cx - btn_w / 2;
    uint8_t ok_y = up_y + btn_h + 4;
    OLED_DrawRectangle(ok_x, ok_y, btn_w, btn_h, (key_res == KEY_CODE_O) ? OLED_FILLED : OLED_UNFILLED);
    OLED_ShowString(ok_x + (btn_w - 6) / 2, ok_y + (btn_h - 8) / 2, "O", OLED_6X8);

    // 下按钮
    uint8_t down_x = cx - btn_w / 2;
    uint8_t down_y = ok_y + btn_h + 4;
    OLED_DrawRectangle(down_x, down_y, btn_w, btn_h, (key_res == KEY_CODE_D) ? OLED_FILLED : OLED_UNFILLED);
    OLED_ShowString(down_x + (btn_w - 6) / 2, down_y + (btn_h - 8) / 2, "D", OLED_6X8);

    // 左按钮
    uint8_t left_x = cx - 22 - btn_w / 2;
    uint8_t left_y = ok_y;
    OLED_DrawRectangle(left_x, left_y, btn_w, btn_h, (key_res == KEY_CODE_L) ? OLED_FILLED : OLED_UNFILLED);
    OLED_ShowString(left_x + (btn_w - 6) / 2, left_y + (btn_h - 8) / 2, "L", OLED_6X8);

    // 右按钮
    uint8_t right_x = cx + 22 - btn_w / 2;
    uint8_t right_y = ok_y;
    OLED_DrawRectangle(right_x, right_y, btn_w, btn_h, (key_res == KEY_CODE_R) ? OLED_FILLED : OLED_UNFILLED);
    OLED_ShowString(right_x + (btn_w - 6) / 2, right_y + (btn_h - 8) / 2, "R", OLED_6X8);

    OLED_ShowNum(0, 64 - 8, KeyDown_Counter, 2, OLED_6X8);

    OLED_Update();
}

void GUI_DisplayCheckUI_Process(void) {
    // 显示检测界面处理
    // TODO: 实现显示检测界面显示与逻辑
}

void GUI_IRCheckUI_Process(void) {
    // 红外检测界面处理
    // TODO: 实现红外检测界面显示与逻辑
}

void GUI_SettingUI_Process(void) {
    // 设置界面处理
    // TODO: 实现设置界面显示与逻辑
}

void GUI_Process(){
    switch (now_gui)
    {
    case IR_SEND_UI:
        GUI_IRSendUI_Process();
        break;
    case IR_RECVIE_UI:
        GUI_IRRecvUI_Process();
        break;
    case MENU_UI:
        GUI_MenuUI_Process();
        break;
    case KEYBOARD_CHECK_UI:
        GUI_KeyboardCheckUI_Process();
        break;
    case DISPLAY_CHECK_UI:
        GUI_DisplayCheckUI_Process();
        break;
    case IR_CHECK_UI:
        GUI_IRCheckUI_Process();
        break;
    case SETTING_UI:
        GUI_SettingUI_Process();
        break;
    default:
        // 默认处理
        GUI_IRSendUI_Process();
        break;
    }
}