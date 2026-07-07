/***************************************************************************************
  * 精简版 OLED 驱动（96x16），仅支持 6x8 ASCII 字符，所有绘图功能完整
  ***************************************************************************************
  */
#include "oled_0_69inch.h"
#include "OLED_Data.h"
#include "ws51f6240.h"
#include <string.h>
#include <math.h>

#define round(x)  ((x) >= 0 ? (int)((x) + 0.5) : (int)((x) - 0.5))

/* ---------- 引脚定义 ---------- */
sbit OLED_SCL = P1^0;
sbit OLED_SDA = P1^1;

static void OLED_Delay(unsigned long us)
{
    unsigned long i;
    while (us--)
        for (i = 0; i < 10; i++);
}

uint8_t xdata OLED_DisplayBuf[OLED_PAGES][OLED_WIDTH];
#define OLED_ADDR 0x7A

/* ==================== 引脚操作 ==================== */
void OLED_W_SCL(uint8_t BitValue) { OLED_SCL = BitValue ? 1 : 0; }
void OLED_W_SDA(uint8_t BitValue) { OLED_SDA = BitValue ? 1 : 0; }

void OLED_GPIO_Init(void)
{
    unsigned int i, j;
    for (i = 0; i < 1000; i++)
        for (j = 0; j < 1000; j++);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

/* ==================== I2C 通信 ==================== */
void OLED_I2C_Start(void)
{
    OLED_W_SDA(1); OLED_W_SCL(1); OLED_Delay(1);
    OLED_W_SDA(0); OLED_Delay(1);
    OLED_W_SCL(0);
}

void OLED_I2C_Stop(void)
{
    OLED_W_SDA(0); OLED_W_SCL(1); OLED_Delay(1);
    OLED_W_SDA(1); OLED_Delay(1);
}

void OLED_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        OLED_W_SDA(!!(Byte & (0x80 >> i)));
        OLED_Delay(1);
        OLED_W_SCL(1); OLED_Delay(1);
        OLED_W_SCL(0); OLED_Delay(1);
    }
    OLED_W_SCL(1); OLED_Delay(1);
    OLED_W_SCL(0); OLED_Delay(1);
}

void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(OLED_ADDR);
    OLED_I2C_SendByte(0x00);
    OLED_I2C_SendByte(Command);
    OLED_I2C_Stop();
}

void OLED_WriteData(uint8_t *Data, uint8_t Count)
{
    uint8_t i;
    OLED_I2C_Start();
    OLED_I2C_SendByte(OLED_ADDR);
    OLED_I2C_SendByte(0x40);
    for (i = 0; i < Count; i++)
        OLED_I2C_SendByte(Data[i]);
    OLED_I2C_Stop();
}

/* ==================== 硬件配置 ==================== */
void OLED_Init(void)
{
    OLED_GPIO_Init();
    OLED_WriteCommand(0xAE);
    OLED_WriteCommand(0xD5); OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8); OLED_WriteCommand(0x0F);
    OLED_WriteCommand(0xD3); OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40);
    OLED_WriteCommand(0xA1);
    OLED_WriteCommand(0xC8);
    OLED_WriteCommand(0xDA); OLED_WriteCommand(0x02);
    OLED_WriteCommand(0x81); OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9); OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB); OLED_WriteCommand(0x30);
    OLED_WriteCommand(0xA4);
    OLED_WriteCommand(0xA6);
    OLED_WriteCommand(0x8D); OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF);
    OLED_Clear();
    OLED_Update();
}

void OLED_SetCursor(uint8_t Page, uint8_t X)
{
    OLED_WriteCommand(0xB0 | (Page & 0x07));
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));
    OLED_WriteCommand(0x00 | (X & 0x0F));
}

/* ==================== 工具函数 ==================== */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--) Result *= X;
    return Result;
}

