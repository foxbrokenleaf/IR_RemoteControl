/*
 * @Author: error: git config user.email & please set dead value or install git
 * @Date: 2024-07-21 15:45:27
 * @LastEditors: 2353919304@qq.com
 * @LastEditTime: 2024-07-22 06:21:03
 * @FilePath: \stm32Template\User\main.c
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "stm32f10x.h"

#define Gree_Address 0x09
#define Temperature_Command  0xA0
#define IR_Data_Default_High 0x22
#define IR_Data_Default_Low 0x00


unsigned char Temperature = 22;
uint32_t counter = 0;

void delay_us(uint32_t delay_us)
{
  volatile unsigned int num;
  volatile unsigned int t;


  for (num = 0; num < delay_us; num++)
  {
    t = 11;
    while (t != 0)
    {
      t--;
    }
  }
}
//毫秒级的延时
void delay_ms(uint16_t delay_ms)
{
  volatile unsigned int num;
  for (num = 0; num < delay_ms; num++)
  {
    delay_us(1000);
  }
}
//26us
void IR_PWM(){
    GPIO_WriteBit(GPIOC,GPIO_Pin_14,1);
    GPIO_WriteBit(GPIOC,GPIO_Pin_14,1);
    delay_us(2);
    GPIO_WriteBit(GPIOC,GPIO_Pin_14,0);
    delay_us(9);
}

void IR_start(){
    counter = 346;
    while(counter--){
        IR_PWM();
    }
    GPIO_WriteBit(GPIOC,GPIO_Pin_14,0);
    delay_ms(2);
}

void IR_bit_1(){
    counter = 21;
    while(counter--){
        IR_PWM();
    }
    GPIO_WriteBit(GPIOC,GPIO_Pin_14,0);
    delay_us(665);
}

void IR_bit_0(){
    counter = 21;
    while(counter--){
        IR_PWM();
    }
    GPIO_WriteBit(GPIOC,GPIO_Pin_14,0);
    delay_us(200);
}

void IR_connect_1(){
    GPIO_WriteBit(GPIOC,GPIO_Pin_14,0);
    delay_ms(10);
}
void IR_connect_2(){
    GPIO_WriteBit(GPIOC,GPIO_Pin_14,0);
    delay_ms(20);
}

void IR_end(){
    counter = 21;
    while(counter--){
        IR_PWM();
    }
}

void Gree_00001010010(){
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
}

void Gree_00001110010(){
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_1();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
}

void NEC_Write_Byte(unsigned char dat){
    unsigned char i = 0;
    for(i = 0;i < 8;i++){
        if(dat & (0x01 << i)) IR_bit_1();
        else IR_bit_0();
    }
}

void Start(){
    IR_start();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();    
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();    
    IR_bit_1();     
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();   
    IR_bit_1();    
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();  
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();  
    IR_bit_1();    
    IR_bit_0();  
    IR_bit_1();     
    IR_bit_0();
    IR_bit_0();  
    IR_bit_1();    
    IR_bit_0();    
    IR_end();                                                                                      
    IR_connect_1();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0();     
    IR_bit_1();    
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0();     
    IR_bit_0();
    IR_bit_0();     
    IR_bit_1();  
    IR_bit_1();          
    IR_bit_0();      
    IR_bit_1();     
    IR_end();                                                                                 
    IR_connect_2();
    IR_start();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();     
    IR_bit_1();    
    IR_bit_1();  
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0();    
    IR_bit_1();  
    IR_bit_0();
    IR_bit_0();   
    IR_bit_0();
    IR_bit_0();   
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_1();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();                                                                                        
    IR_end();
    IR_connect_1();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_end();         
}

void Stop(){
    IR_start();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_end();
    IR_connect_1();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_end();
    IR_connect_2();
    IR_start();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_1();
    IR_bit_1();    
    IR_bit_0();
    IR_bit_0(); 
    IR_bit_1();        
    IR_bit_0(); 
    IR_end();
    IR_connect_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_bit_1();
    IR_bit_0();
    IR_bit_0();
    IR_bit_0();
    IR_end();
}

void IR_Gree_Add(){
    Temperature++;
    IR_start();
    NEC_Write_Byte(Gree_Address);
    NEC_Write_Byte(Temperature - 16);
    NEC_Write_Byte(Temperature_Command);
    Gree_00001010010();
    IR_end();
    IR_connect_1();
    NEC_Write_Byte(IR_Data_Default_Low);
    NEC_Write_Byte(IR_Data_Default_High);
    NEC_Write_Byte(0x00);
    NEC_Write_Byte((Temperature - 16 + 5) << 4);
    IR_end();
    IR_connect_2();
    IR_start();
    NEC_Write_Byte(Gree_Address);
    NEC_Write_Byte(Temperature - 16);
    NEC_Write_Byte(Temperature_Command);
    Gree_00001110010();
    IR_end();
    IR_connect_1();
    NEC_Write_Byte(0x00);
    NEC_Write_Byte(0x00);
    NEC_Write_Byte(0x00);
    NEC_Write_Byte((Temperature - 16 + 3) << 4);
    IR_end();
}

void IR_Gree_Lower(){
    Temperature--;
    IR_start();
    NEC_Write_Byte(Gree_Address);
    NEC_Write_Byte(Temperature - 16);
    NEC_Write_Byte(Temperature_Command);
    Gree_00001010010();
    IR_end();
    IR_connect_1();
    NEC_Write_Byte(IR_Data_Default_Low);
    NEC_Write_Byte(IR_Data_Default_High);
    NEC_Write_Byte(0x00);
    NEC_Write_Byte((Temperature - 16 + 5) << 4);
    IR_end();
    IR_connect_2();
    IR_start();
    NEC_Write_Byte(Gree_Address);
    NEC_Write_Byte(Temperature - 16);
    NEC_Write_Byte(Temperature_Command);
    Gree_00001110010();
    IR_end();
    IR_connect_1();
    NEC_Write_Byte(0x00);
    NEC_Write_Byte(0x00);
    NEC_Write_Byte(0x00);
    NEC_Write_Byte((Temperature - 16 + 3) << 4);
    IR_end();
}

int main(){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitTypeDef gpio = {
        .GPIO_Mode = GPIO_Mode_Out_PP,
        .GPIO_Pin = GPIO_Pin_14,
        .GPIO_Speed = GPIO_Speed_50MHz
    };
    GPIO_Init(GPIOC,&gpio);
    gpio.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC,&gpio);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    gpio.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA,&gpio);
    gpio.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA,&gpio);
    gpio.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA,&gpio);
    gpio.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA,&gpio);
    gpio.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA,&gpio);            

    GPIO_WriteBit(GPIOC,GPIO_Pin_14,0);
    GPIO_WriteBit(GPIOA,GPIO_Pin_0,0);
    GPIO_WriteBit(GPIOA,GPIO_Pin_1,0);
    GPIO_WriteBit(GPIOA,GPIO_Pin_2,0);
    GPIO_WriteBit(GPIOA,GPIO_Pin_3,0);
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,0);

    delay_ms(10);   //等待系统稳定

    while(1){
        //限制温度范围确保数据正常
        if(Temperature > 31) Temperature = 16;
        else if(Temperature < 16) Temperature = 31;

        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1){
            GPIO_WriteBit(GPIOC,GPIO_Pin_13,0);
            Start();
        }
        else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1){
            GPIO_WriteBit(GPIOC,GPIO_Pin_13,0);
            Stop();
        }
        else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) == 1){
            GPIO_WriteBit(GPIOC,GPIO_Pin_13,0);
            IR_Gree_Add();
        }
        else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3) == 1){
            GPIO_WriteBit(GPIOC,GPIO_Pin_13,0);
            IR_Gree_Lower();
        }
        else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 1){
            GPIO_WriteBit(GPIOC,GPIO_Pin_13,0);
            
        }                        
        else GPIO_WriteBit(GPIOC,GPIO_Pin_13,1); 
    }
}