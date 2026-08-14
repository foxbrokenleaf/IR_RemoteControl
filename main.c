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
sbit TF0 = TCON ^ 5;
sbit TR0 = TCON ^ 4;
sbit TF1 = TCON ^ 7;
sbit TR1 = TCON ^ 6;
sbit EA = IE ^ 7;
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
void TestMode(void);
void IrTask(void);
void IR_Send_Bit(bit v);
void IR_Send_Leader_Code(void);
void IR_Send_Repeat_Leader_Code(void);
void IR_Send_Interval_20ms();
void IR_Send_Interval_40ms();
uint8_t reverse_bits(uint8_t value, uint8_t bits);
void calculate_checksum();
void IR_Data_Updata(void);

/*
 *	Private var
 */
bit TickFlag = 0;
uint8_t tmpVar = 0;
uint8_t GuiIndex = 0;
uint8_t DataFrame_1 = 0xC0;
/***********************************
 * 0     * 1    * 2 ~ 6    * 7     *
 * Power * Mode * Tempture * Light *
 ***********************************
*/
uint8_t DataFrame_2 = 0x00;
/************************************************
 * 0 ~ 1 * 2 ~ 3 * 4     * 5      * 6           *
 * Speed * Swing * Sleep * AuxHot * DisplayFlag *
 ************************************************
*/
uint8_t MainFrame[5] = {0x10, 0x00, 0x00, 0x0A, 0x40};
uint8_t AuxFrame[4] = {0x00, 0x04, 0x00, 0x00};
uint8_t test_num = 0;

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

        IR_Data_Updata();
        GuiManage();

        if(Old_KeyCode != KeyCode){
            Old_KeyCode = KeyCode;
            tmpVar = 0;
            if(((KeyCode & 0x02) == 0x02) && (GuiIndex > 0)) GuiIndex--;
            if(((KeyCode & 0x04) == 0x04) && ((DataFrame_1 & 0x01) == 0x01)) GuiIndex++;
            //U2 左按键
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
                if(GuiIndex == 8){
                    test_num--;
                }                 
            }
            //U4 右按键
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
                if(GuiIndex == 8){
                    test_num++;
                }                 
            }
            if((KeyCode & 0x08) == 0x08){
                IrTask();
                // IR_Send_Bit(1);
                // IR_Send_Leader_Code();
                // IR_Send_Repeat_Leader_Code();
                // IR_Send_Interval_20ms();
                // IR_Send_Interval_40ms();
                OLED_ShowHexNum(0, 8, MainFrame[0], 2, OLED_6X8);
                OLED_ShowHexNum(12, 8, MainFrame[1], 2, OLED_6X8);
                OLED_ShowHexNum(24, 8, MainFrame[2], 2, OLED_6X8);
                OLED_ShowHexNum(36, 8, MainFrame[3], 2, OLED_6X8);
                OLED_ShowHexNum(48, 8, MainFrame[4], 2, OLED_6X8);

                OLED_ShowHexNum(72, 8, AuxFrame[3], 2, OLED_6X8);
                //128 064 000 010 064
            }
            else{
                // IR_Send_Bit(0);
            }
            DataFrame_2 |= 0x40;
        } 
    }

}

void SystemInit(void){
    SCCON  = 0x00;//HRC
    HRCON |= 0x80;//16MHz

    //Config Timer0
    TH0 = 0xFF;
    TL0 = 0xEE;
    TMOD &= 0xF0;
    TMOD |= 0x01; //配置为模式1：16位计数器
    TCON &= 0xCF;
    TCON |= 0x10;    
    //Config Timer1
    TH1 = 0x00;
    TL1 = 0x00;
    TMOD &= 0x0F;
    TMOD |= 0x10; //配置为模式1：16位计数器
    TCON &= 0x3F;
    TCON |= 0x40;  

    IE = 0x8A;
    IP = 0x0A;

    P11F = 0x02;
    P17F = 0x01;
    P21F = 0x01;
    P07F = 0x01;
    P06F = 0x01;
    P05F = 0x01;

    TR0 = 0;
    TR1 = 0;
    IR = 0;
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
        case 8:
            TestMode();
            break;
        default:
            OLED_ShowString(0, 4, "Undefine UI", OLED_6X8);
            break;
    }

    if((DataFrame_2 & 0x40) == 0x40){
        DataFrame_2 &= 0xBF;
        OLED_Update();
        OLED_Update();
        OLED_Clear();
    }

}

