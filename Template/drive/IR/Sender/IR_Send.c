#include "IR_Send.h"

void IR_Send_Init(){
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_bit_write(GPIOA, GPIO_PIN_11, RESET);
}

void IR_Send_BUS(uint8_t value){
    gpio_bit_write(GPIOA, GPIO_PIN_11, (bit_status)value);
    
}

// 发送单个位
void IR_Send_Bit(uint8_t bit) {
    // 记录起始时刻的tick值
    uint32_t startTick;
    
    // 发送0：560us低电平（约1个tick）
    IR_Send_BUS(0);
    startTick = tim2Tick;
    while(tim2Tick - startTick < 1);  // 等待约550us（接近560us要求）
    
    // 根据位值发送不同时长的高电平
    IR_Send_BUS(1);
    if (bit) {
        // 发送1：1690us高电平（约3个tick：3×550=1650us）
        startTick = tim2Tick;
        while(tim2Tick - startTick < 3);
    } else {
        // 发送0：560us高电平（约1个tick）
        startTick = tim2Tick;
        while(tim2Tick - startTick < 1);
    }
}

void IR_Send_Leader_Code(void) {
    uint32_t startTick;
    
    // 发送9ms低电平（约16个tick：16×550=8800us）
    IR_Send_BUS(0);
    startTick = tim2Tick;
    while(tim2Tick - startTick < 16);
    
    // 发送4.5ms高电平（约8个tick：8×550=4400us）
    IR_Send_BUS(1);
    startTick = tim2Tick;
    while(tim2Tick - startTick < 8);
}

/**
 * @brief 红外发送间隔（空闲等待）
 * @param interval：间隔时长，单位为550us（即1个tim2Tick的时长）
 *        例如：interval=10 → 等待10×550us=5500us=5.5ms
 */
void IR_Send_Interval(uint8_t interval) {
    uint32_t startTick;

    // 发送0：560us低电平（约1个tick）
    IR_Send_BUS(0);
    startTick = tim2Tick;
    while(tim2Tick - startTick < 1);  // 等待约550us（接近560us要求）    
    // 间隔期间默认总线保持高电平（可根据协议改为低电平）
    IR_Send_BUS(1);
    
    // 等待指定数量的tick（每个tick=550us）
    startTick = tim2Tick;
    while (tim2Tick - startTick < interval) {
        // 空循环等待，利用tim2Tick计时
    }
}

/**
 * @brief 格力空调红外控制信号发送函数
 * @note 采用长度为35的数组直接存储每一位数据，直观映射二进制帧，避免位操作错误
 *       协议结构：引导码 + 主数据帧 + 间隔 + 辅助帧 + 间隔 + 重复帧
 */
void AirGree(){
    // 1. 发送引导码（9ms低电平 + 4.5ms高电平）
    IR_Send_Leader_Code();

    // 2. 发送主数据帧（35位，使用数组直接存储每一位）
    // 对应二进制: 10011000 01010000 00000100 00001010 010
    const uint8_t main_frame[35] = {
        1,0,0,1,1,0,0,0,
        0,1,0,1,0,0,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,1,0,1,0,
        0,1,0
    };
    for (uint8_t i = 0; i < 35; i++) {
        IR_Send_Bit(main_frame[i]);  // 直接发送数组中的每一位
    }

    // 3. 发送帧间短间隔（20ms高电平）
    IR_Send_Interval(20);

    // 4. 发送辅助数据帧（24位，数组存储）
    // 对应二进制: 00000000 00000100 00000000 00001111
    const uint8_t aux_frame[32] = {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,1,1,1,1  
    };
    for (uint8_t i = 0; i < 32; i++) {
        IR_Send_Bit(aux_frame[i]);
    }

    // 5. 发送帧间长间隔（40ms高电平）
    IR_Send_Interval(40);

    // 6. 发送重复引导码
    IR_Send_Leader_Code();

    // 7. 发送重复数据帧（35位，数组存储）
    // 对应二进制: 10011000 01010000 00000100 00001110 010
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

    // 8. 发送重复帧间短间隔
    IR_Send_Interval(20);

    // 9. 发送重复辅助数据帧（24位，数组存储）
    // 对应二进制: 00000000 00000000 00001000 00000111
    const uint8_t repeat_aux_frame[32] = {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,0,1,1,1  
    };
    for (uint8_t i = 0; i < 32; i++) {
        IR_Send_Bit(repeat_aux_frame[i]);
    }

    // 10. 发送最终结束间隔
    IR_Send_Interval(40);
}

void IR_Send_Test(){
    
}