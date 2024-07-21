/*
 * @Author: error: git config user.email & please set dead value or install git
 * @Date: 2024-07-21 15:45:27
 * @LastEditors: error: git config user.email & please set dead value or install git
 * @LastEditTime: 2024-07-21 19:57:46
 * @FilePath: \stm32Template\User\main.c
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "stm32f10x.h"

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

int main(){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitTypeDef gpio = {
        .GPIO_Mode = GPIO_Mode_Out_PP,
        .GPIO_Pin = GPIO_Pin_14,
        .GPIO_Speed = GPIO_Speed_50MHz
    };
    
    GPIO_Init(GPIOC,&gpio);

    GPIO_WriteBit(GPIOC,GPIO_Pin_14,0);

    delay_ms(10);

    Start();

    while(1){
                                                                                                                  
    }
}