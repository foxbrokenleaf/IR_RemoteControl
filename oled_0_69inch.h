#ifndef __OLED_H
#define __OLED_H

/* Exact-width integer types for C51 */
typedef unsigned char      uint8_t;
typedef signed   int       int16_t;
typedef unsigned int       uint16_t;
typedef unsigned long      uint32_t;

/* Font size options */
#define OLED_8X16    0
#define OLED_6X8     1

/* Display buffer (external, placed in XDATA) */
extern uint8_t xdata OLED_DisplayBuf[2][96];

/* Public functions */
void OLED_Init(void);
void OLED_Update(void);
void OLED_Clear(void);

void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);
void OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize);
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);

void OLED_DrawPoint(int16_t X, int16_t Y);
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void OLED_DrawDashedLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);

#endif