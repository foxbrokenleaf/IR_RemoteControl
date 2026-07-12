#include "oled_0_69inch.h"
#include "OLED_Data.h"
#include <string.h>

// 显存数组：2页 × 96列
unsigned char OLED_DisplayBuf[2][96];

/*------------------ 简易延时（根据主频调整） ------------------*/
static void OLED_Delay_us(unsigned int us)
{
    while (us--)
    {
        unsigned char i = 12;    // 约1us @ 12MHz，若主频不同请调整
        while (i--);
    }
}

/*------------------ I2C 底层函数 ------------------*/
static void OLED_I2C_Start(void)
{
    OLED_SDA_HIGH();
    OLED_SCL_HIGH();
    OLED_Delay_us(1);
    OLED_SDA_LOW();
    OLED_Delay_us(1);
    OLED_SCL_LOW();
}

static void OLED_I2C_Stop(void)
{
    OLED_SDA_LOW();
    OLED_SCL_HIGH();
    OLED_Delay_us(1);
    OLED_SDA_HIGH();
    OLED_Delay_us(1);
}

static void OLED_I2C_SendByte(unsigned char byte)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        if (byte & (0x80 >> i))
            OLED_SDA_HIGH();
        else
            OLED_SDA_LOW();
        OLED_SCL_HIGH();
        OLED_Delay_us(1);
        OLED_SCL_LOW();
        OLED_Delay_us(1);
    }
    // 释放SDA，忽略应答
    OLED_SDA_HIGH();
    OLED_SCL_HIGH();
    OLED_Delay_us(1);
    OLED_SCL_LOW();
}

static void OLED_WriteCommand(unsigned char cmd)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78);      // 从机地址（默认0x78）
    OLED_I2C_SendByte(0x00);      // 控制字节：命令
    OLED_I2C_SendByte(cmd);
    OLED_I2C_Stop();
}

static void OLED_WriteData(unsigned char *pData, unsigned char len)
{
    unsigned char i;
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78);
    OLED_I2C_SendByte(0x40);      // 控制字节：数据
    for (i = 0; i < len; i++)
        OLED_I2C_SendByte(pData[i]);
    OLED_I2C_Stop();
}

/*------------------ 光标设置 ------------------*/
static void OLED_SetCursor(unsigned char page, unsigned char x)
{
    if (page > 1) page = 1;
    if (x > 95) x = 95;
    OLED_WriteCommand(0xB0 | page);
    OLED_WriteCommand(0x10 | ((x & 0xF0) >> 4));
    OLED_WriteCommand(0x00 | (x & 0x0F));
}

/*------------------ 初始化 ------------------*/
void OLED_Init(void)
{
    unsigned int i;

    P14F = 0x22;
    P13F = 0x02;
    P12F = 0x02;

    for (i = 0; i < 10000; i++);   // 上电延时

    OLED_WriteCommand(0xAE);
    OLED_WriteCommand(0xD5);
    OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8);
    OLED_WriteCommand(0x0F);        // 多路复用 16-1
    OLED_WriteCommand(0xD3);
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40);
    OLED_WriteCommand(0xA1);
    OLED_WriteCommand(0xC8);
    OLED_WriteCommand(0xDA);
    OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81);
    OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9);
    OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB);
    OLED_WriteCommand(0x30);
    OLED_WriteCommand(0xA4);
    OLED_WriteCommand(0xA6);
    OLED_WriteCommand(0x8D);
    OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF);

    OLED_Clear();
    OLED_Update();
}

/*------------------ 更新显存 ------------------*/
void OLED_Update(void)
{
    unsigned char page;
    for (page = 0; page < 2; page++)
    {
        OLED_SetCursor(page, 0);
        OLED_WriteData(OLED_DisplayBuf[page], 96);
    }
}

/*------------------ 清屏 ------------------*/
void OLED_Clear(void)
{
    unsigned char page, col;
    for (page = 0; page < 2; page++)
        for (col = 0; col < 96; col++)
            OLED_DisplayBuf[page][col] = 0x00;
}

