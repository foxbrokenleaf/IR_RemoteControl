/***************************************************************************************
 * OLED Driver for 0.69" 96x16 SSD1306 (I2C)
 * Platform: WS51F6240 (8051)
 * Kept functions: ShowChar, ShowString, ShowNum, ShowFloatNum,
 *                 DrawPoint, DrawLine, DrawDashedLine,
 *                 DrawRectangle, DrawTriangle, DrawCircle
 ***************************************************************************************/

#include "ws51f6240.h"
#include "oled_0_69inch.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <intrins.h>

#define SOFTWARE_I2C
// #define HARDWARE_I2C

/* Pin definitions (modify according to your wiring) */
sbit OLED_SCL_Pin = P1^3;
sbit OLED_SDA_Pin = P1^4;
sbit OLED_RST_Pin = P1^2;

/* Pin write macros */
#define OLED_W_SCL(x)   do { OLED_SCL_Pin = (x); } while(0)
#define OLED_W_SDA(x)   do { OLED_SDA_Pin = (x); } while(0)
#define OLED_W_RST(x)   do { OLED_RST_Pin = (x); } while(0)

/* Display buffer explicitly placed in XDATA */
uint8_t xdata OLED_DisplayBuf[2][96];

/* I2C slave address */
#define OLED_ADDR   0X78

/* External font data (defined in another source file, e.g., font.c) */
extern const uint8_t code OLED_F8x16[][16];
extern const uint8_t code OLED_F6x8[][6];

/*-------------------------------------------------------------------------
 * GPIO initialization (reset pulse only)
 *------------------------------------------------------------------------*/
static void OLED_GPIO_Init(void)
{
    uint32_t i, j;
    for (i = 0; i < 1000; i++)
        for (j = 0; j < 1000; j++);

    P14F = 0x22;
    P13F = 0x02;
    P12F = 0x02;

    OLED_W_SCL(1);
    
    OLED_W_SDA(1);
    OLED_W_RST(0);
    OLED_W_RST(1);
}

// static void OLED_GPIO_Init(void){

//     uint32_t i, j;
//     for (i = 0; i < 1000; i++)
//         for (j = 0; j < 1000; j++);

//     P14F = 0xA5;
//     P13F = 0xA5;
//     P12F = 0x02;

//     I2CTXD = OLED_ADDR;

//     I2CCON = 0x83;
//     I2CCFG0 = 0x00;
//     I2CFG1 = 0x0C;

//     OLED_W_RST(0);
//     OLED_W_RST(1);
    
// }

/*-------------------------------------------------------------------------
 * Software I2C primitives
 *------------------------------------------------------------------------*/
static void OLED_I2C_Start(void)
{
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    OLED_W_SDA(0);
    OLED_W_SCL(0);
}

// static void OLED_I2C_Start(void)
// {
//     I2CTXD = OLED_ADDR;
//     I2CCON = 0x8B;
// }

static void OLED_I2C_Stop(void)
{
    OLED_W_SDA(0);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

// static void OLED_I2C_Stop(void)
// {
//     I2CCON = 0x87;
// }

static void OLED_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++) {
        OLED_W_SDA((Byte & (0x80 >> i)));
        OLED_W_SCL(1);
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();
        OLED_W_SCL(0);
    }
    OLED_W_SCL(1);
    OLED_W_SCL(0);
}

// #define OLED_I2C_SendByte(x) I2CTXD = x

static void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(OLED_ADDR);
    OLED_I2C_SendByte(0x00);
    OLED_I2C_SendByte(Command);
    OLED_I2C_Stop();
}

// static void OLED_WriteCommand(uint8_t cmd){
//     OLED_I2C_Start();
//     I2CFLG &= 0xC0;
//     I2CTXD = 0x00;
//     I2CFLG &= 0xC0;
//     I2CTXD = cmd;
//     I2CFLG &= 0xC0;
//     OLED_I2C_Stop();
// }

