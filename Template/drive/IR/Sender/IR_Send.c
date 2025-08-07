#include "IR_Send.h"

void IR_Send_Init(){
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_bit_write(GPIOA, GPIO_PIN_11, RESET);
}

void IR_Send_BUS(uint8_t value){
    gpio_bit_write(GPIOA, GPIO_PIN_11, (bit_status)value);
    
}

// ���͵���λ
void IR_Send_Bit(uint8_t bit) {
    // ��¼��ʼʱ�̵�tickֵ
    uint32_t startTick;
    
    // ����0��560us�͵�ƽ��Լ1��tick��
    IR_Send_BUS(0);
    startTick = tim2Tick;
    while(tim2Tick - startTick < 1);  // �ȴ�Լ550us���ӽ�560usҪ��
    
    // ����λֵ���Ͳ�ͬʱ���ĸߵ�ƽ
    IR_Send_BUS(1);
    if (bit) {
        // ����1��1690us�ߵ�ƽ��Լ3��tick��3��550=1650us��
        startTick = tim2Tick;
        while(tim2Tick - startTick < 3);
    } else {
        // ����0��560us�ߵ�ƽ��Լ1��tick��
        startTick = tim2Tick;
        while(tim2Tick - startTick < 1);
    }
}

void IR_Send_Leader_Code(void) {
    uint32_t startTick;
    
    // ����9ms�͵�ƽ��Լ16��tick��16��550=8800us��
    IR_Send_BUS(0);
    startTick = tim2Tick;
    while(tim2Tick - startTick < 16);
    
    // ����4.5ms�ߵ�ƽ��Լ8��tick��8��550=4400us��
    IR_Send_BUS(1);
    startTick = tim2Tick;
    while(tim2Tick - startTick < 8);
}

/**
 * @brief ���ⷢ�ͼ�������еȴ���
 * @param interval�����ʱ������λΪ550us����1��tim2Tick��ʱ����
 *        ���磺interval=10 �� �ȴ�10��550us=5500us=5.5ms
 */
void IR_Send_Interval(uint8_t interval) {
    uint32_t startTick;

    // ����0��560us�͵�ƽ��Լ1��tick��
    IR_Send_BUS(0);
    startTick = tim2Tick;
    while(tim2Tick - startTick < 1);  // �ȴ�Լ550us���ӽ�560usҪ��    
    // ����ڼ�Ĭ�����߱��ָߵ�ƽ���ɸ���Э���Ϊ�͵�ƽ��
    IR_Send_BUS(1);
    
    // �ȴ�ָ��������tick��ÿ��tick=550us��
    startTick = tim2Tick;
    while (tim2Tick - startTick < interval) {
        // ��ѭ���ȴ�������tim2Tick��ʱ
    }
}

/**
 * @brief �����յ���������źŷ��ͺ���
 * @note ���ó���Ϊ35������ֱ�Ӵ洢ÿһλ���ݣ�ֱ��ӳ�������֡������λ��������
 *       Э��ṹ�������� + ������֡ + ��� + ����֡ + ��� + �ظ�֡
 */
void AirGree(){
    // 1. ���������루9ms�͵�ƽ + 4.5ms�ߵ�ƽ��
    IR_Send_Leader_Code();

    // 2. ����������֡��35λ��ʹ������ֱ�Ӵ洢ÿһλ��
    // ��Ӧ������: 10011000 01010000 00000100 00001010 010
    const uint8_t main_frame[35] = {
        1,0,0,1,1,0,0,0,
        0,1,0,1,0,0,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,1,0,1,0,
        0,1,0
    };
    for (uint8_t i = 0; i < 35; i++) {
        IR_Send_Bit(main_frame[i]);  // ֱ�ӷ��������е�ÿһλ
    }

    // 3. ����֡��̼����20ms�ߵ�ƽ��
    IR_Send_Interval(20);

    // 4. ���͸�������֡��24λ������洢��
    // ��Ӧ������: 00000000 00000100 00000000 00001111
    const uint8_t aux_frame[32] = {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,1,1,1,1  
    };
    for (uint8_t i = 0; i < 32; i++) {
        IR_Send_Bit(aux_frame[i]);
    }

    // 5. ����֡�䳤�����40ms�ߵ�ƽ��
    IR_Send_Interval(40);

    // 6. �����ظ�������
    IR_Send_Leader_Code();

    // 7. �����ظ�����֡��35λ������洢��
    // ��Ӧ������: 10011000 01010000 00000100 00001110 010
    const uint8_t repeat_frame[35] = {
        1,0,0,1,1,0,0,0,
        0,1,0,1,0,0,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,1,1,1,0,
        0,1,0
    };
    for (uint8_t i = 0; i < 35; i++) {
        IR_Send_Bit(repeat_frame[i]);
    }

    // 8. �����ظ�֡��̼��
    IR_Send_Interval(20);

    // 9. �����ظ���������֡��24λ������洢��
    // ��Ӧ������: 00000000 00000000 00001000 00000111
    const uint8_t repeat_aux_frame[32] = {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,0,1,1,1  
    };
    for (uint8_t i = 0; i < 32; i++) {
        IR_Send_Bit(repeat_aux_frame[i]);
    }

    // 10. �������ս������
    IR_Send_Interval(40);
}

void IR_Send_Test(){
    
}