/*------------------ 辅助：次方 ------------------*/
static unsigned int OLED_Pow(unsigned int base, unsigned char exp)
{
    unsigned int result = 1;
    while (exp--) result *= base;
    return result;
}

/*------------------ 显示字符 ------------------*/
void OLED_ShowChar(int X, int Y, char Char, unsigned char FontSize)
{
    const unsigned char code *pFont;
    unsigned char w, h;
    unsigned char page, shift;
    unsigned char i, j;
    unsigned char byteData;

    if (X < 0 || Y < 0 || X >= 96 || Y >= 16) return;

    if (FontSize == OLED_8X16)
    {
        w = 8; h = 16;
        pFont = OLED_F8x16[Char - ' '];   // 取消注释，并确保该数组已定义
    }
    else if (FontSize == OLED_6X8)
    {
        w = 6; h = 8;
        pFont = OLED_F6x8[Char - ' '];
    }
    else return;

    page = (unsigned char)(Y / 8);
    shift = (unsigned char)(Y % 8);

    for (i = 0; i < w; i++)
    {
        if (X + i >= 96) break;
        for (j = 0; j < h/8; j++)
        {
            byteData = pFont[j * w + i];
            // 与原始驱动一致：左移进入当前页，右移进入下一页
            if (page + j < 2)
                OLED_DisplayBuf[page + j][X + i] |= byteData << shift;
            if (shift && (page + j + 1 < 2))
                OLED_DisplayBuf[page + j + 1][X + i] |= byteData >> (8 - shift);
        }
    }
}

/*------------------ 显示字符串 ------------------*/
void OLED_ShowString(int X, int Y, char *String, unsigned char FontSize)
{
    while (*String)
    {
        OLED_ShowChar(X, Y, *String++, FontSize);
        X += FontSize;
        if (X >= 96) break;
    }
}

/*------------------ 显示十进制数 ------------------*/
void OLED_ShowNum(int X, int Y, unsigned int Number, unsigned char Length, unsigned char FontSize)
{
    unsigned char i;
    unsigned char digit;
    for (i = 0; i < Length; i++)
    {
        digit = (unsigned char)(Number / OLED_Pow(10, Length - i - 1) % 10);
        OLED_ShowChar(X + i * FontSize, Y, digit + '0', FontSize);
    }
}

/*------------------ 显示十六进制 ------------------*/
void OLED_ShowHexNum(int X, int Y, unsigned int Number, unsigned char Length, unsigned char FontSize)
{
    unsigned char i, digit;
    for (i = 0; i < Length; i++)
    {
        digit = (unsigned char)(Number / OLED_Pow(16, Length - i - 1) % 16);
        if (digit < 10)
            OLED_ShowChar(X + i * FontSize, Y, digit + '0', FontSize);
        else
            OLED_ShowChar(X + i * FontSize, Y, digit - 10 + 'A', FontSize);
    }
}

/*------------------ 显示二进制 ------------------*/
void OLED_ShowBinNum(int X, int Y, unsigned int Number, unsigned char Length, unsigned char FontSize)
{
    unsigned char i;
    unsigned char bitValue;
    for (i = 0; i < Length; i++)
    {
        bitValue = (unsigned char)(Number / OLED_Pow(2, Length - i - 1) % 2);
        OLED_ShowChar(X + i * FontSize, Y, bitValue + '0', FontSize);
    }
}

/*------------------ 画点 ------------------*/
void OLED_DrawPoint(int X, int Y)
{
    if (X < 0 || X >= 96 || Y < 0 || Y >= 16) return;
    OLED_DisplayBuf[Y / 8][X] |= (1 << (Y % 8));
}

/*------------------ 读点 ------------------*/
unsigned char OLED_GetPoint(int X, int Y)
{
    if (X < 0 || X >= 96 || Y < 0 || Y >= 16) return 0;
    return (OLED_DisplayBuf[Y / 8][X] >> (Y % 8)) & 0x01;
}