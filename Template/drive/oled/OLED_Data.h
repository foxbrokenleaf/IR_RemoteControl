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

/*图像数据声明*/
extern const uint8_t Diode[];
/*按照上面的格式，在这�?位置加入新的图像数据声明*/
//...

#endif


/*****************江协科技|版权所�?****************/
/*****************jiangxiekeji.com*****************/