uint8_t OLED_pnpoly(uint8_t nvert, int16_t *vertx, int16_t *verty, int16_t testx, int16_t testy)
{
    int16_t i, j, c = 0;
    for (i = 0, j = nvert - 1; i < nvert; j = i++)
    {
        if (((verty[i] > testy) != (verty[j] > testy)) &&
            (testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
            c = !c;
    }
    return c;
}

uint8_t OLED_IsInAngle(int16_t X, int16_t Y, int16_t StartAngle, int16_t EndAngle)
{
    int16_t PointAngle = atan2(Y, X) / 3.14 * 180;
    if (StartAngle < EndAngle)
        return (PointAngle >= StartAngle && PointAngle <= EndAngle) ? 1 : 0;
    else
        return (PointAngle >= StartAngle || PointAngle <= EndAngle) ? 1 : 0;
}

/* ==================== 显存管理 ==================== */
void OLED_Update(void)
{
    uint8_t j;
    for (j = 0; j < OLED_PAGES; j++)
    {
        OLED_SetCursor(j, 0);
        OLED_WriteData(OLED_DisplayBuf[j], OLED_WIDTH);
    }
}

void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
    int16_t j, Page, Page1;
    Page = Y / 8;
    Page1 = (Y + Height - 1) / 8 + 1;
    if (Y < 0) { Page--; Page1--; }
    for (j = Page; j < Page1; j++)
        if (X >= 0 && X < OLED_WIDTH && j >= 0 && j < OLED_PAGES)
        {
            OLED_SetCursor(j, X);
            OLED_WriteData(&OLED_DisplayBuf[j][X], Width);
        }
}

void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < OLED_PAGES; j++)
        for (i = 0; i < OLED_WIDTH; i++)
            OLED_DisplayBuf[j][i] = 0x00;
}

void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
    int16_t i, j;
    for (j = Y; j < Y + Height; j++)
        for (i = X; i < X + Width; i++)
            if (i >= 0 && i < OLED_WIDTH && j >= 0 && j < OLED_HEIGHT)
                OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8));
}

void OLED_Reverse(void)
{
    uint8_t i, j;
    for (j = 0; j < OLED_PAGES; j++)
        for (i = 0; i < OLED_WIDTH; i++)
            OLED_DisplayBuf[j][i] ^= 0xFF;
}

void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
    int16_t i, j;
    for (j = Y; j < Y + Height; j++)
        for (i = X; i < X + Width; i++)
            if (i >= 0 && i < OLED_WIDTH && j >= 0 && j < OLED_HEIGHT)
                OLED_DisplayBuf[j / 8][i] ^= 0x01 << (j % 8);
}

/* ==================== 文字显示（仅 6x8） ==================== */
void OLED_ShowChar(int16_t X, int16_t Y, char Char)
{
    OLED_ShowImage(X, Y, 6, 8, OLED_F6x8[Char - ' ']);
}

void OLED_ShowString(int16_t X, int16_t Y, char *String)
{
    uint16_t XOffset = 0;
    while (*String != '\0')
    {
        OLED_ShowChar(X + XOffset, Y, *String);
        XOffset += 6;
        String++;
    }
}

void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
        OLED_ShowChar(X + i * 6, Y, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
}

void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0) { OLED_ShowChar(X, Y, '+'); Number1 = Number; }
    else { OLED_ShowChar(X, Y, '-'); Number1 = -Number; }
    for (i = 0; i < Length; i++)
        OLED_ShowChar(X + (i + 1) * 6, Y, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
}

void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10) OLED_ShowChar(X + i * 6, Y, SingleNumber + '0');
        else OLED_ShowChar(X + i * 6, Y, SingleNumber - 10 + 'A');
    }
}

void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
        OLED_ShowChar(X + i * 6, Y, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
}

void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength)
{
    uint32_t PowNum, IntNum, FraNum;
    if (Number >= 0) OLED_ShowChar(X, Y, '+');
    else { OLED_ShowChar(X, Y, '-'); Number = -Number; }
    IntNum = Number;
    Number -= IntNum;
    PowNum = OLED_Pow(10, FraLength);
    FraNum = round(Number * PowNum);
    IntNum += FraNum / PowNum;
    OLED_ShowNum(X + 6, Y, IntNum, IntLength);
    OLED_ShowChar(X + (IntLength + 1) * 6, Y, '.');
    OLED_ShowNum(X + (IntLength + 2) * 6, Y, FraNum, FraLength);
}

