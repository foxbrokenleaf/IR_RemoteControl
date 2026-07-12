#include "ws51f6240.h"
#include "OLED_Data.h"
#include "oled_0_69inch.h"

sbit OLED_SCL_P = P1^3;
sbit OLED_SDA_P = P1^4;

sbit P11 = P1 ^ 1;


/*
 * Define private funcation
*/


/*
 *	Private var
 */


static void delay(int i)
{
	long j;
	for(j=0;j<10*i;++j);
}

void main(){
		
    int i;
	
    SCCON  = 0x00;//HRC
    HRCON |= 0x80;
	
    P11F = 0x02;

    P11 = 1;
    P12 = 1;
    P12 = 0;
    P12 = 1;

	OLED_Init();
    OLED_ShowString(0, 0, "0123456789", OLED_8X16);
    OLED_Update();

    while(1){

    }
}

