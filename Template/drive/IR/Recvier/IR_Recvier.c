#include "IR_Recvier.h"

void IR_Recvier_Init(){
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
}

uint8_t IR_Recvier_Get_Bit(){
    gpio_input_bit_get(GPIOA, GPIO_PIN_12);
}