/* ==================== 图像显示 ==================== */
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
    uint8_t i = 0, j = 0;
    int16_t Page, Shift;
    OLED_ClearArea(X, Y, Width, Height);
    for (j = 0; j < (Height - 1) / 8 + 1; j++)
        for (i = 0; i < Width; i++)
            if (X + i >= 0 && X + i < OLED_WIDTH)
            {
                Page = Y / 8;
                Shift = Y % 8;
                if (Y < 0) { Page--; Shift += 8; }
                if (Page + j >= 0 && Page + j < OLED_PAGES)
                    OLED_DisplayBuf[Page + j][X + i] |= Image[j * Width + i] << Shift;
                if (Page + j + 1 >= 0 && Page + j + 1 < OLED_PAGES)
                    OLED_DisplayBuf[Page + j + 1][X + i] |= Image[j * Width + i] >> (8 - Shift);
            }
}

/* ==================== 绘图函数（完整实现） ==================== */
void OLED_DrawPoint(int16_t X, int16_t Y)
{
    if (X >= 0 && X < OLED_WIDTH && Y >= 0 && Y < OLED_HEIGHT)
        OLED_DisplayBuf[Y / 8][X] |= 0x01 << (Y % 8);
}

uint8_t OLED_GetPoint(int16_t X, int16_t Y)
{
    if (X >= 0 && X < OLED_WIDTH && Y >= 0 && Y < OLED_HEIGHT)
        if (OLED_DisplayBuf[Y / 8][X] & (0x01 << (Y % 8))) return 1;
    return 0;
}

void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1)
{
    int16_t x, y, dx, dy, d, incrE, incrNE, temp;
    int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
    uint8_t yflag = 0, xyflag = 0;

    if (y0 == y1) {
        if (x0 > x1) { temp = x0; x0 = x1; x1 = temp; }
        for (x = x0; x <= x1; x++) OLED_DrawPoint(x, y0);
    }
    else if (x0 == x1) {
        if (y0 > y1) { temp = y0; y0 = y1; y1 = temp; }
        for (y = y0; y <= y1; y++) OLED_DrawPoint(x0, y);
    }
    else {
        if (x0 > x1) { temp = x0; x0 = x1; x1 = temp; temp = y0; y0 = y1; y1 = temp; }
        if (y0 > y1) { y0 = -y0; y1 = -y1; yflag = 1; }
        if (y1 - y0 > x1 - x0) { temp = x0; x0 = y0; y0 = temp; temp = x1; x1 = y1; y1 = temp; xyflag = 1; }
        dx = x1 - x0; dy = y1 - y0;
        incrE = 2 * dy; incrNE = 2 * (dy - dx);
        d = 2 * dy - dx;
        x = x0; y = y0;
        if (yflag && xyflag) OLED_DrawPoint(y, -x);
        else if (yflag) OLED_DrawPoint(x, -y);
        else if (xyflag) OLED_DrawPoint(y, x);
        else OLED_DrawPoint(x, y);
        while (x < x1) {
            x++;
            if (d < 0) d += incrE;
            else { y++; d += incrNE; }
            if (yflag && xyflag) OLED_DrawPoint(y, -x);
            else if (yflag) OLED_DrawPoint(x, -y);
            else if (xyflag) OLED_DrawPoint(y, x);
            else OLED_DrawPoint(x, y);
        }
    }
}