void PowerFrame(void){
    if((DataFrame_1 & 0x01) == 0x01) OLED_ShowString(0, 0, "Power : On", OLED_6X8);
    else OLED_ShowString(0, 0, "Power : Off", OLED_6X8);
}

void ModeFrame(void){
    if((DataFrame_1 & 0x02) == 0x02) OLED_ShowString(0, 0, "Mode : Hot", OLED_6X8);    
    else OLED_ShowString(0, 0, "Mode : Cold", OLED_6X8);
}

void TemptureFrame(void){
    OLED_ShowString(0, 0, "Tempture : XX", OLED_6X8);
    OLED_ShowNum(66, 0, (DataFrame_1 & 0x7C) >> 2, 2, OLED_6X8);
}

void SpeedFrame(void){
    OLED_ShowString(0, 0, "Speed : X", OLED_6X8);
    OLED_ShowNum(48, 0, (DataFrame_2 & 0x03), 1, OLED_6X8);
}

void LightFrame(void){
    OLED_ShowString(0, 0, "Light : On", OLED_6X8);
}

void SwingFrame(void){
    OLED_ShowString(0, 0, "Swing : V-SWING", OLED_6X8);
}

void SleepFrame(void){
    OLED_ShowString(0, 0, "Sleep : Off", OLED_6X8);
}

void AuxiliaryHot(void){
    OLED_ShowString(0, 0, "AuxHot : Off", OLED_6X8);
}

void TestMode(void){
    switch(test_num){
        case 0:
            OLED_ShowString(0, 0, "IR_Send", OLED_6X8);
            OLED_ShowString(0, 8, "LeaderCode()", OLED_6X8);            
            break;
        case 1:
            //IR_Send_Repeat_Leader_Code
            OLED_ShowString(0, 0, "IR_Send_Repeat", OLED_6X8);
            OLED_ShowString(0, 8, "_Leader_Code()", OLED_6X8);             
            break;
        case 2:
            //IR_Send_Interval_20ms
            OLED_ShowString(0, 0, "IR_Send_", OLED_6X8);
            OLED_ShowString(0, 8, "Interval_20ms", OLED_6X8);
            break;
        case 3:
            //IR_Send_Interval_40ms
            OLED_ShowString(0, 0, "IR_Send_", OLED_6X8);
            OLED_ShowString(0, 8, "Interval_40ms", OLED_6X8);
            break;
        case 4:
            OLED_ShowString(0, 0, "IR IO Statue", OLED_6X8);
            OLED_ShowString(0, 8, "IR = ", OLED_6X8);
            if(IR) OLED_ShowString(30, 8, "1", OLED_6X8);
            else OLED_ShowString(30, 8, "0", OLED_6X8);
            break;
        case 5:

            break;                                                
        default:
            break;
    }

}

void IrTask(void){
    uint8_t i = 0;
    IR_Send_Leader_Code();

    for(i = 0;i < 35;i++){
        if(MainFrame[i / 8] & (0x80 >> (i % 8))) IR_Send_Bit(1);
        else IR_Send_Bit(0);
    }
    
    IR_Send_Interval_20ms();

    for(i = 0;i < 32;i++){
        if(AuxFrame[i / 8] & (0x80 >> (i % 8))) IR_Send_Bit(1);
        else IR_Send_Bit(0);
    }

    IR_Send_Interval_40ms();

}

