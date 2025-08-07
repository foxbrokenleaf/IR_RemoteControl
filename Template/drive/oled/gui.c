#include "gui.h"

gui_uid now_gui = CF_CHECK_UI;
// 当前运行模式
OperationMode currentMode = MODE_COOL;
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

    enum {
        CUR_TEMP = 0,
        CUR_MODE,
        CUR_FAN,
        CUR_SUPER,
        CUR_SWING,
        CUR_SLEEP,
        CUR_LIGHT,
        CUR_MUTE,
        CUR_DRY_HEAT,
        CUR_HEALTH,
        CUR_TIMER,
        CUR_ECO,
        CUR_MAX
    };

    static uint8_t cursor = 0;      // 当前光标项
    static uint8_t editing = 0;     // 是否处于编辑状态
    static uint8_t currentTimerField = 0;  // 定时编辑字段：0-天，1-时，2-分
    KeyboardCode key = GetKeyboardCode();
    uint8_t key_processed = 0;      // 标记按键是否被处理

    // 处理按键
    if (!editing) {
        // 光标移动（上下左右键控制）
        if (key == KEY_CODE_U || key == KEY_CODE_L) {
            cursor = (cursor == 0) ? (CUR_MAX - 1) : (cursor - 1);
            key_processed = 1;
        } else if (key == KEY_CODE_D || key == KEY_CODE_R) {
            cursor = (cursor + 1) % CUR_MAX;
            key_processed = 1;
        } else if (key == KEY_CODE_O) {
            // 检查当前项是否支持编辑，不支持则不进入编辑模式
            switch (cursor) {
                case CUR_TEMP:
                    if (currentMode == MODE_AUTO) key_processed = 1; // 自动模式不支持编辑温度
                    else editing = 1;
                    break;
                case CUR_SUPER:
                    if (currentMode != MODE_COOL && currentMode != MODE_HEAT) key_processed = 1;
                    else editing = 1;
                    break;
                case CUR_SLEEP:
                    if (currentMode != MODE_COOL && currentMode != MODE_DRY && currentMode != MODE_HEAT) key_processed = 1;
                    else editing = 1;
                    break;
                case CUR_DRY_HEAT:
                    if (currentMode != MODE_COOL && currentMode != MODE_DRY && currentMode != MODE_HEAT) key_processed = 1;
                    else editing = 1;
                    break;
                case CUR_ECO:
                    if (currentMode != MODE_COOL) key_processed = 1;
                    else editing = 1;
                    break;
                default:
                    editing = 1; // 其他项默认支持编辑
                    break;
            }
            key_processed = 1;
        }
    } else {
        // 编辑当前项（按O键退出编辑，其他键根据项处理）
        switch (cursor) {
        case CUR_TEMP:  // 温度调节（非自动模式可编辑）
            if (currentMode != MODE_AUTO) {
                if (key == KEY_CODE_U || key == KEY_CODE_L) {
                    Temperature = (Temperature < 30) ? (Temperature + 1) : 30;
                    key_processed = 1;
                } else if (key == KEY_CODE_D || key == KEY_CODE_R) {
                    Temperature = (Temperature > 16) ? (Temperature - 1) : 16;
                    key_processed = 1;
                } else if (key == KEY_CODE_O) {
                    editing = 0;
                    key_processed = 1;
                }
            } else {
                editing = 0;  // 自动模式不可调温度，强制退出
                key_processed = 1;
            }
            break;

        case CUR_MODE:  // 运行模式调节
            if (key == KEY_CODE_U || key == KEY_CODE_L) {
                currentMode = (currentMode == MODE_AUTO) ? MODE_HEAT : (currentMode - 1);
                key_processed = 1;
            } else if (key == KEY_CODE_D || key == KEY_CODE_R) {
                currentMode = (currentMode == MODE_HEAT) ? MODE_AUTO : (currentMode + 1);
                key_processed = 1;
            } else if (key == KEY_CODE_O) {
                editing = 0;
                key_processed = 1;
            }
            break;

        case CUR_FAN:  // 风速调节
            if (key == KEY_CODE_U || key == KEY_CODE_L) {
                FanSpeed = (FanSpeed < 5) ? (FanSpeed + 1) : 5;
                key_processed = 1;
            } else if (key == KEY_CODE_D || key == KEY_CODE_R) {
                FanSpeed = (FanSpeed > 0) ? (FanSpeed - 1) : 0;
                key_processed = 1;
            } else if (key == KEY_CODE_O) {
                editing = 0;
                key_processed = 1;
            }
            break;

        case CUR_SUPER:  // 超强模式（仅制冷/制热有效）
            if (currentMode == MODE_COOL || currentMode == MODE_HEAT) {
                if (key == KEY_CODE_U || key == KEY_CODE_L || key == KEY_CODE_D || key == KEY_CODE_R) {
                    Super = !Super;  // 切换开关
                    key_processed = 1;
                } else if (key == KEY_CODE_O) {
                    editing = 0;
                    key_processed = 1;
                }
            } else {
                editing = 0;  // 其他模式不可用，退出编辑
                key_processed = 1;
            }
            break;

        case CUR_SWING:  // 扫风模式
            if (key == KEY_CODE_U || key == KEY_CODE_L) {
                currentSwing = (currentSwing == SWING_OFF) ? SWING_HORIZ_VERT : (currentSwing - 1);
                key_processed = 1;
            } else if (key == KEY_CODE_D || key == KEY_CODE_R) {
                currentSwing = (currentSwing == SWING_HORIZ_VERT) ? SWING_OFF : (currentSwing + 1);
                key_processed = 1;
            } else if (key == KEY_CODE_O) {
                editing = 0;
                key_processed = 1;
            }
            break;

        case CUR_SLEEP:  // 睡眠模式
            if (currentMode == MODE_COOL || currentMode == MODE_DRY || currentMode == MODE_HEAT) {
                if (key == KEY_CODE_U || key == KEY_CODE_L) {
                    Sleep = (Sleep < 3) ? (Sleep + 1) : 0;
                    key_processed = 1;
                } else if (key == KEY_CODE_D || key == KEY_CODE_R) {
                    Sleep = (Sleep > 0) ? (Sleep - 1) : 3;
                    key_processed = 1;
                } else if (key == KEY_CODE_O) {
                    editing = 0;
                    key_processed = 1;
                }
            } else {
                editing = 0;  // 其他模式不可用
                key_processed = 1;
            }
            break;

        case CUR_LIGHT:  // 灯光开关
            if (key == KEY_CODE_U || key == KEY_CODE_L || key == KEY_CODE_D || key == KEY_CODE_R) {
                Light = !Light;
                key_processed = 1;
            } else if (key == KEY_CODE_O) {
                editing = 0;
                key_processed = 1;
            }
            break;

        case CUR_MUTE:  // 静音模式
            if (key == KEY_CODE_U || key == KEY_CODE_L) {
                currentMute = (currentMute == MUTE_OFF) ? MUTE_AUTO_MUTE : (currentMute - 1);
                key_processed = 1;
            } else if (key == KEY_CODE_D || key == KEY_CODE_R) {
                currentMute = (currentMute == MUTE_AUTO_MUTE) ? MUTE_OFF : (currentMute + 1);
                key_processed = 1;
            } else if (key == KEY_CODE_O) {
                editing = 0;
                key_processed = 1;
            }
            break;

        case CUR_DRY_HEAT:  // 干燥/辅热
            if (currentMode == MODE_COOL || currentMode == MODE_DRY) {
                if (key == KEY_CODE_U || key == KEY_CODE_L || key == KEY_CODE_D || key == KEY_CODE_R) {
                    Dry = !Dry;
                    key_processed = 1;
                } else if (key == KEY_CODE_O) {
                    editing = 0;
                    key_processed = 1;
                }
            } else if (currentMode == MODE_HEAT) {
                if (key == KEY_CODE_U || key == KEY_CODE_L || key == KEY_CODE_D || key == KEY_CODE_R) {
                    Heat = !Heat;
                    key_processed = 1;
                } else if (key == KEY_CODE_O) {
                    editing = 0;
                    key_processed = 1;
                }
            } else {
                editing = 0;  // 其他模式不可用
                key_processed = 1;
            }
            break;

        case CUR_HEALTH:  // 健康/换气
            if (key == KEY_CODE_U || key == KEY_CODE_L) {
                currentHealth = (currentHealth == HEALTH_OFF) ? HEALTH_HV : (currentHealth - 1);
                key_processed = 1;
            } else if (key == KEY_CODE_D || key == KEY_CODE_R) {
                currentHealth = (currentHealth == HEALTH_HV) ? HEALTH_OFF : (currentHealth + 1);
                key_processed = 1;
            } else if (key == KEY_CODE_O) {
                editing = 0;
                key_processed = 1;
            }
            break;

        case CUR_TIMER:  // 定时时间（支持天/时/分选择调节）
            if (key == KEY_CODE_L) {
                // 左移选择：天 ← 时 ← 分（循环）
                currentTimerField = (currentTimerField == 0) ? 2 : (currentTimerField - 1);
                key_processed = 1;
            } else if (key == KEY_CODE_R) {
                // 右移选择：天 → 时 → 分（循环）
                currentTimerField = (currentTimerField + 1) % 3;
                key_processed = 1;
            } else if (key == KEY_CODE_U) {
                // 上调节数值（根据当前选中字段）
                switch(currentTimerField) {
                    case 0:  // 天（0-30循环）
                        timerRingTime.day = (timerRingTime.day + 1) % 31;
                        break;
                    case 1:  // 时（0-23循环）
                        timerRingTime.hour = (timerRingTime.hour + 1) % 24;
                        break;
                    case 2:  // 分（0-59循环）
                        timerRingTime.min = (timerRingTime.min + 1) % 60;
                        break;
                }
                key_processed = 1;
            } else if (key == KEY_CODE_D) {
                // 下调节数值（根据当前选中字段）
                switch(currentTimerField) {
                    case 0:  // 天（0-30循环）
                        timerRingTime.day = (timerRingTime.day == 0) ? 30 : (timerRingTime.day - 1);
                        break;
                    case 1:  // 时（0-23循环）
                        timerRingTime.hour = (timerRingTime.hour == 0) ? 23 : (timerRingTime.hour - 1);
                        break;
                    case 2:  // 分（0-59循环）
                        timerRingTime.min = (timerRingTime.min == 0) ? 59 : (timerRingTime.min - 1);
                        break;
                }
                key_processed = 1;
            } else if (key == KEY_CODE_O) {
                editing = 0;  // 退出编辑模式
                key_processed = 1;
            }
            break;

        case CUR_ECO:  // 节能模式
            if (currentMode == MODE_COOL) {
                if (key == KEY_CODE_U || key == KEY_CODE_L || key == KEY_CODE_D || key == KEY_CODE_R) {
                    Eco = !Eco;
                    key_processed = 1;
                } else if (key == KEY_CODE_O) {
                    editing = 0;
                    key_processed = 1;
                }
            } else {
                editing = 0;  // 其他模式不可用
                key_processed = 1;
            }
            break;

        default:  // 未定义项按O键退出
            if (key == KEY_CODE_O) {
                editing = 0;
                key_processed = 1;
            }
            break;
        }

        // 处理未定义的按键（如无效键），避免阻塞
        if (!key_processed) {
            key_processed = 1;
        }
    }

    // 清除未处理的按键（防止缓冲区堆积）
    if (!key_processed) {
        // 根据实际硬件添加清除按键缓冲区的代码
        // ClearKeyboardBuffer();
    }

    // -------------------------- 绘制显示内容及光标横线 --------------------------
    // 1. 温度（CUR_TEMP）
    if(currentMode != MODE_AUTO){
        if(cursor == CUR_TEMP) {
            if (editing) {
                OLED_DrawDashedLine(0, 8, 60, 8);  // 选中状态：虚线
            } else {
                OLED_DrawLine(0, 8, 60, 8);         // 未选中：实线
            }
        }
        OLED_ShowString(0, 0, "Temp:", OLED_6X8);
        Temperature = (Temperature < 16) ? 16 : (Temperature > 30) ? 30 : Temperature;  // 限幅
        OLED_ShowNum(30, 0, Temperature, 2, OLED_6X8);
        OLED_ShowString(45, 0, "C", OLED_6X8);
        // 绘制摄氏度符号小点
        OLED_DrawPoint(43, 0);
        OLED_DrawPoint(44, 0);
        OLED_DrawPoint(43, 1);
        OLED_DrawPoint(44, 1);
    }

    // 2. 运行模式（CUR_MODE）
    if(currentMode == MODE_NONE) currentMode = MODE_AUTO;  // 初始化为自动模式
    if(cursor == CUR_MODE) {
        if (editing) {
            OLED_DrawDashedLine(72, 8, 128, 8);  // 选中：虚线
        } else {
            OLED_DrawLine(72, 8, 128, 8);         // 未选中：实线
        }
    }
    OLED_ShowString(72, 0, "Mode:    ", OLED_6X8);
    switch(currentMode) {
        case MODE_AUTO: OLED_ShowString(102, 0, "Auto", OLED_6X8); break;
        case MODE_COOL: OLED_ShowString(102, 0, "Cool", OLED_6X8); break;
        case MODE_DRY:  OLED_ShowString(102, 0, "Dry ", OLED_6X8); break;
        case MODE_FAN:  OLED_ShowString(102, 0, "Fan ", OLED_6X8); break;
        case MODE_HEAT: OLED_ShowString(102, 0, "Heat", OLED_6X8); break;
    }

    // 3. 风速（CUR_FAN）
    if(cursor == CUR_FAN) {
        if (editing) {
            OLED_DrawDashedLine(72, 19, 128, 19);  // 选中：虚线
        } else {
            OLED_DrawLine(72, 19, 128, 19);         // 未选中：实线
        }
    }
    OLED_ShowString(72, 11, "Fan:", OLED_6X8);
    if(FanSpeed > 5) FanSpeed = 0;  // 限幅
    if(FanSpeed == 0) {
        OLED_ShowString(96, 11, "Auto", OLED_6X8);
    } else {
        OLED_ShowNum(96, 11, FanSpeed, 1, OLED_6X8);
    }

    // 4. 超强模式（CUR_SUPER）
    if(currentMode == MODE_COOL || currentMode == MODE_HEAT){
        if(cursor == CUR_SUPER) {
            if (editing) {
                OLED_DrawDashedLine(0, 19, 60, 19);  // 选中：虚线
            } else {
                OLED_DrawLine(0, 19, 60, 19);         // 未选中：实线
            }
        }
        OLED_ShowString(0, 11, "Super:", OLED_6X8);
        OLED_ShowString(36, 11, Super ? "ON" : "OFF", OLED_6X8);
    }

    // 5. 扫风（CUR_SWING）
    if(cursor == CUR_SWING) {
        if (editing) {
            OLED_DrawDashedLine(72, 30, 128, 30);  // 选中：虚线
        } else {
            OLED_DrawLine(72, 30, 128, 30);         // 未选中：实线
        }
    }
    if(currentSwing == SWING_NONE) currentSwing = SWING_OFF;  // 初始化为关闭
    OLED_ShowString(72, 22, "Swing:", OLED_6X8);
    switch(currentSwing) {
        case SWING_OFF:        OLED_ShowString(108, 22, "OFF", OLED_6X8); break;
        case SWING_VERT:       OLED_ShowString(108, 22, "V  ", OLED_6X8); break;
        case SWING_HORIZ:      OLED_ShowString(108, 22, "H  ", OLED_6X8); break;
        case SWING_HORIZ_VERT: OLED_ShowString(108, 22, "V&H", OLED_6X8); break;
    }

    // 6. 静音（CUR_MUTE）
    if(cursor == CUR_MUTE) {
        if (editing) {
            OLED_DrawDashedLine(0, 30, 60, 30);  // 选中：虚线
        } else {
            OLED_DrawLine(0, 30, 60, 30);         // 未选中：实线
        }
    }
    if(currentMute == MUTE_NONE) currentMute = MUTE_OFF;  // 初始化为关闭
    OLED_ShowString(0, 22, "Mute:", OLED_6X8);
    switch(currentMute) {
        case MUTE_OFF:        OLED_ShowString(30, 22, "OFF", OLED_6X8); break;
        case MUTE_MUTE:       OLED_ShowString(30, 22, "M  ", OLED_6X8); break;
        case MUTE_AUTO_MUTE:  OLED_ShowString(30, 22, "A&M", OLED_6X8); break;
    }

    // 7. 睡眠（CUR_SLEEP）
    if(currentMode == MODE_COOL || currentMode == MODE_DRY || currentMode == MODE_HEAT){    
        if(cursor == CUR_SLEEP) {
            if (editing) {
                OLED_DrawDashedLine(72, 41, 128, 41);  // 选中：虚线
            } else {
                OLED_DrawLine(72, 41, 128, 41);         // 未选中：实线
            }
        }
        OLED_ShowString(72, 33, "Sleep:", OLED_6X8);
        Sleep = (Sleep > 3) ? 0 : Sleep;  // 限幅
        if(Sleep == 0) {
            OLED_ShowString(108, 33, "OFF", OLED_6X8);
        } else {
            OLED_ShowNum(108, 33, Sleep, 1, OLED_6X8);
        }
    }

    // 8. 干燥/辅热（CUR_DRY_HEAT）
    if(currentMode == MODE_COOL || currentMode == MODE_DRY){
        if(cursor == CUR_DRY_HEAT) {
            if (editing) {
                OLED_DrawDashedLine(0, 41, 60, 41);  // 选中：虚线
            } else {
                OLED_DrawLine(0, 41, 60, 41);         // 未选中：实线
            }
        }
        OLED_ShowString(0, 33, "Dry:", OLED_6X8);
        OLED_ShowString(24, 33, Dry ? "ON" : "OFF", OLED_6X8);
    } else if(currentMode == MODE_HEAT){
        if(cursor == CUR_DRY_HEAT) {
            if (editing) {
                OLED_DrawDashedLine(0, 41, 60, 41);  // 选中：虚线
            } else {
                OLED_DrawLine(0, 41, 60, 41);         // 未选中：实线
            }
        }
        OLED_ShowString(0, 33, "Heat:", OLED_6X8);
        OLED_ShowString(30, 33, Heat ? "ON" : "OFF", OLED_6X8);
    }

    // 9. 灯光（CUR_LIGHT）
    if(cursor == CUR_LIGHT) {
        if (editing) {
            OLED_DrawDashedLine(72, 52, 128, 52);  // 调整Y坐标防止超出屏幕
        } else {
            OLED_DrawLine(72, 52, 128, 52);         // 调整Y坐标防止超出屏幕
        }
    }
    OLED_ShowString(72, 44, "Light:", OLED_6X8);  // 上移显示位置
    Light = (Light > 1) ? 0 : Light;  // 限幅
    OLED_ShowString(108, 44, Light ? "ON" : "OFF", OLED_6X8);  // 上移显示位置

    // 10. 健康/换气（CUR_HEALTH）
    if(cursor == CUR_HEALTH) {
        if (editing) {
            OLED_DrawDashedLine(0, 52, 60, 52);  // 调整Y坐标防止超出屏幕
        } else {
            OLED_DrawLine(0, 52, 60, 52);         // 调整Y坐标防止超出屏幕
        }
    }
    if(currentHealth == HEALTH_NONE) currentHealth = HEALTH_OFF;  // 初始化为关闭
    OLED_ShowString(0, 44, "Health:", OLED_6X8);  // 上移显示位置
    switch(currentHealth) {
        case HEALTH_OFF: OLED_ShowString(42, 44, "OFF", OLED_6X8); break;
        case HEALTH_H:   OLED_ShowString(42, 44, "H", OLED_6X8); break;
        case HEALTH_V:   OLED_ShowString(42, 44, "V", OLED_6X8); break;
        case HEALTH_HV:  OLED_ShowString(42, 44, "H&V", OLED_6X8); break;
    }

    // 11. 定时（CUR_TIMER）- 支持天/时/分选择调节
    OLED_ShowString(0, 55, "Tmr:", OLED_6X8);  // 上移显示位置
    // 定时时间格式：天-时:分（限幅处理）
    timerRingTime.day = (timerRingTime.day > 30) ? 30 : timerRingTime.day;
    timerRingTime.hour = (timerRingTime.hour > 23) ? 23 : timerRingTime.hour;
    timerRingTime.min = (timerRingTime.min > 59) ? 59 : timerRingTime.min;

    // 显示天、时、分
    OLED_ShowNum(24, 55, timerRingTime.day, 2, OLED_6X8);   // 天
    OLED_ShowChar(36, 55, '-', OLED_6X8);
    OLED_ShowNum(42, 55, timerRingTime.hour, 2, OLED_6X8);  // 时
    OLED_ShowChar(54, 55, ':', OLED_6X8);
    OLED_ShowNum(60, 55, timerRingTime.min, 2, OLED_6X8);   // 分    
    if(cursor == CUR_TIMER) {
        if (editing) {
            OLED_DrawDashedLine(0, 62, 70, 62);  // 上移1像素，防止超出屏幕底部
        } else {
            OLED_DrawLine(0, 62, 70, 62);         // 上移1像素，防止超出屏幕底部
        }
        
        // 绘制当前选中字段的高亮指示（下划线）
        if (editing) {  // 仅在编辑模式显示字段选择指示
            switch(currentTimerField) {
                case 0:  // 天
                    OLED_DrawLine(24, 60, 34, 60);  // 天数字下方下划线
                    break;
                case 1:  // 时
                    OLED_DrawLine(42, 60, 52, 60);  // 时数字下方下划线
                    break;
                case 2:  // 分
                    OLED_DrawLine(60, 60, 70, 60);  // 分数字下方下划线
                    break;
            }
        }
    }

    // 12. 节能（CUR_ECO）
    if(currentMode == MODE_COOL){
        OLED_ShowString(86, 55, "Eco:", OLED_6X8);  // 上移显示位置
        Eco = (Eco > 1) ? 0 : Eco;  // 限幅
        OLED_ShowString(110, 55, Eco ? "ON" : "OFF", OLED_6X8);  // 上移显示位置
        if(cursor == CUR_ECO) {
            if (editing) {
                OLED_DrawDashedLine(86, 62, 128, 62);  // 上移1像素，防止超出屏幕底部
            } else {
                OLED_DrawLine(86, 62, 128, 62);         // 上移1像素，防止超出屏幕底部
            }
        }
    }
}
    

