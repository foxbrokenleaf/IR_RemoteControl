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
    timer2_config();
    IR_Send_Init();
    IR_Recvier_Init();
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

    AirGree();

    while(1){
        if(tim2Tick % 2) tim2Tick_ms++;
        if(tim2Tick_ms % 1000) tim2Tick_s++;

        GUI_Process();
        if(tim2Tick_s % 10) AirGree();
        // currentMode++;
        // currentSwing++;
        // currentMute++;
        // currentHealth++;
        // Temperature++;
        // FanSpeed++;
        // Sleep++;
        // Light++;
        // Super++;
        // Heat++;
        // timerRingTime.month = timerRingTime.day = timerRingTime.hour = ++timerRingTime.min;
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


// ��ʱ��2���ú�����΢�뼶��
void timer2_config(void)
{
    // ʹ�ܶ�ʱ��2ʱ��
    rcu_periph_clock_enable(RCU_TIMER2);
    
    // ��λ��ʱ��2
    timer_deinit(TIMER2);
    
    // ���ö�ʱ����������
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);
    
    // ��ʱ��ʱ�ӷ�Ƶ��72MHz / 72 = 1MHz (1us)
    // 72��Ƶ�󣬶�ʱ������Ƶ��Ϊ1MHz��ÿ��������Ӧ1΢��
    timer_initpara.prescaler         = 72 - 1;  // Ԥ��Ƶֵ = 72-1
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;  // ���ϼ���
    timer_initpara.period            = 560 - 1;  // �������ڣ�100us����һ���ж�
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;  // ʱ�ӷ�Ƶ����
    timer_initpara.repetitioncounter = 0;  // �ظ�������ֵ
    
    // ��ʼ����ʱ��
    timer_init(TIMER2, &timer_initpara);
    
    // ��������жϱ�־λ
    timer_interrupt_flag_clear(TIMER2, TIMER_INT_UP);
    
    // ʹ�ܶ�ʱ�������ж�
    timer_interrupt_enable(TIMER2, TIMER_INT_UP);
    
    // �����ж����ȼ�
    nvic_irq_enable(TIMER2_IRQn, 1, 0);
    
    // ������ʱ��
    timer_enable(TIMER2);
}

int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART1, (uint8_t)ch);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return ch;
}