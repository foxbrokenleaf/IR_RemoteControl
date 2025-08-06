#include "main.h"


/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    systick_config();
    usart_config();
    IR_Send_Init();
    OLED_Init();
    Keyboard_Init();
    printf("Init done!\r\n");
    OLED_ShowString(0, 0, "SYS_CLK:", OLED_6X8);
    OLED_ShowNum(48, 0, rcu_clock_freq_get(CK_SYS), 10, OLED_6X8);
    OLED_ShowString(0, 8, "Init done!", OLED_6X8);
    OLED_Update();
    // delay_1ms(3000);
    OLED_Clear();

    //================================
    FanSpeed = 0;
    Temperature = 0;

    while(1){
        GUI_Process();
        currentMode++;
        currentSwing++;
        currentMute++;
        currentHealth++;
        Temperature++;
        FanSpeed++;
        Sleep++;
        Light++;
        Super++;
        Heat++;
        timerRingTime.month = timerRingTime.day = timerRingTime.hour = ++timerRingTime.min;
        // delay_1ms(1000);
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

int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART1, (uint8_t)ch);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return ch;
}