#include "gui.h"

gui_uid now_gui = IR_SEND_UI;
// ��ǰ����ģʽ
OperationMode currentMode = MODE_COOL;
// ��ǰɨ��ģʽ
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

    static uint8_t cursor = 0;      // ��ǰ�����
    static uint8_t editing = 0;     // �Ƿ��ڱ༭״̬
    static uint8_t currentTimerField = 0;  // ��ʱ�༭�ֶΣ�0-�죬1-ʱ��2-��
    KeyboardCode key = GetKeyboardCode();
    uint8_t key_processed = 0;      // ��ǰ����Ƿ񱻴���

    // ������
    if (!editing) {
        // ����ƶ����������Ҽ����ƣ�
        if (key == KEY_CODE_U || key == KEY_CODE_L) {
            cursor = (cursor == 0) ? (CUR_MAX - 1) : (cursor - 1);
            key_processed = 1;
        } else if (key == KEY_CODE_D || key == KEY_CODE_R) {
            cursor = (cursor + 1) % CUR_MAX;
            key_processed = 1;
        } else if (key == KEY_CODE_O) {
            // ��鵱ǰ���Ƿ�֧�ֱ༭����֧���򲻽���༭ģʽ
            switch (cursor) {
                case CUR_TEMP:
                    if (currentMode == MODE_AUTO) key_processed = 1; // �Զ�ģʽ��֧�ֱ༭�¶�
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
                    editing = 1; // ������Ĭ��֧�ֱ༭
                    break;
            }
            key_processed = 1;
        }
    } else {
        // �༭��ǰ���O���˳��༭���������������
        switch (cursor) {
        case CUR_TEMP:  // �¶ȵ��ڣ����Զ�ģʽ�ɱ༭��
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
                editing = 0;  // �Զ�ģʽ���ɵ��¶ȣ�ǿ���˳�
                key_processed = 1;
            }
            break;

        case CUR_MODE:  // ����ģʽ����
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

        case CUR_FAN:  // ���ٵ���
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

        case CUR_SUPER:  // ��ǿģʽ��������/������Ч��
            if (currentMode == MODE_COOL || currentMode == MODE_HEAT) {
                if (key == KEY_CODE_U || key == KEY_CODE_L || key == KEY_CODE_D || key == KEY_CODE_R) {
                    Super = !Super;  // �л�����
                    key_processed = 1;
                } else if (key == KEY_CODE_O) {
                    editing = 0;
                    key_processed = 1;
                }
            } else {
                editing = 0;  // ����ģʽ�����ã��˳��༭
                key_processed = 1;
            }
            break;

        case CUR_SWING:  // ɨ��ģʽ
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

        case CUR_SLEEP:  // ˯��ģʽ
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
                editing = 0;  // ����ģʽ������
                key_processed = 1;
            }
            break;

        case CUR_LIGHT:  // �ƹ⿪��
            if (key == KEY_CODE_U || key == KEY_CODE_L || key == KEY_CODE_D || key == KEY_CODE_R) {
                Light = !Light;
                key_processed = 1;
            } else if (key == KEY_CODE_O) {
                editing = 0;
                key_processed = 1;
            }
            break;

        case CUR_MUTE:  // ����ģʽ
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

        case CUR_DRY_HEAT:  // ����/����
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
                editing = 0;  // ����ģʽ������
                key_processed = 1;
            }
            break;

        case CUR_HEALTH:  // ����/����
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

        case CUR_TIMER:  // ��ʱʱ�䣨֧����/ʱ/��ѡ����ڣ�
            if (key == KEY_CODE_L) {
                // ����ѡ���� �� ʱ �� �֣�ѭ����
                currentTimerField = (currentTimerField == 0) ? 2 : (currentTimerField - 1);
                key_processed = 1;
            } else if (key == KEY_CODE_R) {
                // ����ѡ���� �� ʱ �� �֣�ѭ����
                currentTimerField = (currentTimerField + 1) % 3;
                key_processed = 1;
            } else if (key == KEY_CODE_U) {
                // �ϵ�����ֵ�����ݵ�ǰѡ���ֶΣ�
                switch(currentTimerField) {
                    case 0:  // �죨0-30ѭ����
                        timerRingTime.day = (timerRingTime.day + 1) % 31;
                        break;
                    case 1:  // ʱ��0-23ѭ����
                        timerRingTime.hour = (timerRingTime.hour + 1) % 24;
                        break;
                    case 2:  // �֣�0-59ѭ����
                        timerRingTime.min = (timerRingTime.min + 1) % 60;
                        break;
                }
                key_processed = 1;
            } else if (key == KEY_CODE_D) {
                // �µ�����ֵ�����ݵ�ǰѡ���ֶΣ�
                switch(currentTimerField) {
                    case 0:  // �죨0-30ѭ����
                        timerRingTime.day = (timerRingTime.day == 0) ? 30 : (timerRingTime.day - 1);
                        break;
                    case 1:  // ʱ��0-23ѭ����
                        timerRingTime.hour = (timerRingTime.hour == 0) ? 23 : (timerRingTime.hour - 1);
                        break;
                    case 2:  // �֣�0-59ѭ����
                        timerRingTime.min = (timerRingTime.min == 0) ? 59 : (timerRingTime.min - 1);
                        break;
                }
                key_processed = 1;
            } else if (key == KEY_CODE_O) {
                editing = 0;  // �˳��༭ģʽ
                key_processed = 1;
            }
            break;

        case CUR_ECO:  // ����ģʽ
            if (currentMode == MODE_COOL) {
                if (key == KEY_CODE_U || key == KEY_CODE_L || key == KEY_CODE_D || key == KEY_CODE_R) {
                    Eco = !Eco;
                    key_processed = 1;
                } else if (key == KEY_CODE_O) {
                    editing = 0;
                    key_processed = 1;
                }
            } else {
                editing = 0;  // ����ģʽ������
                key_processed = 1;
            }
            break;

        default:  // δ�����O���˳�
            if (key == KEY_CODE_O) {
                editing = 0;
                key_processed = 1;
            }
            break;
        }

        // ����δ����İ���������Ч��������������
        if (!key_processed) {
            key_processed = 1;
        }
    }

    // ���δ����İ�������ֹ�������ѻ���
    if (!key_processed) {
        // ����ʵ��Ӳ�������������������Ĵ���
        // ClearKeyboardBuffer();
    }

    // -------------------------- ������ʾ���ݼ������� --------------------------
    // 1. �¶ȣ�CUR_TEMP��
    if(currentMode != MODE_AUTO){
        if(cursor == CUR_TEMP) {
            if (editing) {
                OLED_DrawDashedLine(0, 8, 60, 8);  // ѡ��״̬������
            } else {
                OLED_DrawLine(0, 8, 60, 8);         // δѡ�У�ʵ��
            }
        }
        OLED_ShowString(0, 0, "Temp:", OLED_6X8);
        Temperature = (Temperature < 16) ? 16 : (Temperature > 30) ? 30 : Temperature;  // �޷�
        OLED_ShowNum(30, 0, Temperature, 2, OLED_6X8);
        OLED_ShowString(45, 0, "C", OLED_6X8);
        // �������϶ȷ���С��
        OLED_DrawPoint(43, 0);
        OLED_DrawPoint(44, 0);
        OLED_DrawPoint(43, 1);
        OLED_DrawPoint(44, 1);
    }

    // 2. ����ģʽ��CUR_MODE��
    if(currentMode == MODE_NONE) currentMode = MODE_AUTO;  // ��ʼ��Ϊ�Զ�ģʽ
    if(cursor == CUR_MODE) {
        if (editing) {
            OLED_DrawDashedLine(72, 8, 128, 8);  // ѡ�У�����
        } else {
            OLED_DrawLine(72, 8, 128, 8);         // δѡ�У�ʵ��
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

    // 3. ���٣�CUR_FAN��
    if(cursor == CUR_FAN) {
        if (editing) {
            OLED_DrawDashedLine(72, 19, 128, 19);  // ѡ�У�����
        } else {
            OLED_DrawLine(72, 19, 128, 19);         // δѡ�У�ʵ��
        }
    }
    OLED_ShowString(72, 11, "Fan:", OLED_6X8);
    if(FanSpeed > 5) FanSpeed = 0;  // �޷�
    if(FanSpeed == 0) {
        OLED_ShowString(96, 11, "Auto", OLED_6X8);
    } else {
        OLED_ShowNum(96, 11, FanSpeed, 1, OLED_6X8);
    }

    // 4. ��ǿģʽ��CUR_SUPER��
    if(currentMode == MODE_COOL || currentMode == MODE_HEAT){
        if(cursor == CUR_SUPER) {
            if (editing) {
                OLED_DrawDashedLine(0, 19, 60, 19);  // ѡ�У�����
            } else {
                OLED_DrawLine(0, 19, 60, 19);         // δѡ�У�ʵ��
            }
        }
        OLED_ShowString(0, 11, "Super:", OLED_6X8);
        OLED_ShowString(36, 11, Super ? "ON" : "OFF", OLED_6X8);
    }

    // 5. ɨ�磨CUR_SWING��
    if(cursor == CUR_SWING) {
        if (editing) {
            OLED_DrawDashedLine(72, 30, 128, 30);  // ѡ�У�����
        } else {
            OLED_DrawLine(72, 30, 128, 30);         // δѡ�У�ʵ��
        }
    }
    if(currentSwing == SWING_NONE) currentSwing = SWING_OFF;  // ��ʼ��Ϊ�ر�
    OLED_ShowString(72, 22, "Swing:", OLED_6X8);
    switch(currentSwing) {
        case SWING_OFF:        OLED_ShowString(108, 22, "OFF", OLED_6X8); break;
        case SWING_VERT:       OLED_ShowString(108, 22, "V  ", OLED_6X8); break;
        case SWING_HORIZ:      OLED_ShowString(108, 22, "H  ", OLED_6X8); break;
        case SWING_HORIZ_VERT: OLED_ShowString(108, 22, "V&H", OLED_6X8); break;
    }

    // 6. ������CUR_MUTE��
    if(cursor == CUR_MUTE) {
        if (editing) {
            OLED_DrawDashedLine(0, 30, 60, 30);  // ѡ�У�����
        } else {
            OLED_DrawLine(0, 30, 60, 30);         // δѡ�У�ʵ��
        }
    }
    if(currentMute == MUTE_NONE) currentMute = MUTE_OFF;  // ��ʼ��Ϊ�ر�
    OLED_ShowString(0, 22, "Mute:", OLED_6X8);
    switch(currentMute) {
        case MUTE_OFF:        OLED_ShowString(30, 22, "OFF", OLED_6X8); break;
        case MUTE_MUTE:       OLED_ShowString(30, 22, "M  ", OLED_6X8); break;
        case MUTE_AUTO_MUTE:  OLED_ShowString(30, 22, "A&M", OLED_6X8); break;
    }

    // 7. ˯�ߣ�CUR_SLEEP��
    if(currentMode == MODE_COOL || currentMode == MODE_DRY || currentMode == MODE_HEAT){    
        if(cursor == CUR_SLEEP) {
            if (editing) {
                OLED_DrawDashedLine(72, 41, 128, 41);  // ѡ�У�����
            } else {
                OLED_DrawLine(72, 41, 128, 41);         // δѡ�У�ʵ��
            }
        }
        OLED_ShowString(72, 33, "Sleep:", OLED_6X8);
        Sleep = (Sleep > 3) ? 0 : Sleep;  // �޷�
        if(Sleep == 0) {
            OLED_ShowString(108, 33, "OFF", OLED_6X8);
        } else {
            OLED_ShowNum(108, 33, Sleep, 1, OLED_6X8);
        }
    }

    // 8. ����/���ȣ�CUR_DRY_HEAT��
    if(currentMode == MODE_COOL || currentMode == MODE_DRY){
        if(cursor == CUR_DRY_HEAT) {
            if (editing) {
                OLED_DrawDashedLine(0, 41, 60, 41);  // ѡ�У�����
            } else {
                OLED_DrawLine(0, 41, 60, 41);         // δѡ�У�ʵ��
            }
        }
        OLED_ShowString(0, 33, "Dry:", OLED_6X8);
        OLED_ShowString(24, 33, Dry ? "ON" : "OFF", OLED_6X8);
    } else if(currentMode == MODE_HEAT){
        if(cursor == CUR_DRY_HEAT) {
            if (editing) {
                OLED_DrawDashedLine(0, 41, 60, 41);  // ѡ�У�����
            } else {
                OLED_DrawLine(0, 41, 60, 41);         // δѡ�У�ʵ��
            }
        }
        OLED_ShowString(0, 33, "Heat:", OLED_6X8);
        OLED_ShowString(30, 33, Heat ? "ON" : "OFF", OLED_6X8);
    }

    // 9. �ƹ⣨CUR_LIGHT��
    if(cursor == CUR_LIGHT) {
        if (editing) {
            OLED_DrawDashedLine(72, 52, 128, 52);  // ����Y�����ֹ������Ļ
        } else {
            OLED_DrawLine(72, 52, 128, 52);         // ����Y�����ֹ������Ļ
        }
    }
    OLED_ShowString(72, 44, "Light:", OLED_6X8);  // ������ʾλ��
    Light = (Light > 1) ? 0 : Light;  // �޷�
    OLED_ShowString(108, 44, Light ? "ON" : "OFF", OLED_6X8);  // ������ʾλ��

    // 10. ����/������CUR_HEALTH��
    if(cursor == CUR_HEALTH) {
        if (editing) {
            OLED_DrawDashedLine(0, 52, 60, 52);  // ����Y�����ֹ������Ļ
        } else {
            OLED_DrawLine(0, 52, 60, 52);         // ����Y�����ֹ������Ļ
        }
    }
    if(currentHealth == HEALTH_NONE) currentHealth = HEALTH_OFF;  // ��ʼ��Ϊ�ر�
    OLED_ShowString(0, 44, "Health:", OLED_6X8);  // ������ʾλ��
    switch(currentHealth) {
        case HEALTH_OFF: OLED_ShowString(42, 44, "OFF", OLED_6X8); break;
        case HEALTH_H:   OLED_ShowString(42, 44, "H", OLED_6X8); break;
        case HEALTH_V:   OLED_ShowString(42, 44, "V", OLED_6X8); break;
        case HEALTH_HV:  OLED_ShowString(42, 44, "H&V", OLED_6X8); break;
    }

    // 11. ��ʱ��CUR_TIMER��- ֧����/ʱ/��ѡ�����
    OLED_ShowString(0, 55, "Tmr:", OLED_6X8);  // ������ʾλ��
    // ��ʱʱ���ʽ����-ʱ:�֣��޷�����
    timerRingTime.day = (timerRingTime.day > 30) ? 30 : timerRingTime.day;
    timerRingTime.hour = (timerRingTime.hour > 23) ? 23 : timerRingTime.hour;
    timerRingTime.min = (timerRingTime.min > 59) ? 59 : timerRingTime.min;

    // ��ʾ�졢ʱ����
    OLED_ShowNum(24, 55, timerRingTime.day, 2, OLED_6X8);   // ��
    OLED_ShowChar(36, 55, '-', OLED_6X8);
    OLED_ShowNum(42, 55, timerRingTime.hour, 2, OLED_6X8);  // ʱ
    OLED_ShowChar(54, 55, ':', OLED_6X8);
    OLED_ShowNum(60, 55, timerRingTime.min, 2, OLED_6X8);   // ��    
    if(cursor == CUR_TIMER) {
        if (editing) {
            OLED_DrawDashedLine(0, 62, 70, 62);  // ����1���أ���ֹ������Ļ�ײ�
        } else {
            OLED_DrawLine(0, 62, 70, 62);         // ����1���أ���ֹ������Ļ�ײ�
        }
        
        // ���Ƶ�ǰѡ���ֶεĸ���ָʾ���»��ߣ�
        if (editing) {  // ���ڱ༭ģʽ��ʾ�ֶ�ѡ��ָʾ
            switch(currentTimerField) {
                case 0:  // ��
                    OLED_DrawLine(24, 60, 34, 60);  // �������·��»���
                    break;
                case 1:  // ʱ
                    OLED_DrawLine(42, 60, 52, 60);  // ʱ�����·��»���
                    break;
                case 2:  // ��
                    OLED_DrawLine(60, 60, 70, 60);  // �������·��»���
                    break;
            }
        }
    }

    // 12. ���ܣ�CUR_ECO��
    if(currentMode == MODE_COOL){
        OLED_ShowString(86, 55, "Eco:", OLED_6X8);  // ������ʾλ��
        Eco = (Eco > 1) ? 0 : Eco;  // �޷�
        OLED_ShowString(110, 55, Eco ? "ON" : "OFF", OLED_6X8);  // ������ʾλ��
        if(cursor == CUR_ECO) {
            if (editing) {
                OLED_DrawDashedLine(86, 62, 128, 62);  // ����1���أ���ֹ������Ļ�ײ�
            } else {
                OLED_DrawLine(86, 62, 128, 62);         // ����1���أ���ֹ������Ļ�ײ�
            }
        }
    }

    OLED_Update();
}
    

// void GUI_IRSendUI_Process(void) {
//     OLED_Clear();

//     OLED_DrawLine(0, 20, 69, 20);
//     OLED_DrawLine(69, 0, 69, 20);
//     OLED_DrawLine(0, 9, 128, 9);
//     OLED_DrawLine(0, 20, 128, 20);
//     OLED_DrawLine(0, 31, 128, 31);
//     OLED_DrawLine(69, 9, 69, 54);    
//     OLED_DrawLine(0, 42, 128, 42);
//     OLED_DrawLine(0, 54, 128, 54);

//     // �¶ȣ������Զ����У�
//     if(currentMode != MODE_AUTO){
//         OLED_ShowString(0, 0, "Temp:", OLED_6X8);
//         if(Temperature > 30) Temperature = 0;
//         OLED_ShowNum(30, 0, Temperature, 2, OLED_6X8); // �����¶�26
//         OLED_ShowString(45, 0, "C", OLED_6X8);
//         OLED_DrawPoint(43, 0);
//         OLED_DrawPoint(44, 0);
//         OLED_DrawPoint(43, 1);
//         OLED_DrawPoint(44, 1);
//     }


//     // ����ģʽ
//     if(currentMode == MODE_NONE) currentMode = MODE_AUTO;
//     OLED_ShowString(72, 0, "Mode:    ", OLED_6X8);
//     switch(currentMode) {
//         case MODE_AUTO:
//             OLED_ShowString(102, 0, "Auto", OLED_6X8);
//             break;
//         case MODE_COOL:
//             OLED_ShowString(102, 0, "Cool", OLED_6X8);
//             break;
//         case MODE_DRY:
//             OLED_ShowString(102, 0, "Dry ", OLED_6X8);
//             break;
//         case MODE_FAN:
//             OLED_ShowString(102, 0, "Fan ", OLED_6X8);
//             break;
//         case MODE_HEAT:
//             OLED_ShowString(102, 0, "Heat", OLED_6X8);
//             break;
//     }    

//     // ���٣����У�
//     OLED_ShowString(72, 11, "Fan:", OLED_6X8);
//     if(FanSpeed > 0 && FanSpeed <= 5) OLED_ShowNum(96, 11, FanSpeed, 1, OLED_6X8);
//     else{
//         OLED_ShowString(96, 11, "Auto", OLED_6X8);
//         FanSpeed = 0;
//     }
    
//     //  ��ǿ�����䡢����
//     if(currentMode == MODE_COOL || currentMode == MODE_HEAT){
//         OLED_ShowString(0, 11, "Super:", OLED_6X8);
//         if(Super > 0 && Super <= 1) OLED_ShowString(36, 11, "ON", OLED_6X8);
//         else{
//             OLED_ShowString(36, 11, "OFF", OLED_6X8);
//             Super = 0;
//         }
//     }

//     // ��ʾ��ǰɨ��ģʽ�����У�
//     if(currentSwing == SWING_NONE) currentSwing = SWING_OFF;
//     OLED_ShowString(72, 22, "Swing:", OLED_6X8);
//     switch(currentSwing) {
//         case SWING_OFF:
//             OLED_ShowString(108, 22, "OFF", OLED_6X8);
//             break;
//         case SWING_VERT:
//             OLED_ShowString(108, 22, "V  ", OLED_6X8); // ����
//             break;
//         case SWING_HORIZ:
//             OLED_ShowString(108, 22, "H  ", OLED_6X8); // ����
//             break;
//         case SWING_HORIZ_VERT:
//             OLED_ShowString(108, 22, "V&H  ", OLED_6X8); // ����&����
//             break;
//     }
    

//     // ˯��(���䡢��ʪ������)
//     if(currentMode == MODE_COOL || currentMode == MODE_DRY || currentMode == MODE_HEAT){    
//         OLED_ShowString(72, 33, "Sleep:", OLED_6X8);
//         if(Sleep > 0 && Sleep <= 3) OLED_ShowNum(108, 33, Sleep, 1, OLED_6X8);
//         else{
//             Sleep = 0;
//             OLED_ShowString(108, 33, "OFF", OLED_6X8);
//         }
//     }

//     //�ƹ�״̬�����У�
//     OLED_ShowString(72, 45, "Light:", OLED_6X8);
//     if(Light > 0 && Light <= 1) OLED_ShowString(108, 45, "ON", OLED_6X8);
//     else {
//         OLED_ShowString(108, 45, "OFF", OLED_6X8);
//         Light = 0;
//     }

//     // ���������У�
//     OLED_ShowString(0, 22, "Mute:", OLED_6X8);
//     if(currentMute == MUTE_NONE) currentMute = MUTE_OFF;
//     if(currentMute == MUTE_MUTE) OLED_ShowString(30, 22, "M  ", OLED_6X8);
//     if(currentMute == MUTE_OFF) OLED_ShowString(30, 22, "OFF", OLED_6X8);
//     if(currentMute == MUTE_AUTO_MUTE)  OLED_ShowString(30, 22, "A&M", OLED_6X8);
    
//     // ������䡢��ʪ��
//     if(currentMode == MODE_COOL || currentMode == MODE_DRY){
//         OLED_ShowString(0, 33, "Dry:", OLED_6X8);
//         if(Dry > 0 && Dry <= 1) OLED_ShowString(24, 33, "ON", OLED_6X8);
//         else{
//             OLED_ShowString(24, 33, "OFF", OLED_6X8);
//             Dry = 0;
//         }        
//     }
//     else if(currentMode == MODE_HEAT){
//         // ���ȣ����ȣ�
//         OLED_ShowString(0, 33, "Heat:", OLED_6X8);
//         if(Heat > 0 && Heat <= 1) OLED_ShowString(30, 33, "ON", OLED_6X8);
//         else {
//             OLED_ShowString(30, 33, "OFF", OLED_6X8);
//             Heat = 0;
//         }
//     }

//     // ����/���������У�
//     OLED_ShowString(0, 45, "Health:", OLED_6X8);
//     if(currentHealth == HEALTH_NONE) currentHealth = HEALTH_OFF;
//     switch (currentHealth)
//     {
//     case HEALTH_OFF:
//         OLED_ShowString(42, 45, "OFF", OLED_6X8);
//         break;
//     case HEALTH_H:
//         OLED_ShowString(42, 45, "H", OLED_6X8);      // ����
//         break;
//     case HEALTH_V:
//         OLED_ShowString(42, 45, "V", OLED_6X8);      // ����
//         break;
//     case HEALTH_HV:
//         OLED_ShowString(42, 45, "H&V", OLED_6X8);    // ����&����
//         break;
//     }

//     // ��ʱ�رգ����У�
//     OLED_ShowString(0, 56, "Tmr:", OLED_6X8);
//     OLED_ShowNum(24, 56, timerRingTime.day, 2, OLED_6X8);
//     OLED_ShowChar(36, 56, '-', OLED_6X8);
//     OLED_ShowNum(42, 56, timerRingTime.hour, 2, OLED_6X8);
//     OLED_ShowChar(54, 56, ':', OLED_6X8);
//     OLED_ShowNum(60, 56, timerRingTime.min, 2, OLED_6X8);
//     // OLED_ShowChar(88, 56, ':', OLED_6X8);
//     // OLED_ShowNum(96, 56, timerRingTime.min, 2, OLED_6X8);
    

//     // ���ܣ����䣩
//     if(currentMode == MODE_COOL){
//         OLED_ShowString(86, 56, "Eco:", OLED_6X8);
//         if(Eco > 0 && Eco <= 1) OLED_ShowString(106, 56, "ON", OLED_6X8);
//         else{
//             OLED_ShowString(106, 56, "OFF", OLED_6X8);
//             Eco = 0;
//         }        
//     }


//     OLED_Update();
// }

void GUI_IRRecvUI_Process(void) {
    // ������ս��洦��
    // TODO: ʵ�ֺ�����ս�����ʾ���߼�
}

void GUI_MenuUI_Process(void) {
    // �˵����洦��
    // TODO: ʵ�ֲ˵�������ʾ���߼�
}

void GUI_KeyboardCheckUI_Process(void) {

    KeyboardCode key_res = GetKeyboardCode();
    static uint8_t KeyDown_Counter = 0;
    if(key_res != 0) KeyDown_Counter++;

    OLED_Clear();

    // ��ť�ߴ�
    const uint8_t btn_w = 18, btn_h = 12;
    // ��Ļ����
    const uint8_t cx = 64;

    // �������
    OLED_ShowString((128 - 12 * 6) / 2, 0, "Keyboard Test", OLED_6X8);

    // �ָ���
    OLED_DrawLine(0, 12, 128, 12);

    // �ϰ�ť
    uint8_t up_x = cx - btn_w / 2;
    uint8_t up_y = 16;
    OLED_DrawRectangle(up_x, up_y, btn_w, btn_h, (key_res == KEY_CODE_U) ? OLED_FILLED : OLED_UNFILLED);
    OLED_ShowString(up_x + (btn_w - 6) / 2, up_y + (btn_h - 8) / 2, "U", OLED_6X8);

    // �м䰴ť
    uint8_t ok_x = cx - btn_w / 2;
    uint8_t ok_y = up_y + btn_h + 4;
    OLED_DrawRectangle(ok_x, ok_y, btn_w, btn_h, (key_res == KEY_CODE_O) ? OLED_FILLED : OLED_UNFILLED);
    OLED_ShowString(ok_x + (btn_w - 6) / 2, ok_y + (btn_h - 8) / 2, "O", OLED_6X8);

    // �°�ť
    uint8_t down_x = cx - btn_w / 2;
    uint8_t down_y = ok_y + btn_h + 4;
    OLED_DrawRectangle(down_x, down_y, btn_w, btn_h, (key_res == KEY_CODE_D) ? OLED_FILLED : OLED_UNFILLED);
    OLED_ShowString(down_x + (btn_w - 6) / 2, down_y + (btn_h - 8) / 2, "D", OLED_6X8);

    // ��ť
    uint8_t left_x = cx - 22 - btn_w / 2;
    uint8_t left_y = ok_y;
    OLED_DrawRectangle(left_x, left_y, btn_w, btn_h, (key_res == KEY_CODE_L) ? OLED_FILLED : OLED_UNFILLED);
    OLED_ShowString(left_x + (btn_w - 6) / 2, left_y + (btn_h - 8) / 2, "L", OLED_6X8);

    // �Ұ�ť
    uint8_t right_x = cx + 22 - btn_w / 2;
    uint8_t right_y = ok_y;
    OLED_DrawRectangle(right_x, right_y, btn_w, btn_h, (key_res == KEY_CODE_R) ? OLED_FILLED : OLED_UNFILLED);
    OLED_ShowString(right_x + (btn_w - 6) / 2, right_y + (btn_h - 8) / 2, "R", OLED_6X8);

    OLED_ShowString(0, 64 - 16, "Click:", OLED_6X8);
    OLED_ShowNum(0, 64 - 8, KeyDown_Counter, 3, OLED_6X8);

    OLED_Update();
}

void GUI_DisplayCheckUI_Process(void) {
    // ��ʾ�����洦��
    // TODO: ʵ����ʾ��������ʾ���߼�
}

void GUI_IRCheckUI_Process(void) {
    // ��������洦��
    // TODO: ʵ�ֺ����������ʾ���߼�
}

void GUI_SettingUI_Process(void) {
    // ���ý��洦��
    // TODO: ʵ�����ý�����ʾ���߼�
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
        // Ĭ�ϴ���
        GUI_IRSendUI_Process();
        break;
    }
}