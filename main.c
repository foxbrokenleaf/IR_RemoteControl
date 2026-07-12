#include "ws51f6240.h"
#include "OLED_Data.h"
#include "oled_0_69inch.h"


sbit IR = P1 ^ 1;       //P11F
/*
 * IR
*/
sbit Key_1 = P1 ^ 7;    //P17F
sbit Key_2 = P2 ^ 1;    //P21F
sbit Key_3 = P0 ^ 7;    //P07F
sbit Key_4 = P0 ^ 6;    //P06F
sbit Key_5 = P0 ^ 5;    //P05F
/*************************
 *       * Key_2 * Key_4 *
 * Key_1 * Key_3 * Key_5 *
 * ***********************
*/


/*
 * Define private funcation
*/
void SystemInit(void);      //初始化所需要的外设
void GuiManage(void);       //掌管GUI的神
void PowerFrame(void);      //Done
void ModeFrame(void);       //Done
void TemptureFrame(void);   //Done
void SpeedFrame(void);      //Done
void LightFrame(void);      //WIP
void SwingFrame(void);      //WIP
void SleepFrame(void);      //WIP
void AuxiliaryHot(void);    //WIP

/*
 *	Private var
 */
uint8_t tmpVar = 0;
uint8_t GuiIndex = 0;
uint8_t DataFrame_1 = 0xC0;
/***********************************
 * 0     * 1    * 2 ~ 6    * 7     *
 * Power * Mode * Tempture * Light *
 ***********************************
*/
uint8_t DataFrame_2 = 0x00;
/**********************************
 * 0 ~ 1 * 2 ~ 3 * 4     * 5      *
 * Speed * Swing * Sleep * AuxHot *
 **********************************
*/

static void delay(int i)
{
	long j;
	for(j=0;j<10*i;++j);
}

void main(){
    uint8_t KeyCode = 0;
    uint8_t Old_KeyCode = 0;
	
    SystemInit();
	OLED_Init();
    GuiManage();
    OLED_Update();
    OLED_Update();

    while(1){
        
        KeyCode = (uint8_t)(Key_1) | ((uint8_t)(Key_2) << 1) | ((uint8_t)(Key_3) << 2) | ((uint8_t)(Key_4) << 3) | ((uint8_t)(Key_5) << 4);

        GuiManage();

        if(Old_KeyCode != KeyCode){
            Old_KeyCode = KeyCode;
            tmpVar = 0;
            if(((KeyCode & 0x02) == 0x02) && (GuiIndex > 0)) GuiIndex--;
            if(((KeyCode & 0x04) == 0x04) && ((DataFrame_1 & 0x01) == 0x01)) GuiIndex++;
            if((KeyCode & 0x01) == 0x01){
                if(GuiIndex == 0){
                    tmpVar = (DataFrame_1 & 0x01);
                    tmpVar = ~tmpVar;
                    DataFrame_1 &= 0xFE;
                    DataFrame_1 |= (tmpVar & 0x01);
                }
                if(GuiIndex == 1){
                    tmpVar = (DataFrame_1 & 0x02);
                    tmpVar = ~tmpVar;
                    DataFrame_1 &= 0xFD;
                    DataFrame_1 |= (tmpVar & 0x02);                    
                }
                if((GuiIndex == 2) && ((DataFrame_1 & 0x7C) >> 2) > 16){
                    tmpVar = (DataFrame_1 & 0x7C) >> 2;
                    tmpVar--;
                    DataFrame_1 &= 0x83;
                    DataFrame_1 |= (tmpVar << 2);
                }
                if(GuiIndex == 3){
                    tmpVar = (DataFrame_2 & 0x03);
                    tmpVar--;
                    DataFrame_2 &= 0xFC;
                    DataFrame_2 |= tmpVar;
                }                
            }
            if((KeyCode & 0x10) == 0x10){
                if(GuiIndex == 0){
                    tmpVar = (DataFrame_1 & 0x01);
                    tmpVar = ~tmpVar;
                    DataFrame_1 &= 0xFE;
                    DataFrame_1 |= (tmpVar & 0x01);
                }            
                if(GuiIndex == 1){
                    tmpVar = (DataFrame_1 & 0x02);
                    tmpVar = ~tmpVar;
                    DataFrame_1 &= 0xFD;
                    DataFrame_1 |= (tmpVar & 0x02);                       
                }                    
                if((GuiIndex == 2) && ((DataFrame_1 & 0x7C) >> 2) < 31){
                    tmpVar = (DataFrame_1 & 0x7C) >> 2;
                    tmpVar++;
                    DataFrame_1 &= 0x83;
                    DataFrame_1 |= (tmpVar << 2);
                }
                if(GuiIndex == 3){
                    tmpVar = (DataFrame_2 & 0x03);
                    tmpVar++;
                    DataFrame_2 &= 0xFC;
                    DataFrame_2 |= tmpVar;
                }                 
            }            
            OLED_Update();
            OLED_Update();
            OLED_Clear();            
        }        

    }
}

void SystemInit(void){
    SCCON  = 0x00;//HRC
    HRCON |= 0x80;
    P11F = 0x02;
    P17F = 0x01;
    P21F = 0x01;
    P07F = 0x01;
    P06F = 0x01;
    P05F = 0x01;
}

void GuiManage(void){
    switch(GuiIndex){
        case 0:
            PowerFrame();
            break;
        case 1:
            ModeFrame();
            break;
        case 2:
            TemptureFrame();
            break;
        case 3:
            SpeedFrame();
            break;
        case 4:
            LightFrame();
            break;
        case 5:
            SwingFrame();
            break;
        case 6:
            SleepFrame();
            break; 
        case 7:
            AuxiliaryHot();
            break;
        default:
            OLED_ShowString(0, 4, "Undefine UI", OLED_6X8);
            break;
    }
}

void PowerFrame(void){
    (DataFrame_1 & 0x01) == 0x01 ? OLED_ShowString(0, 4, "Power : On", OLED_6X8) : OLED_ShowString(0, 4, "Power : Off", OLED_6X8);
}

void ModeFrame(void){
    (DataFrame_1 & 0x02) == 0x02 ? OLED_ShowString(0, 4, "Mode : Hot", OLED_6X8) : OLED_ShowString(0, 4, "Mode : Cold", OLED_6X8);
}

void TemptureFrame(void){
    OLED_ShowString(0, 4, "Tempture : XX", OLED_6X8);
    OLED_ShowNum(66, 4, (DataFrame_1 & 0x7C) >> 2, 2, OLED_6X8);
}

void SpeedFrame(void){
    OLED_ShowString(0, 4, "Speed : X", OLED_6X8);
    OLED_ShowNum(48, 4, (DataFrame_2 & 0x03), 1, OLED_6X8);
}

void LightFrame(void){
    OLED_ShowString(0, 4, "Light : On", OLED_6X8);
}

void SwingFrame(void){
    OLED_ShowString(0, 4, "Swing : V-SWING", OLED_6X8);
}

void SleepFrame(void){
    OLED_ShowString(0, 4, "Sleep : Off", OLED_6X8);
}

void AuxiliaryHot(void){
    OLED_ShowString(0, 4, "AuxHot : Off", OLED_6X8);
}
