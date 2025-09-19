#include "main.h"

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{

    AirData ad;

    systick_config();
    OLED_Init();
    usart_config();
    OLED_ShowImage(24, 9, 16, 16, uart_init_img);
    OLED_Update();
    timer2_config();
    OLED_ShowImage(56, 9, 16, 16, timer2_init_img);
    OLED_Update();
    timer3_config();
    OLED_ShowImage(88, 9, 16, 16, timer3_init_img);
    OLED_Update();    
    IR_Send_Init();
    OLED_ShowImage(24, 32, 16, 16, IRSender_init_img);
    OLED_Update();
    IR_Recvier_Init();
    OLED_ShowImage(56, 32, 16, 16, IRRecvier_init_img);
    OLED_Update();
    Keyboard_Init();    
    OLED_ShowImage(88, 32, 16, 16, Keyboard_init_img);
    OLED_Update();
    delay_1ms(500);
    OLED_Clear();

    /*欢迎界面*/
    OLED_ShowImage(8, 16, 32, 32, tuanzi_r);
    OLED_ShowImage(48, 16, 32, 32, contral);
    OLED_ShowImage(88, 16, 32, 32, tuanzi_l);
    OLED_Update();
    delay_1ms(500);
    OLED_Clear();
    

    ad.MainData.mode = MODE_COLD;
    ad.MainData.fan_speed = FAN_HIGH;
    ad.MainData.light = 1;
    ad.MainData.power = 1;
    ad.MainData.temperature = 26;

    while(1){

        if(timer3counter > 1000){
            timer3counter = 0;
            BowknotFlashFlag = !BowknotFlashFlag;
        }

        // 根据不同按键码执行对应操作
        KeyboardCode keyCode = GetKeyboardCode();
        if (keyCode == KEY_CODE_O && index >= 1 && index <= 5) {
            AirGree(ad.MainData.mode, //√
                ad.MainData.power, //√
                ad.MainData.fan_speed, //√
                ad.MainData.swing, //√
                ad.MainData.sleep_mode, //√
                ad.MainData.temperature, //√
                ad.MainData.timer_hours, 
                ad.MainData.muscle_function, 
                ad.MainData.light, //√
                ad.MainData.negative_ion, 
                ad.MainData.auxiliary_heat, //√
                ad.MainData.ventilation, 
                ad.AuxData.vertical_swing, //√
                ad.AuxData.horizontal_swing, //√
                ad.AuxData.temperature_read);            
        }
        UpdateUIIconData(&ad, keyCode);
        UI_Process(keyCode);
    }
}



/*! 
    \brief      USART1 configure 
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usart_config(void)
{
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    
    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    
    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1, 115200U);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_enable(USART1);
}


void timer2_config(void)
{
    rcu_periph_clock_enable(RCU_TIMER2);
    
    timer_deinit(TIMER2);
    
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);
    
    timer_initpara.prescaler         = 72 - 1;  
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;  
    timer_initpara.period            = 560 - 1;  
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    
    timer_init(TIMER2, &timer_initpara);
    
    timer_interrupt_flag_clear(TIMER2, TIMER_INT_UP);
    
    timer_interrupt_enable(TIMER2, TIMER_INT_UP);
    
    nvic_irq_enable(TIMER2_IRQn, 1, 0);
    
    timer_enable(TIMER2);
}

void timer3_config(void)
{
    rcu_periph_clock_enable(RCU_TIMER3);
    
    timer_deinit(TIMER3);
    
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);
    
    timer_initpara.prescaler         = 72 - 1;  
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;  
    timer_initpara.period            = 1000 - 1;  
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    
    timer_init(TIMER3, &timer_initpara);
    
    timer_interrupt_flag_clear(TIMER3, TIMER_INT_UP);
    
    timer_interrupt_enable(TIMER3, TIMER_INT_UP);
    
    nvic_irq_enable(TIMER3_IRQn, 1, 0);
    
    timer_enable(TIMER3);
}

int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART1, (uint8_t)ch);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return ch;
}