void IR_Data_Updata(void){
    //开关
    MainFrame[0] &= 0xEF;
    if((DataFrame_1 & 0x01) == 0x01) MainFrame[0] |= 0x10;
    else MainFrame[0] |= 0x20;

    //模式
    MainFrame[0] &= 0x1F;
    if((DataFrame_1 & 0x02) == 0x02) MainFrame[0] |= 0x20;
    else MainFrame[0] |= 0x80;

    //温度
    MainFrame[1] &= 0x0F;
    MainFrame[1] |= (reverse_bits(((DataFrame_1 & 0x7C) >> 2) - 16, 4) & 0x0F) << 4;

    //风速
    MainFrame[0] &= 0xF3;
    MainFrame[0] |= ((DataFrame_2 & 0x03) << 2);
    
    //灯光
    MainFrame[2] &= 0xFB;
    MainFrame[2] |= 0x04;

    //扫风
    MainFrame[0] &= 0xFD;
    MainFrame[0] |= 0x02;

    //睡眠
    MainFrame[0] &= 0xFE;
    MainFrame[0] |= 0x00;

    //辅热
    MainFrame[2] &= 0xFE;
    MainFrame[2] |= 0x00;

    calculate_checksum();
}

// 发送单个位（核心：对齐“载波+无载波”时序）
// 逻辑0：560us载波（HS0038低） + 560us无载波（HS0038高）
// 逻辑1：560us载波（HS0038低） + 1680us无载波（HS0038高）
void IR_Send_Bit(bit v) {
    // 第一步：发送560us载波（HS0038输出低，对应原始数据的“低电平段”）
    TH1 = 0xFD;
    TL1 = 0x15;
    TR0 = 1;
    TR1 = 1;
    IR = 0;
    // 560us ≈ 560/525 ≈ 1.06 tick → 用1个tick（550us，误差≈6%）
    while(!TickFlag);
    TickFlag = 0;
    TR0 = 0;
    IR = 1;
    // 第二步：根据位值发送无载波时长（HS0038输出高，对应原始数据的“高电平段”）
    if (v) {
        // 逻辑1：1680us无载波 → 1680/550≈3.05 tick → 用3个tick（1650us，误差≈1.8%）
        TH1 = 0xF7;
        TL1 = 0x40;
        TR1 = 1;
        IR = 1;
        while(!TickFlag);
        TickFlag = 0;
    } else {
        // 逻辑0：560us无载波 → 560/550≈1.02 tick → 用1个tick（550us，误差≈2%）
        TH1 = 0xFD;
        TL1 = 0x15;
        TR1 = 1;
        IR = 1;
        while(!TickFlag);
        TickFlag = 0;
    }
    TR1 = 0;
    TR0 = 0;
    IR = 1;    
}

// 发送引导码（9msL + 4.5msH，L=载波，H=无载波）
void IR_Send_Leader_Code(void) {

    // 9msL：9ms载波（HS0038低）→ 9000us / 550us ≈16.36 tick → 用16个tick（8800us，误差≈2.2%）
    TH1 = 0xD1;
    TL1 = 0x20;
    TR0 = 1;
    TR1 = 1;
    IR = 0;
    while(!TickFlag);
    TickFlag = 0;    
    TR0 = 0;
    IR = 1;    
    // 4.5msH：4.5ms无载波（HS0038高）→ 4500us / 550us≈8.18 tick → 用8个tick（4400us，误差≈2.2%）
    TH1 = 0xE8;
    TL1 = 0x90;
    TR1 = 1;
    IR = 1; 
    while(!TickFlag);
    TickFlag = 0;
    TR1 = 0;
    TR0 = 0;
    IR = 1;
}

// 发送重复引导码（适配原始数据中的9msL + 4.45msH）
void IR_Send_Repeat_Leader_Code(void) {
    // 9msL：9ms载波（HS0038低）→ 9000us / 550us ≈16.36 tick → 用16个tick（8800us，误差≈2.2%）
    TH1 = 0xD1;
    TL1 = 0x20;
    TR0 = 1;
    TR1 = 1;
    IR = 0;
    while(!TickFlag);
    TickFlag = 0;    
    TR0 = 0;
    IR = 1;    
    // 4.5msH：4.5ms无载波（HS0038高）→ 4500us / 550us≈8.18 tick → 用8个tick（4400us，误差≈2.2%）
    TH1 = 0xE8;
    TL1 = 0x90;
    TR1 = 1;
    IR = 1;
    while(!TickFlag);
    TickFlag = 0;
    TR1 = 0;
    TR0 = 0;
    IR = 1;
}

