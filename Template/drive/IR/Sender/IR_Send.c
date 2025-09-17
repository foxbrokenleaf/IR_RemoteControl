#include "IR_Send.h"

void IR_Send_Init(){
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_bit_write(GPIOA, GPIO_PIN_11, RESET);
}

void IR_Send_BUS(uint8_t value){
    gpio_bit_write(GPIOA, GPIO_PIN_11, (bit_status)value);
    
}

// ���͵���λ�����ģ����롰�ز�+���ز���ʱ��
// �߼�0��560us�ز���HS0038�ͣ� + 560us���ز���HS0038�ߣ�
// �߼�1��560us�ز���HS0038�ͣ� + 1680us���ز���HS0038�ߣ�
void IR_Send_Bit(uint8_t bit) {
    uint32_t startTick;
    
    // ��һ��������560us�ز���HS0038����ͣ���Ӧԭʼ���ݵġ��͵�ƽ�Ρ���
    IR_Send_BUS(0);  // ����38kHz�ز���0��ʾ���䣩
    startTick = tim2Tick;
    // 560us �� 560/550 �� 1.02 tick �� ��1��tick��550us������2%��
    while(tim2Tick - startTick < 1);  
    
    // �ڶ���������λֵ�������ز�ʱ����HS0038����ߣ���Ӧԭʼ���ݵġ��ߵ�ƽ�Ρ���
    IR_Send_BUS(1);  // ֹͣ���䣨1��ʾֹͣ��
    if (bit) {
        // �߼�1��1680us���ز� �� 1680/550��3.05 tick �� ��3��tick��1650us������1.8%��
        startTick = tim2Tick;
        while(tim2Tick - startTick < 3);
    } else {
        // �߼�0��560us���ز� �� 560/550��1.02 tick �� ��1��tick��550us������2%��
        startTick = tim2Tick;
        while(tim2Tick - startTick < 1);
    }
}

// ���������루9msL + 4.5msH��L=�ز���H=���ز���
void IR_Send_Leader_Code(void) {
    uint32_t startTick;
    
    // 9msL��9ms�ز���HS0038�ͣ��� 9000us / 550us ��16.36 tick �� ��16��tick��8800us������2.2%��
    IR_Send_BUS(0);  // �����ز�
    startTick = tim2Tick;
    while(tim2Tick - startTick < 16);
    
    // 4.5msH��4.5ms���ز���HS0038�ߣ��� 4500us / 550us��8.18 tick �� ��8��tick��4400us������2.2%��
    IR_Send_BUS(1);  // ֹͣ����
    startTick = tim2Tick;
    while(tim2Tick - startTick < 8);
}

// �����ظ������루����ԭʼ�����е�9msL + 4.45msH��
void IR_Send_Repeat_Leader_Code(void) {
    uint32_t startTick;
    
    // 9msL��ͬ�������루16��tick��
    IR_Send_BUS(0);
    startTick = tim2Tick;
    while(tim2Tick - startTick < 16);
    
    // 4.45msH��4450us / 550us��8.09 tick �� ��8��tick��4400us������1.1%��
    IR_Send_BUS(1);
    startTick = tim2Tick;
    while(tim2Tick - startTick < 8);
}

/**
 * @brief ���ⷢ�ͼ������Ӧԭʼ�����е�20msH��40msH����Ϊ���ز�״̬��
 * @param ms�����ʱ������λ��ms����20/40��
 */
void IR_Send_Interval(uint16_t ms) {
    uint32_t startTick;

    // ��һ��������560us�ز���HS0038����ͣ���Ӧԭʼ���ݵġ��͵�ƽ�Ρ���
    IR_Send_BUS(0);  // ����38kHz�ز���0��ʾ���䣩
    startTick = tim2Tick;
    // 560us �� 560/550 �� 1.02 tick �� ��1��tick��550us������2%��
    while(tim2Tick - startTick < 1);  
    
    // ����ڼ�Ϊ���ز���HS0038�ߵ�ƽ��������ֹͣ����״̬
    IR_Send_BUS(1);  // ֹͣ����
    
    // ����tick����ms * 1000us / 550us per tick �� ����ȡ���������
    uint16_t ticks = (ms * 1000 + 549) / 550;  // ��������
    startTick = tim2Tick;
    while(tim2Tick - startTick < ticks);
}