/* Data pointer must be xdata to access the buffer */
void OLED_WriteData(uint8_t xdata *Data, uint8_t Count)
{
    uint8_t i;
    OLED_I2C_Start();
    OLED_I2C_SendByte(OLED_ADDR);
    OLED_I2C_SendByte(0x40);
    for (i = 0; i < Count; i++)
        OLED_I2C_SendByte(Data[i]);
    OLED_I2C_Stop();
}

// static void OLED_WriteData(uint8_t xdata *Data, uint8_t Count){
//     uint8_t i;
//     OLED_I2C_Start();
//     I2CFLG &= 0xC0;
//     I2CTXD = 0x40;
//     I2CFLG &= 0xC0;
//     for (i = 0; i < Count; i++)
//         I2CTXD = Data[i];
//         I2CFLG &= 0xC0;
//     OLED_I2C_Stop();
// }

/*-------------------------------------------------------------------------
 * OLED initialization sequence (96x16)
 *------------------------------------------------------------------------*/
void OLED_Init(void)
{
    OLED_GPIO_Init();

    OLED_WriteCommand(0xAE);
    OLED_WriteCommand(0xD5); OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8); OLED_WriteCommand(0x0F);
    OLED_WriteCommand(0xD3); OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40);
    OLED_WriteCommand(0x8D); OLED_WriteCommand(0x14);
    OLED_WriteCommand(0x20); OLED_WriteCommand(0x00);
    OLED_WriteCommand(0xA1);
    OLED_WriteCommand(0xC8);
    OLED_WriteCommand(0xDA); OLED_WriteCommand(0x02);
    OLED_WriteCommand(0x81); OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9); OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB); OLED_WriteCommand(0x40);
    OLED_WriteCommand(0xA4);
    OLED_WriteCommand(0xA6);
    OLED_WriteCommand(0xAF);

    OLED_Clear();
    OLED_Update();
}

/*-------------------------------------------------------------------------
 * Set cursor (page 0~1, X 0~95)
 *------------------------------------------------------------------------*/
void OLED_SetCursor(uint8_t Page, uint8_t X)
{
    if (Page > 1) Page = 1;
    if (X > 95) X = 95;
    OLED_WriteCommand(0xB0 | Page);
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));
    OLED_WriteCommand(0x00 | (X & 0x0F));
}

/*-------------------------------------------------------------------------
 * Update whole buffer to screen
 *------------------------------------------------------------------------*/
void OLED_Update(void)
{
    uint8_t j;
    for (j = 0; j < 2; j++) {
        OLED_SetCursor(j, 0);
        OLED_WriteData(OLED_DisplayBuf[j], 96);
    }
}

/*-------------------------------------------------------------------------
 * Clear whole buffer
 *------------------------------------------------------------------------*/
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 2; j++)
        for (i = 0; i < 96; i++)
            OLED_DisplayBuf[j][i] = 0x00;
}

/*-------------------------------------------------------------------------
 * Internal: clear a rectangular area in buffer
 *------------------------------------------------------------------------*/
static void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
    int16_t i, j;
    for (j = Y; j < Y + Height; j++)
        for (i = X; i < X + Width; i++)
            if (i >= 0 && i <= 95 && j >= 0 && j <= 15)
                OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8));
}

/*-------------------------------------------------------------------------
 * Internal: display an image bitmap (font data resides in CODE)
 *------------------------------------------------------------------------*/
static void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height,
                           const uint8_t code *Image)
{
    uint8_t i = 0, j = 0;
    int16_t Page, Shift;

    if (X + Width > 96) Width = 96 - X;
    if (Y + Height > 16) Height = 16 - Y;

    OLED_ClearArea(X, Y, Width, Height);

    for (j = 0; j < (Height - 1) / 8 + 1; j++) {
        for (i = 0; i < Width; i++) {
            if (X + i >= 0 && X + i <= 95) {
                Page = Y / 8;
                Shift = Y % 8;
                if (Y < 0) {
                    Page -= 1;
                    Shift += 8;
                }
                if (Page + j >= 0 && Page + j <= 1)
                    OLED_DisplayBuf[Page + j][X + i] |= Image[j * Width + i] << Shift;
                if (Page + j + 1 >= 0 && Page + j + 1 <= 1)
                    OLED_DisplayBuf[Page + j + 1][X + i] |= Image[j * Width + i] >> (8 - Shift);
            }
        }
    }
}

