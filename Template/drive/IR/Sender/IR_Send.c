#include "IR_Send.h"

void IR_Send_Init(){
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_bit_write(GPIOA, GPIO_PIN_11, RESET);
}

void IR_Send_BUS(uint8_t value){
    gpio_bit_write(GPIOA, GPIO_PIN_11, (bit_status)value);
    
}

void IR_Send_Test(){
    IR_Send_BUS(1);
    delay_1ms(500);
    IR_Send_BUS(0);
    delay_1ms(500);
}