void AirGreePowerOff(){
        // ������֡��35λ��ʹ������ֱ�Ӵ洢ÿһλ��
    uint8_t main_frame[35] = {
        0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,1,0
    };

    // ��������֡��32λ������洢��
    uint8_t aux_frame[32] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0
    };
    // ���������루9ms�͵�ƽ + 4.5ms�ߵ�ƽ��
    IR_Send_Leader_Code();
    
    for (uint8_t i = 0; i < 35; i++) {
        IR_Send_Bit(main_frame[i]);  // ֱ�ӷ��������е�ÿһλ
    }

    // 3. ����֡��̼����20ms�ߵ�ƽ��
    IR_Send_Interval(20);


    for (uint8_t i = 0; i < 32; i++) {
        IR_Send_Bit(aux_frame[i]);
    }

    // 5. ����֡�䳤�����40ms�ߵ�ƽ��
    IR_Send_Interval(40);    
}
/**
 * ����У���룺(mode - 1) + (temp - 16) + 5 ȡ��4λ����
 * 
 * ����:
 *   mode: ģʽֵ
 *   temp: �¶�ֵ
 * 
 * ����:
 *   ����õ���У����
 */
uint8_t calculate_checksum(uint8_t mode, uint8_t temp, uint8_t horizontal_swing, uint8_t ventilate) {
    // �������ֵ
    uint8_t base_value = (mode - 1) + (temp - 16) + 5 + horizontal_swing + ventilate;
    
    // ȡ��4λ������16ȡģ��
    uint8_t low4 = base_value % 16;
    // �����������ȷ�����Ϊ����
    // if (low4 < 0) {
    //     low4 += 16;
    // }
    
    // ����4λ������
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < 4; i++) {
        // ȡ�����λ���Ƶ���Ӧλ��
        checksum |= ((low4 >> i) & 1) << (3 - i);
    }
    
    return checksum;
}
// ��תnλ���������ĸ�������
uint8_t reverse_bits(uint8_t value, uint8_t bits) {
    uint8_t result = 0;
    for (uint8_t i = 0; i < bits; i++) {
        result <<= 1;
        result |= (value & 1);
        value >>= 1;
    }
    return result;
}
void AirGree(uint8_t mode, uint8_t power, uint8_t speed, uint8_t swing, uint8_t sleep, uint8_t temp, uint8_t timer, uint8_t muscle, uint8_t light, uint8_t anion, uint8_t heat, uint8_t ventilate, uint8_t upDownSwing, uint8_t leftRightSwing, uint8_t tempMeasure){

    // ������֡��35λ��
    uint8_t main_frame[35] = {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0};

    // ��������֡��32λ��
    uint8_t aux_frame[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    // ����������֡�ֶ�
    // m: ����ɨ�磨1λ��- ��0λ
    aux_frame[0] = (upDownSwing > 0) ? 1 : 0;
    
    // �̶�λ��000 - ��1-3λ����0
    
    // n: ����ɨ�磨1λ��- ��4λ
    aux_frame[4] = (leftRightSwing > 0) ? 1 : 0;
    
    // �̶�λ��000 - ��5-7λ����0
    
    // o: ���£�2λ��- ��8-9λ
    aux_frame[8] = (tempMeasure & 0x01) ? 1 : 0;  // ��λ
    aux_frame[9] = (tempMeasure & 0x02) ? 1 : 0;  // ��λ

    // p: У���루4λ��- ��28-31λ
    uint8_t checksum = calculate_checksum(mode, temp, leftRightSwing, ventilate);

    // У����ֻȡ��4λ
    aux_frame[28] = (checksum & 0x08) ? 1 : 0;
    aux_frame[29] = (checksum & 0x04) ? 1 : 0;
    aux_frame[30] = (checksum & 0x02) ? 1 : 0;
    aux_frame[31] = (checksum & 0x01) ? 1 : 0;

    // a: ģʽ (3λ) - ����2λ�����÷������
    uint8_t mode_code = 0;
    switch (mode)
    {
    case MODE_COLD:    mode_code = 1; break;  // 001 -> 100
    case MODE_DRY:     mode_code = 2; break;  // 010 -> 010
    case MODE_FAN:     mode_code = 3; break;  // 011 -> 110
    case MODE_HOT:     mode_code = 4; break;  // 100 -> 001
    case MODE_AUTO:    // 000 -> 000
    default: mode_code = 0; break;
    }
    uint8_t reversed_mode = reverse_bits(mode_code, 3);
    main_frame[0] = (reversed_mode & 0x04) ? 1 : 0;
    main_frame[1] = (reversed_mode & 0x02) ? 1 : 0;
    main_frame[2] = (reversed_mode & 0x01) ? 1 : 0;

    // b: ���� (1λ)
    main_frame[3] = (power != 0) ? 1 : 0;

    // c: ���� (2λ) - ������2λ����������
    main_frame[4] = (speed & 0x01) ? 1 : 0;  // ��λ
    main_frame[5] = (speed & 0x02) ? 1 : 0;  // ��λ

    // d: ɨ�� (1λ)
    main_frame[6] = (swing > 0) ? 1 : 0;

    // e: ˯�� (1λ)
    main_frame[7] = (sleep > 0) ? 1 : 0;

    // f: �¶� (4λ) - ����2λ�����÷������
    if(temp >= 16 && temp <= 31)
    {
        uint8_t temp_code = temp - 16;  // 0-15
        uint8_t reversed_temp = reverse_bits(temp_code, 4);
        main_frame[8]  = (reversed_temp & 0x08) ? 1 : 0;
        main_frame[9]  = (reversed_temp & 0x04) ? 1 : 0;
        main_frame[10] = (reversed_temp & 0x02) ? 1 : 0;
        main_frame[11] = (reversed_temp & 0x01) ? 1 : 0;
    }

    // g: ��ʱ (8λ) - ����2λ�����÷������
    if(timer <= 255)
    {
        uint8_t reversed_timer = reverse_bits(timer, 7);
        main_frame[12] = (reversed_timer & 0x80) ? 1 : 0;
        main_frame[13] = (reversed_timer & 0x40) ? 1 : 0;
        main_frame[14] = (reversed_timer & 0x20) ? 1 : 0;
        main_frame[15] = (reversed_timer & 0x10) ? 1 : 0;
        main_frame[16] = (reversed_timer & 0x08) ? 1 : 0;
        main_frame[17] = (reversed_timer & 0x04) ? 1 : 0;
        main_frame[18] = (reversed_timer & 0x02) ? 1 : 0;
        main_frame[19] = (reversed_timer & 0x01) ? 1 : 0;
    }

    // h: ���� (1λ)
    main_frame[20] = (muscle > 0) ? 1 : 0;

    // i: �ƹ� (1λ)
    main_frame[21] = (light > 0) ? 1 : 0;

    // j: ������ (1λ)
    main_frame[22] = (anion > 0) ? 1 : 0;

    // k: ���� (1λ)
    main_frame[23] = (heat > 0) ? 1 : 0;

    // l: ���� (1λ)
    main_frame[24] = (ventilate > 0) ? 1 : 0;

    // ��������
    if(power != 0)
    {
        // ���������루9ms�͵�ƽ + 4.5ms�ߵ�ƽ��
        IR_Send_Leader_Code();
        
        // ����������֡
        for (uint8_t i = 0; i < 35; i++) {
            IR_Send_Bit(main_frame[i]);
        }

        // ����֡��̼����20ms�ߵ�ƽ��
        IR_Send_Interval(20);

        // ���͸�������֡
        for (uint8_t i = 0; i < 32; i++) {
            IR_Send_Bit(aux_frame[i]);
        }

        // ����֡�䳤�����40ms�ߵ�ƽ��
        IR_Send_Interval(40);
    }
    else
    {
        AirGreePowerOff();  // �رտյ�
    }
}

void IR_Send_Test(){
    
}