/*-------------------------------------------------------------------------
 * Power function (for number display)
 *------------------------------------------------------------------------*/
static uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--) Result *= X;
    return Result;
}

/*-------------------------------------------------------------------------
 * Public: draw a point
 *------------------------------------------------------------------------*/
void OLED_DrawPoint(int16_t X, int16_t Y)
{
    if (X >= 0 && X <= 95 && Y >= 0 && Y <= 15)
        OLED_DisplayBuf[Y / 8][X] |= 0x01 << (Y % 8);
}

/*-------------------------------------------------------------------------
 * Public: display a character
 *------------------------------------------------------------------------*/
void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize)
{
    if (FontSize == OLED_8X16) {
        if (X + 8 > 96 || Y + 16 > 16) return;
        OLED_ShowImage(X, Y, 8, 16, OLED_F8x16[Char - ' ']);
    } else if (FontSize == OLED_6X8) {
        if (X + 6 > 96 || Y + 8 > 16) return;
        OLED_ShowImage(X, Y, 6, 8, OLED_F6x8[Char - ' ']);
    }
}

/*-------------------------------------------------------------------------
 * Public: display a string
 *------------------------------------------------------------------------*/
void OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize)
{
    uint16_t i = 0;
    uint16_t XOffset = 0;
    uint8_t charWidth = (FontSize == OLED_8X16) ? 8 : 6;

    while (String[i] != '\0') {
        if (X + XOffset + charWidth <= 96) {
            OLED_ShowChar(X + XOffset, Y, String[i], FontSize);
            XOffset += charWidth;
        } else break;
        i++;
    }
}

/*-------------------------------------------------------------------------
 * Public: display an unsigned integer
 *------------------------------------------------------------------------*/
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length,
                  uint8_t FontSize)
{
    uint8_t i;
    uint8_t charWidth = (FontSize == OLED_8X16) ? 8 : 6;
    for (i = 0; i < Length; i++) {
        if (X + i * charWidth + charWidth <= 96) {
            OLED_ShowChar(X + i * charWidth, Y,
                          Number / OLED_Pow(10, Length - i - 1) % 10 + '0',
                          FontSize);
        }
    }
}

/*-------------------------------------------------------------------------
 * Public: display a float (simplified: sign, integer part, and dot)
 *------------------------------------------------------------------------*/
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number,
                       uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
{
    uint32_t IntNum;
    uint8_t charWidth = (FontSize == OLED_8X16) ? 8 : 6;

    if (Number >= 0) {
        if (X + charWidth <= 96) OLED_ShowChar(X, Y, '+', FontSize);
        IntNum = (uint32_t)Number;
    } else {
        if (X + charWidth <= 96) OLED_ShowChar(X, Y, '-', FontSize);
        IntNum = (uint32_t)(-Number);
    }

    OLED_ShowNum(X + charWidth, Y, IntNum, IntLength, FontSize);

    if (X + (IntLength + 1) * charWidth + charWidth <= 96)
        OLED_ShowChar(X + (IntLength + 1) * charWidth, Y, '.', FontSize);

    /* Fractional part omitted in this simplified version */
}