/**
 * @brief 红外发送间隔 20ms
 */
void IR_Send_Interval_20ms() {
    // 第一步：发送560us载波（HS0038输出低，对应原始数据的“低电平段”）
    TH1 = 0xFD;
    TL1 = 0x15;
    TR0 = 1;
    TR1 = 1;
    IR = 0;
    // 560us ≈ 560/525 ≈ 1.06 tick → 用1个tick（550us，误差≈6%）
    while(!TickFlag);
    TickFlag = 0;
    TR0 = 0;
    IR = 1;
    
    // 计算tick数：ms * 1000us / 550us per tick → 向上取整减少误差
    TH1 = 0x97;
    TL1 = 0xD5;
    TR0 = 1;
    TR1 = 1;
    IR = 1;
    while(!TickFlag);
    TickFlag = 0;
    TR0 = 0;
    TR1 = 0;
    IR = 1;
}

/**
 * @brief 红外发送间隔 40ms
 */
void IR_Send_Interval_40ms(){
    // 第一步：发送560us载波（HS0038输出低，对应原始数据的“低电平段”）
    TH1 = 0xFD;
    TL1 = 0x15;
    TR0 = 1;
    TR1 = 1;
    IR = 0;
    // 560us ≈ 560/525 ≈ 1.06 tick → 用1个tick（550us，误差≈6%）
    while(!TickFlag);
    TickFlag = 0;
    TR0 = 0;
    IR = 1;
    
    // 计算tick数：ms * 1000us / 550us per tick → 向上取整减少误差
    TH1 = 0x2F;
    TL1 = 0xAB;
    TR0 = 1;
    TR1 = 1;
    IR = 1;
    while(!TickFlag);
    TickFlag = 0;
    TR0 = 0;
    TR1 = 0;
    IR = 1;
}

// 反转n位二进制数的辅助函数
uint8_t reverse_bits(uint8_t value, uint8_t bits) {
    uint8_t result = 0;
    uint8_t i = 0;

    for (i = 0; i < bits; i++) {
        result <<= 1;
        result |= (value & 1);
        value >>= 1;
    }
    return result;
}

/**
 * 计算校验码：(mode - 1) + (temp - 16) + 5 取低4位后反序
 * 
 * 参数:
 *   mode: 模式值
 *   temp: 温度值
 * 
 * 返回:
 *   计算得到的校验码
 */
void calculate_checksum() {
    // 计算基础值
    // uint8_t base_value = (mode - 1) + (temp - 16) + 5 + horizontal_swing + ventilate;
    uint8_t base_value = 0;
    uint8_t low4 = 0;
    uint8_t checksum = 0;
    uint8_t i = 0;

    if(DataFrame_1 & 0x02) base_value += 3;
    else base_value += 0;

    base_value += ((DataFrame_1 & 0x7C) >> 2) - 16;

    base_value += 5;

    base_value += 0;

    base_value += 0;
    
    // 取低4位（即对16取模）
    low4 = base_value % 16;
    // 处理负数情况，确保结果为正数
    // if (low4 < 0) {
    //     low4 += 16;
    // }
    
    // 反序4位二进制
    checksum = 0;
    for (i = 0; i < 4; i++) {
        // 取出最低位并移到对应位置
        checksum |= ((low4 >> i) & 1) << (3 - i);
    }
    
    AuxFrame[3] &= 0xF0;
    AuxFrame[3] |= checksum;

}

void Timer0_Isr(void) interrupt 1
{
    TF0 = 0;
    TH0 = 0xFF;
    TL0 = 0xF0;
    TR0 = 1;
    IR = ~IR;
}

void Timer1_Isr(void) interrupt 3
{
    TF1 = 0;
    TR1 = 0;
    TickFlag = 1;
}