void GUI_IRRecvUI_Process(void) {
    OLED_Clear();
    OLED_ShowString(0, 0, "IR Recv Bits:", OLED_6X8);

    static uint8_t bits[64] = {0};
    static uint8_t bit_count = 0;
    static uint8_t capturing = 0;    // 0: 等待信号, 1: 捕获中, 2: 捕获完成等待显示
    static uint32_t last_tick = 0;

    if (capturing == 0) {
        // 等待信号出现（IR_Recvier_Get_Bit返回0时认为有信号）
        uint8_t bit = IR_Recvier_Get_Bit();
        if (bit == 0) {
            capturing = 1;
            bit_count = 0;
            bits[bit_count++] = bit;
            last_tick = systemtick; // 记录捕获开始时间
        }
    } else if (capturing == 1) {
        // 捕获数据
        uint8_t bit = IR_Recvier_Get_Bit();
        bits[bit_count++] = bit;
        last_tick = systemtick;
        // 捕获满64位或检测到连续16个1（无信号）或超时
        if (bit_count >= 64) {
            capturing = 2;
        } else if (bit == 1) {
            // 检查最近16位是否全为1，作为结束条件
            if (bit_count >= 16) {
                uint8_t all_one = 1;
                for (uint8_t i = bit_count - 16; i < bit_count; i++) {
                    if (bits[i] != 1) {
                        all_one = 0;
                        break;
                    }
                }
                if (all_one) capturing = 2;
            }
        }
        // 超时保护（假设100ms无新bit则结束）
        if (systemtick - last_tick > 100) {
            capturing = 2;
        }
    } else if (capturing == 2) {
        // 显示捕获到的数据
        for (uint8_t row = 0; row < 8; row++) {
            char line[9] = {0};
            for (uint8_t col = 0; col < 8; col++) {
                uint8_t idx = row * 8 + col;
                if (idx < bit_count) {
                    line[col] = bits[idx] ? '1' : '0';
                } else {
                    line[col] = ' ';
                }
            }
            OLED_ShowString(0, 10 + row * 8, line, OLED_6X8);
        }
        // 检查信号是否恢复为全1（无信号），准备下一次捕获
        uint8_t idle = 1;
        for (uint8_t i = 0; i < 8; i++) {
            if (IR_Recvier_Get_Bit() == 0) {
                idle = 0;
                break;
            }
        }
        if (idle) {
            capturing = 0;
        }
    }
}

