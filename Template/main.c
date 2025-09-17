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
    usart_config();
    timer2_config();
    IR_Send_Init();
    IR_Recvier_Init();
    OLED_Init();
    Keyboard_Init();    
    OLED_Update();
    // delay_1ms(3000);
    OLED_Clear();

    ad.MainData.mode = MODE_COLD;
    ad.MainData.fan_speed = FAN_HIGH;
    ad.MainData.light = 1;
    ad.MainData.power = 1;
    ad.MainData.temperature = 26;

    while(1){

        if(tim2Tick % 20 == 0) BowknotFlashFlag = !BowknotFlashFlag;

        // 根据不同按键码执行对应操作
        KeyboardCode keyCode = GetKeyboardCode();
        if (keyCode == KEY_CODE_O) {
            AirGree(ad.MainData.mode, 
                ad.MainData.power, 
                ad.MainData.fan_speed, 
                ad.MainData.swing, 
                ad.MainData.sleep_mode, 
                ad.MainData.temperature, 
                ad.MainData.timer_hours, 
                ad.MainData.muscle_function, 
                ad.MainData.light, 
                ad.MainData.negative_ion, 
                ad.MainData.auxiliary_heat, 
                ad.MainData.ventilation, 
                ad.AuxData.vertical_swing, 
                ad.AuxData.horizontal_swing, 
                ad.AuxData.temperature_read);            
        }
        else if (keyCode == KEY_CODE_U) {
            OLED_Clear();
        } 
        else if (keyCode == KEY_CODE_D) {
            OLED_Clear();
        }
        else if (keyCode == KEY_CODE_R) {
           index += 1;
           index = index >= 6 ? 1 : index;
           OLED_Clear();
        }
        else if (keyCode == KEY_CODE_L) {
           index -= 1;
           index = index <= 0 ? 5 : index;
           OLED_Clear();
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