/**
  * 函    数：OLED显示十六进制数字（十六进制，正整数）
  * 参    数：X 指定数字左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定数字左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Number 指定要显示的数字，范围：0x00000000~0xFFFFFFFF
  * 参    数：Length 指定数字的长度，范围：0~8
  * 参    数：FontSize 指定字体大小
  *           范围：OLED_8X16		宽8像素，高16像素
  *                 OLED_6X8		宽6像素，高8像素
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i, SingleNumber;
    uint8_t charWidth = (FontSize == OLED_8X16) ? 8 : 6;

	for (i = 0; i < Length; i++)		//遍历数字的每一位
	{
		/*以十六进制提取数字的每一位*/
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		
		if (SingleNumber < 10)			//单个数字小于10
		{
			/*调用OLED_ShowChar函数，显示此数字*/
			/*+ '0' 可将数字转换为字符格式*/
			OLED_ShowChar(X + i * charWidth, Y, SingleNumber + '0', FontSize);
		}
		else							//单个数字大于10
		{
			/*调用OLED_ShowChar函数，显示此数字*/
			/*+ 'A' 可将数字转换为从A开始的十六进制字符*/
			OLED_ShowChar(X + i * charWidth, Y, SingleNumber - 10 + 'A', FontSize);
		}
	}
}

/*-------------------------------------------------------------------------
 * Public: draw a line (Bresenham)
 *------------------------------------------------------------------------*/
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1)
{
    int16_t x, y, dx, dy, d, incrE, incrNE, temp;
    int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
    uint8_t yflag = 0, xyflag = 0;

    if (y0 == y1) {
        if (x0 > x1) { temp = x0; x0 = x1; x1 = temp; }
        for (x = x0; x <= x1; x++) OLED_DrawPoint(x, y0);
    } else if (x0 == x1) {
        if (y0 > y1) { temp = y0; y0 = y1; y1 = temp; }
        for (y = y0; y <= y1; y++) OLED_DrawPoint(x0, y);
    } else {
        if (x0 > x1) {
            temp = x0; x0 = x1; x1 = temp;
            temp = y0; y0 = y1; y1 = temp;
        }
        if (y0 > y1) {
            y0 = -y0; y1 = -y1; yflag = 1;
        }
        if (y1 - y0 > x1 - x0) {
            temp = x0; x0 = y0; y0 = temp;
            temp = x1; x1 = y1; y1 = temp;
            xyflag = 1;
        }
        dx = x1 - x0;
        dy = y1 - y0;
        incrE = 2 * dy;
        incrNE = 2 * (dy - dx);
        d = 2 * dy - dx;
        x = x0; y = y0;

        if (yflag && xyflag) OLED_DrawPoint(y, -x);
        else if (yflag)      OLED_DrawPoint(x, -y);
        else if (xyflag)     OLED_DrawPoint(y, x);
        else                 OLED_DrawPoint(x, y);

        while (x < x1) {
            x++;
            if (d < 0) d += incrE;
            else { y++; d += incrNE; }

            if (yflag && xyflag) OLED_DrawPoint(y, -x);
            else if (yflag)      OLED_DrawPoint(x, -y);
            else if (xyflag)     OLED_DrawPoint(y, x);
            else                 OLED_DrawPoint(x, y);
        }
    }
}

/*-------------------------------------------------------------------------
 * Public: draw a dashed line (fixed pattern)
 *------------------------------------------------------------------------*/
