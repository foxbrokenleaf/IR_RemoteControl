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
void SystemInit(void);  //初始化所需要的外设

/*
 *	Private var
 */


static void delay(int i)
{
	long j;
	for(j=0;j<10*i;++j);
}

void main(){
    uint8_t KeyCode = 0;
	
    SystemInit();
	OLED_Init();


    while(1){
        
        KeyCode = (uint8_t)(Key_1) | ((uint8_t)(Key_2) << 1) | ((uint8_t)(Key_3) << 2) | ((uint8_t)(Key_4) << 3) | ((uint8_t)(Key_5) << 4);

        OLED_ShowNum(0, 0, KeyCode, 3, OLED_6X8);

        OLED_Update();
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