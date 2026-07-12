#ifndef __OLED_H
#define __OLED_H

#include "ws51f6240.h"      // 包含寄存器定义

/*------------------ 引脚配置（可修改） ------------------*/

sbit P14 = P1 ^ 4;
sbit P13 = P1 ^ 3;
sbit P12 = P1 ^ 2;

#define OLED_SCL_PIN   P13    // P1.3
#define OLED_SDA_PIN   P14    // P1.4
#define OLED_RST_PIN   P12    // P1.2

#define OLED_SCL_HIGH()  OLED_SCL_PIN = 1
#define OLED_SCL_LOW()   OLED_SCL_PIN = 0
#define OLED_SDA_HIGH()  OLED_SDA_PIN = 1
#define OLED_SDA_LOW()   OLED_SDA_PIN = 0

/*------------------ 字体大小定义 ------------------*/
#define OLED_8X16       8
#define OLED_6X8        6

/*------------------ 函数声明 ------------------*/
void OLED_Init(void);
void OLED_Update(void);
void OLED_Clear(void);

void OLED_ShowChar(int X, int Y, char Char, unsigned char FontSize);
void OLED_ShowString(int X, int Y, char *String, unsigned char FontSize);
void OLED_ShowNum(int X, int Y, unsigned int Number, unsigned char Length, unsigned char FontSize);
void OLED_ShowHexNum(int X, int Y, unsigned int Number, unsigned char Length, unsigned char FontSize);
void OLED_ShowBinNum(int X, int Y, unsigned int Number, unsigned char Length, unsigned char FontSize);

void OLED_DrawPoint(int X, int Y);
unsigned char OLED_GetPoint(int X, int Y);

#endif