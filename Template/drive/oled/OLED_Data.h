#ifndef __OLED_DATA_H
#define __OLED_DATA_H

#include <stdint.h>

/*字�?�集定义*/
/*以下两个宏定义只�?解除其中一�?的注�?*/
#define OLED_CHARSET_UTF8			//定义字�?�集为UTF8
//#define OLED_CHARSET_GB2312		//定义字�?�集为GB2312

/*字模基本单元*/
typedef struct 
{
	
#ifdef OLED_CHARSET_UTF8			//定义字�?�集为UTF8
	char Index[5];					//汉字索引，空间为5字节
#endif
	
#ifdef OLED_CHARSET_GB2312			//定义字�?�集为GB2312
	char Index[3];					//汉字索引，空间为3字节
#endif
	
	uint8_t Data[32];				//字模数据
} ChineseCell_t;

/*ASCII字模数据声明*/
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

/*汉字字模数据声明*/
extern const ChineseCell_t OLED_CF16x16[];

extern const uint8_t wifi_name_img[];
extern const uint8_t wifi_key_img[];

/*图像数据声明*/
extern const uint8_t uart_init_img[];
extern const uint8_t timer2_init_img[];
extern const uint8_t timer3_init_img[];
extern const uint8_t IRSender_init_img[];
extern const uint8_t IRRecvier_init_img[];
extern const uint8_t Keyboard_init_img[];

extern const uint8_t tuanzi_r[];
extern const uint8_t tuanzi_l[];
extern const uint8_t contral[];

extern const uint8_t single_img[];
extern const uint8_t BattaryImg[];
extern const uint8_t ChargeImg[];
extern const uint8_t Bowknot[];
extern const uint8_t BowknotFill[];
extern const uint8_t LeftArrow[];
extern const uint8_t RightArrow[];
extern const uint8_t LeftArrowFill[];
extern const uint8_t RightArrowFill[];
extern const uint8_t SettingIcon[];
extern const uint8_t ModeAutoImg[];
extern const uint8_t ModeSnowImg[];
extern const uint8_t ModeDryImg[];
extern const uint8_t ModeFanImg[];
extern const uint8_t ModeHeatImg[];
extern const uint8_t Thermometer[];
extern const uint8_t Power[];
extern const uint8_t PowerFill[];
extern const uint8_t Light[];
extern const uint8_t LightFill[];
extern const uint8_t FanSpeedAuto[];
extern const uint8_t FanSpeedLow[];
extern const uint8_t FanSpeedMedium[];
extern const uint8_t FanSpeedHigh[];
extern const uint8_t isSleepImg[];
extern const uint8_t noSleepImg[];
extern const uint8_t noSwingImg[];
extern const uint8_t VerticalSwingImg[];
extern const uint8_t HorizontalSwingImg[];
extern const uint8_t HorizontalVerticalSwingImg[];
extern const uint8_t noAuxHeat[];
extern const uint8_t AuxHeatImg[];
extern const uint8_t WiFiImg[];
extern const uint8_t UpdateImg[];
extern const uint8_t InfoImg[];
extern const uint8_t InfoIconImg[];
/*按照上面的格式，在这�?位置加入新的图像数据声明*/
extern const uint8_t Update_UI[];
//...

#endif


/*****************江协科技|版权所�?****************/
/*****************jiangxiekeji.com*****************/