void GUI_MenuUI_Process(void) {
    // 菜单界面处理
    // TODO: 实现菜单界面显示与逻辑
}

void GUI_KeyboardCheckUI_Process(void) {

    KeyboardCode key_res = GetKeyboardCode();
    static uint8_t KeyDown_Counter = 0;
    if(key_res != 0) KeyDown_Counter++;

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

    OLED_ShowString(0, 64 - 16, "Click:", OLED_6X8);
    OLED_ShowNum(0, 64 - 8, KeyDown_Counter, 3, OLED_6X8);

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

void GUI_MCU_PeripheralsUI_Process(void){
    OLED_ShowString(0, 0, "MCU Peripherals Test", OLED_6X8);
    OLED_ShowString(0, 8, "Timer counter:", OLED_6X8);
    OLED_ShowNum(0, 16, timer_counter_read(TIMER2), 10, OLED_6X8);
    OLED_ShowString(0, 24, "Timer tick:", OLED_6X8);
    OLED_ShowNum(0, 32, tim2Tick, 10, OLED_6X8);
}

void GUI_CarrierFrequencyCheck_Process(void){

    static uint8_t o_key_state = 0;  // 0表示初始状态，1表示已按下一次

    OLED_ShowString(40, 0, "CF Check", OLED_6X8);
    OLED_DrawLine(0, 10, 128, 10);
    OLED_ShowString(0, 12, "NE555 Status:", OLED_8X16);
    // 获取当前按下的按键值
    uint16_t key_code = GetKeyboardCode();
    
    // 检测是否按下了O键
    if (key_code == KEY_CODE_O) {
        // 根据当前状态调用不同的IR发送函数
        if (o_key_state == 0) {
            IR_Send_BUS(0);
            o_key_state = 1;  // 更新状态
        } else {
            IR_Send_BUS(1);
            o_key_state = 0;  // 重置状态
        }
    }

    o_key_state ? OLED_ShowString(104, 12, "ON", OLED_8X16) : OLED_ShowString(104, 12, "OFF", OLED_8X16);

}

void GUI_Process(){
    OLED_Clear();
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
    case MCU_PERIFS_TEST_UI:
        GUI_MCU_PeripheralsUI_Process();
        break;
    case CF_CHECK_UI:
        GUI_CarrierFrequencyCheck_Process();
        break;
    default:
        // 默认处理
        GUI_IRSendUI_Process();
        break;
    }
    OLED_Update();
}