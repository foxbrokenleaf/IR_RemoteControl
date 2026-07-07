#ifndef __OLED_0_69INCH_H__
#define __OLED_0_69INCH_H__

/* ==================== 分辨率定义 ==================== */
#define OLED_WIDTH         96
#define OLED_HEIGHT        16
#define OLED_PAGES         2

/* ==================== 类型定义 ==================== */
typedef unsigned char   uint8_t;
typedef unsigned int    uint16_t;
typedef unsigned long   uint32_t;
typedef signed   int    int16_t;
typedef signed   long   int32_t;

/* ==================== 字体选项（仅保留 6x8） ==================== */
#define OLED_6X8        2          // 宽6像素，高8像素
#define OLED_UNFILLED   0
#define OLED_FILLED     1

/* ==================== 外部字模库声明（仅 6x8 ASCII） ==================== */

/* ==================== 底层引脚操作 ==================== */
void OLED_W_SCL(uint8_t BitValue);
void OLED_W_SDA(uint8_t BitValue);
void OLED_GPIO_Init(void);

/* ==================== I2C 通信协议 ==================== */
void OLED_I2C_Start(void);
void OLED_I2C_Stop(void);
void OLED_I2C_SendByte(uint8_t Byte);
void OLED_WriteCommand(uint8_t Command);
void OLED_WriteData(uint8_t *Data, uint8_t Count);

/* ==================== 硬件配置 ==================== */
void OLED_Init(void);
void OLED_SetCursor(uint8_t Page, uint8_t X);

/* ==================== 工具函数 ==================== */
uint32_t OLED_Pow(uint32_t X, uint32_t Y);
uint8_t OLED_pnpoly(uint8_t nvert, int16_t *vertx, int16_t *verty, int16_t testx, int16_t testy);
uint8_t OLED_IsInAngle(int16_t X, int16_t Y, int16_t StartAngle, int16_t EndAngle);

/* ==================== 显存管理 ==================== */
extern uint8_t xdata OLED_DisplayBuf[OLED_PAGES][OLED_WIDTH];
void OLED_Update(void);
void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/* ==================== 清屏与取反 ==================== */
void OLED_Clear(void);
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);
void OLED_Reverse(void);
void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/* ==================== 文字显示（仅 6x8 ASCII） ==================== */
void OLED_ShowChar(int16_t X, int16_t Y, char Char);
void OLED_ShowString(int16_t X, int16_t Y, char *String);
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length);
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength);

/* ==================== 图像与绘图 ==================== */
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
void OLED_DrawPoint(int16_t X, int16_t Y);
uint8_t OLED_GetPoint(int16_t X, int16_t Y);
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void OLED_DrawDashedLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);
void OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);
void OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);

#endif