void OLED_DrawDashedLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1)
{
    const uint8_t dashLength = 3, gapLength = 2;
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
    }
    else if (x0 == x1) {
        if (y0 > y1) { temp = y0; y0 = y1; y1 = temp; }
        for (y = y0; y <= y1; y++) {
            if (pointCount < dashLength) OLED_DrawPoint(x0, y);
            pointCount = (pointCount + 1) % (dashLength + gapLength);
        }
    }
    else {
        if (x0 > x1) { temp = x0; x0 = x1; x1 = temp; temp = y0; y0 = y1; y1 = temp; }
        if (y0 > y1) { y0 = -y0; y1 = -y1; yflag = 1; }
        if (y1 - y0 > x1 - x0) { temp = x0; x0 = y0; y0 = temp; temp = x1; x1 = y1; y1 = temp; xyflag = 1; }
        dx = x1 - x0; dy = y1 - y0;
        incrE = 2 * dy; incrNE = 2 * (dy - dx);
        d = 2 * dy - dx;
        x = x0; y = y0;
        if (pointCount < dashLength) {
            if (yflag && xyflag) OLED_DrawPoint(y, -x);
            else if (yflag) OLED_DrawPoint(x, -y);
            else if (xyflag) OLED_DrawPoint(y, x);
            else OLED_DrawPoint(x, y);
        }
        pointCount = (pointCount + 1) % (dashLength + gapLength);
        while (x < x1) {
            x++;
            if (d < 0) d += incrE;
            else { y++; d += incrNE; }
            if (pointCount < dashLength) {
                if (yflag && xyflag) OLED_DrawPoint(y, -x);
                else if (yflag) OLED_DrawPoint(x, -y);
                else if (xyflag) OLED_DrawPoint(y, x);
                else OLED_DrawPoint(x, y);
            }
            pointCount = (pointCount + 1) % (dashLength + gapLength);
        }
    }
}

void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled)
{
    int16_t i, j;
    if (!IsFilled) {
        for (i = X; i < X + Width; i++) { OLED_DrawPoint(i, Y); OLED_DrawPoint(i, Y + Height - 1); }
        for (i = Y; i < Y + Height; i++) { OLED_DrawPoint(X, i); OLED_DrawPoint(X + Width - 1, i); }
    }
    else {
        for (i = X; i < X + Width; i++)
            for (j = Y; j < Y + Height; j++)
                OLED_DrawPoint(i, j);
    }
}

void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled)
{
    int16_t minx = X0, miny = Y0, maxx = X0, maxy = Y0;
    int16_t i, j;
    int16_t vx[3], vy[3];
    vx[0] = X0; vx[1] = X1; vx[2] = X2;
    vy[0] = Y0; vy[1] = Y1; vy[2] = Y2;

    if (!IsFilled) {
        OLED_DrawLine(X0, Y0, X1, Y1);
        OLED_DrawLine(X0, Y0, X2, Y2);
        OLED_DrawLine(X1, Y1, X2, Y2);
    }
    else {
        if (X1 < minx) minx = X1; if (X2 < minx) minx = X2;
        if (Y1 < miny) miny = Y1; if (Y2 < miny) miny = Y2;
        if (X1 > maxx) maxx = X1; if (X2 > maxx) maxx = X2;
        if (Y1 > maxy) maxy = Y1; if (Y2 > maxy) maxy = Y2;
        for (i = minx; i <= maxx; i++)
            for (j = miny; j <= maxy; j++)
                if (OLED_pnpoly(3, vx, vy, i, j)) OLED_DrawPoint(i, j);
    }
}

void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled)
{
    int16_t x, y, d, j;
    d = 1 - Radius; x = 0; y = Radius;
    OLED_DrawPoint(X + x, Y + y); OLED_DrawPoint(X - x, Y - y);
    OLED_DrawPoint(X + y, Y + x); OLED_DrawPoint(X - y, Y - x);
    if (IsFilled) for (j = -y; j < y; j++) OLED_DrawPoint(X, Y + j);
    while (x < y) {
        x++;
        if (d < 0) d += 2 * x + 1;
        else { y--; d += 2 * (x - y) + 1; }
        OLED_DrawPoint(X + x, Y + y); OLED_DrawPoint(X + y, Y + x);
        OLED_DrawPoint(X - x, Y - y); OLED_DrawPoint(X - y, Y - x);
        OLED_DrawPoint(X + x, Y - y); OLED_DrawPoint(X + y, Y - x);
        OLED_DrawPoint(X - x, Y + y); OLED_DrawPoint(X - y, Y + x);
        if (IsFilled) {
            for (j = -y; j < y; j++) { OLED_DrawPoint(X + x, Y + j); OLED_DrawPoint(X - x, Y + j); }
            for (j = -x; j < x; j++) { OLED_DrawPoint(X - y, Y + j); OLED_DrawPoint(X + y, Y + j); }
        }
    }
}

void OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled)
{
    int16_t x, y, j;
    int16_t a = A, b = B;
    float d1, d2;
    x = 0; y = b;
    d1 = b * b + a * a * (-b + 0.5);
    if (IsFilled) for (j = -y; j < y; j++) { OLED_DrawPoint(X, Y + j); OLED_DrawPoint(X, Y + j); }
    OLED_DrawPoint(X + x, Y + y); OLED_DrawPoint(X - x, Y - y);
    OLED_DrawPoint(X - x, Y + y); OLED_DrawPoint(X + x, Y - y);
    while (b * b * (x + 1) < a * a * (y - 0.5)) {
        if (d1 <= 0) d1 += b * b * (2 * x + 3);
        else { d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2); y--; }
        x++;
        if (IsFilled) for (j = -y; j < y; j++) { OLED_DrawPoint(X + x, Y + j); OLED_DrawPoint(X - x, Y + j); }
        OLED_DrawPoint(X + x, Y + y); OLED_DrawPoint(X - x, Y - y);
        OLED_DrawPoint(X - x, Y + y); OLED_DrawPoint(X + x, Y - y);
    }
    d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
    while (y > 0) {
        if (d2 <= 0) { d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3); x++; }
        else d2 += a * a * (-2 * y + 3);
        y--;
        if (IsFilled) for (j = -y; j < y; j++) { OLED_DrawPoint(X + x, Y + j); OLED_DrawPoint(X - x, Y + j); }
        OLED_DrawPoint(X + x, Y + y); OLED_DrawPoint(X - x, Y - y);
        OLED_DrawPoint(X - x, Y + y); OLED_DrawPoint(X + x, Y - y);
    }
}

void OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled)
{
    int16_t x, y, d, j;
    d = 1 - Radius; x = 0; y = Radius;
    if (OLED_IsInAngle(x, y, StartAngle, EndAngle)) OLED_DrawPoint(X + x, Y + y);
    if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) OLED_DrawPoint(X - x, Y - y);
    if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) OLED_DrawPoint(X + y, Y + x);
    if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) OLED_DrawPoint(X - y, Y - x);
    if (IsFilled) for (j = -y; j < y; j++) if (OLED_IsInAngle(0, j, StartAngle, EndAngle)) OLED_DrawPoint(X, Y + j);
    while (x < y) {
        x++;
        if (d < 0) d += 2 * x + 1;
        else { y--; d += 2 * (x - y) + 1; }
        if (OLED_IsInAngle(x, y, StartAngle, EndAngle)) OLED_DrawPoint(X + x, Y + y);
        if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) OLED_DrawPoint(X + y, Y + x);
        if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) OLED_DrawPoint(X - x, Y - y);
        if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) OLED_DrawPoint(X - y, Y - x);
        if (OLED_IsInAngle(x, -y, StartAngle, EndAngle)) OLED_DrawPoint(X + x, Y - y);
        if (OLED_IsInAngle(y, -x, StartAngle, EndAngle)) OLED_DrawPoint(X + y, Y - x);
        if (OLED_IsInAngle(-x, y, StartAngle, EndAngle)) OLED_DrawPoint(X - x, Y + y);
        if (OLED_IsInAngle(-y, x, StartAngle, EndAngle)) OLED_DrawPoint(X - y, Y + x);
        if (IsFilled) {
            for (j = -y; j < y; j++) {
                if (OLED_IsInAngle(x, j, StartAngle, EndAngle)) OLED_DrawPoint(X + x, Y + j);
                if (OLED_IsInAngle(-x, j, StartAngle, EndAngle)) OLED_DrawPoint(X - x, Y + j);
            }
            for (j = -x; j < x; j++) {
                if (OLED_IsInAngle(-y, j, StartAngle, EndAngle)) OLED_DrawPoint(X - y, Y + j);
                if (OLED_IsInAngle(y, j, StartAngle, EndAngle)) OLED_DrawPoint(X + y, Y + j);
            }
        }
    }
}