void OLED_DrawDashedLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1)
{
    const uint8_t dashLength = 3;
    const uint8_t gapLength = 2;
    int16_t x, y, dx, dy, d, incrE, incrNE, temp;
    int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
    uint8_t yflag = 0, xyflag = 0;
    uint16_t pointCount = 0;

    if (y0 == y1) {
        if (x0 > x1) { temp = x0; x0 = x1; x1 = temp; }
        for (x = x0; x <= x1; x++) {
            if (pointCount < dashLength) OLED_DrawPoint(x, y0);
            pointCount = (pointCount + 1) % (dashLength + gapLength);
        }
    } else if (x0 == x1) {
        if (y0 > y1) { temp = y0; y0 = y1; y1 = temp; }
        for (y = y0; y <= y1; y++) {
            if (pointCount < dashLength) OLED_DrawPoint(x0, y);
            pointCount = (pointCount + 1) % (dashLength + gapLength);
        }
    } else {
        if (x0 > x1) {
            temp = x0; x0 = x1; x1 = temp;
            temp = y0; y0 = y1; y1 = temp;
        }
        if (y0 > y1) {
            y0 = -y0; y1 = -y1; yflag = 1;
        }
        if (y1 - y0 > x1 - x0) {
            temp = x0; x0 = y0; y0 = temp;
            temp = x1; x1 = y1; y1 = temp;
            xyflag = 1;
        }
        dx = x1 - x0;
        dy = y1 - y0;
        incrE = 2 * dy;
        incrNE = 2 * (dy - dx);
        d = 2 * dy - dx;
        x = x0; y = y0;

        if (pointCount < dashLength) {
            if (yflag && xyflag) OLED_DrawPoint(y, -x);
            else if (yflag)      OLED_DrawPoint(x, -y);
            else if (xyflag)     OLED_DrawPoint(y, x);
            else                 OLED_DrawPoint(x, y);
        }
        pointCount = (pointCount + 1) % (dashLength + gapLength);

        while (x < x1) {
            x++;
            if (d < 0) d += incrE;
            else { y++; d += incrNE; }

            if (pointCount < dashLength) {
                if (yflag && xyflag) OLED_DrawPoint(y, -x);
                else if (yflag)      OLED_DrawPoint(x, -y);
                else if (xyflag)     OLED_DrawPoint(y, x);
                else                 OLED_DrawPoint(x, y);
            }
            pointCount = (pointCount + 1) % (dashLength + gapLength);
        }
    }
}

/*-------------------------------------------------------------------------
 * Public: draw a rectangle (filled or outline)
 *------------------------------------------------------------------------*/
void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height,
                        uint8_t IsFilled)
{
    int16_t i, j;
    if (X + Width > 96) Width = 96 - X;
    if (Y + Height > 16) Height = 16 - Y;

    if (!IsFilled) {
        for (i = X; i < X + Width; i++) {
            OLED_DrawPoint(i, Y);
            OLED_DrawPoint(i, Y + Height - 1);
        }
        for (i = Y; i < Y + Height; i++) {
            OLED_DrawPoint(X, i);
            OLED_DrawPoint(X + Width - 1, i);
        }
    } else {
        for (i = X; i < X + Width; i++)
            for (j = Y; j < Y + Height; j++)
                OLED_DrawPoint(i, j);
    }
}

/*-------------------------------------------------------------------------
 * Public: draw a triangle (only outline even if filled is requested)
 *------------------------------------------------------------------------*/
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1,
                       int16_t X2, int16_t Y2, uint8_t IsFilled)
{
    OLED_DrawLine(X0, Y0, X1, Y1);
    OLED_DrawLine(X0, Y0, X2, Y2);
    OLED_DrawLine(X1, Y1, X2, Y2);
}

/*-------------------------------------------------------------------------
 * Public: draw a circle (Bresenham, radius limited to 8)
 *------------------------------------------------------------------------*/
void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled)
{
    int16_t x, y, d;
    if (Radius > 8) Radius = 8;

    d = 1 - Radius;
    x = 0;
    y = Radius;

    OLED_DrawPoint(X + x, Y + y);
    OLED_DrawPoint(X - x, Y - y);
    OLED_DrawPoint(X + y, Y + x);
    OLED_DrawPoint(X - y, Y - x);

    while (x < y) {
        x++;
        if (d < 0) d += 2 * x + 1;
        else { y--; d += 2 * (x - y) + 1; }

        OLED_DrawPoint(X + x, Y + y);
        OLED_DrawPoint(X + y, Y + x);
        OLED_DrawPoint(X - x, Y - y);
        OLED_DrawPoint(X - y, Y - x);
        OLED_DrawPoint(X + x, Y - y);
        OLED_DrawPoint(X + y, Y - x);
        OLED_DrawPoint(X - x, Y + y);
        OLED_DrawPoint(X - y, Y + x);
    }
}