#include "IR_Send.h"

void IR_Send_Init(){
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_bit_write(GPIOA, GPIO_PIN_11, RESET);
}

void IR_Send_BUS(uint8_t value){
    gpio_bit_write(GPIOA, GPIO_PIN_11, (bit_status)value);
    
}

// 发送单个位（核心：对齐“载波+无载波”时序）
// 逻辑0：560us载波（HS0038低） + 560us无载波（HS0038高）
// 逻辑1：560us载波（HS0038低） + 1680us无载波（HS0038高）
void IR_Send_Bit(uint8_t bit) {
    uint32_t startTick;
    
    // 第一步：发送560us载波（HS0038输出低，对应原始数据的“低电平段”）
    IR_Send_BUS(0);  // 发射38kHz载波（0表示发射）
    startTick = tim2Tick;
    // 560us ≈ 560/550 ≈ 1.02 tick → 用1个tick（550us，误差≈2%）
    while(tim2Tick - startTick < 1);  
    
    // 第二步：根据位值发送无载波时长（HS0038输出高，对应原始数据的“高电平段”）
    IR_Send_BUS(1);  // 停止发射（1表示停止）
    if (bit) {
        // 逻辑1：1680us无载波 → 1680/550≈3.05 tick → 用3个tick（1650us，误差≈1.8%）
        startTick = tim2Tick;
        while(tim2Tick - startTick < 3);
    } else {
        // 逻辑0：560us无载波 → 560/550≈1.02 tick → 用1个tick（550us，误差≈2%）
        startTick = tim2Tick;
        while(tim2Tick - startTick < 1);
    }
}

// 发送引导码（9msL + 4.5msH，L=载波，H=无载波）
void IR_Send_Leader_Code(void) {
    uint32_t startTick;
    
    // 9msL：9ms载波（HS0038低）→ 9000us / 550us ≈16.36 tick → 用16个tick（8800us，误差≈2.2%）
    IR_Send_BUS(0);  // 发射载波
    startTick = tim2Tick;
    while(tim2Tick - startTick < 16);
    
    // 4.5msH：4.5ms无载波（HS0038高）→ 4500us / 550us≈8.18 tick → 用8个tick（4400us，误差≈2.2%）
    IR_Send_BUS(1);  // 停止发射
    startTick = tim2Tick;
    while(tim2Tick - startTick < 8);
}

// 发送重复引导码（适配原始数据中的9msL + 4.45msH）
void IR_Send_Repeat_Leader_Code(void) {
    uint32_t startTick;
    
    // 9msL：同主引导码（16个tick）
    IR_Send_BUS(0);
    startTick = tim2Tick;
    while(tim2Tick - startTick < 16);
    
    // 4.45msH：4450us / 550us≈8.09 tick → 用8个tick（4400us，误差≈1.1%）
    IR_Send_BUS(1);
    startTick = tim2Tick;
    while(tim2Tick - startTick < 8);
}

/**
 * @brief 红外发送间隔（对应原始数据中的20msH、40msH，均为无载波状态）
 * @param ms：间隔时长（单位：ms，如20/40）
 */
void IR_Send_Interval(uint16_t ms) {
    uint32_t startTick;

    // 第一步：发送560us载波（HS0038输出低，对应原始数据的“低电平段”）
    IR_Send_BUS(0);  // 发射38kHz载波（0表示发射）
    startTick = tim2Tick;
    // 560us ≈ 560/550 ≈ 1.02 tick → 用1个tick（550us，误差≈2%）
    while(tim2Tick - startTick < 1);  
    
    // 间隔期间为无载波（HS0038高电平），保持停止发射状态
    IR_Send_BUS(1);  // 停止发射
    
    // 计算tick数：ms * 1000us / 550us per tick → 向上取整减少误差
    uint16_t ticks = (ms * 1000 + 549) / 550;  // 四舍五入
    startTick = tim2Tick;
    while(tim2Tick - startTick < ticks);
}

void AirGreePowerOff(){
        // 主数据帧（35位，使用数组直接存储每一位）
    uint8_t main_frame[35] = {
        0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,1,0
    };

    // 辅助数据帧（32位，数组存储）
    uint8_t aux_frame[32] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0
    };
    // 发送引导码（9ms低电平 + 4.5ms高电平）
    IR_Send_Leader_Code();
    
    for (uint8_t i = 0; i < 35; i++) {
        IR_Send_Bit(main_frame[i]);  // 直接发送数组中的每一位
    }

    // 3. 发送帧间短间隔（20ms高电平）
    IR_Send_Interval(20);


    for (uint8_t i = 0; i < 32; i++) {
        IR_Send_Bit(aux_frame[i]);
    }

    // 5. 发送帧间长间隔（40ms高电平）
    IR_Send_Interval(40);    
}
/**
 * 计算校验码：(mode - 1) + (temp - 16) + 5 取低4位后反序
 * 
 * 参数:
 *   mode: 模式值
 *   temp: 温度值
 * 
 * 返回:
 *   计算得到的校验码
 */
uint8_t calculate_checksum(uint8_t mode, uint8_t temp, uint8_t horizontal_swing, uint8_t ventilate) {
    // 计算基础值
    uint8_t base_value = (mode - 1) + (temp - 16) + 5 + horizontal_swing + ventilate;
    
    // 取低4位（即对16取模）
    uint8_t low4 = base_value % 16;
    // 处理负数情况，确保结果为正数
    // if (low4 < 0) {
    //     low4 += 16;
    // }
    
    // 反序4位二进制
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < 4; i++) {
        // 取出最低位并移到对应位置
        checksum |= ((low4 >> i) & 1) << (3 - i);
    }
    
    return checksum;
}
// 反转n位二进制数的辅助函数
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

    // 主数据帧（35位）
    uint8_t main_frame[35] = {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0};

    // 辅助数据帧（32位）
    uint8_t aux_frame[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    // 处理辅助数据帧字段
    // m: 上下扫风（1位）- 第0位
    aux_frame[0] = (upDownSwing > 0) ? 1 : 0;
    
    // 固定位：000 - 第1-3位保持0
    
    // n: 左右扫风（1位）- 第4位
    aux_frame[4] = (leftRightSwing > 0) ? 1 : 0;
    
    // 固定位：000 - 第5-7位保持0
    
    // o: 测温（2位）- 第8-9位
    aux_frame[8] = (tempMeasure & 0x01) ? 1 : 0;  // 高位
    aux_frame[9] = (tempMeasure & 0x02) ? 1 : 0;  // 低位

    // p: 校验码（4位）- 第28-31位
    uint8_t checksum = calculate_checksum(mode, temp, leftRightSwing, ventilate);

    // 校验码只取低4位
    aux_frame[28] = (checksum & 0x08) ? 1 : 0;
    aux_frame[29] = (checksum & 0x04) ? 1 : 0;
    aux_frame[30] = (checksum & 0x02) ? 1 : 0;
    aux_frame[31] = (checksum & 0x01) ? 1 : 0;

    // a: 模式 (3位) - 超过2位，采用反序递增
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

    // b: 开关 (1位)
    main_frame[3] = (power != 0) ? 1 : 0;

    // c: 风速 (2位) - 不超过2位，正常编码
    main_frame[4] = (speed & 0x01) ? 1 : 0;  // 高位
    main_frame[5] = (speed & 0x02) ? 1 : 0;  // 低位

    // d: 扫风 (1位)
    main_frame[6] = (swing > 0) ? 1 : 0;

    // e: 睡眠 (1位)
    main_frame[7] = (sleep > 0) ? 1 : 0;

    // f: 温度 (4位) - 超过2位，采用反序递增
    if(temp >= 16 && temp <= 31)
    {
        uint8_t temp_code = temp - 16;  // 0-15
        uint8_t reversed_temp = reverse_bits(temp_code, 4);
        main_frame[8]  = (reversed_temp & 0x08) ? 1 : 0;
        main_frame[9]  = (reversed_temp & 0x04) ? 1 : 0;
        main_frame[10] = (reversed_temp & 0x02) ? 1 : 0;
        main_frame[11] = (reversed_temp & 0x01) ? 1 : 0;
    }

    // g: 定时 (8位) - 超过2位，采用反序递增
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

    // h: 肌肉 (1位)
    main_frame[20] = (muscle > 0) ? 1 : 0;

    // i: 灯光 (1位)
    main_frame[21] = (light > 0) ? 1 : 0;

    // j: 负离子 (1位)
    main_frame[22] = (anion > 0) ? 1 : 0;

    // k: 辅热 (1位)
    main_frame[23] = (heat > 0) ? 1 : 0;

    // l: 换气 (1位)
    main_frame[24] = (ventilate > 0) ? 1 : 0;

    // 发送数据
    if(power != 0)
    {
        // 发送引导码（9ms低电平 + 4.5ms高电平）
        IR_Send_Leader_Code();
        
        // 发送主数据帧
        for (uint8_t i = 0; i < 35; i++) {
            IR_Send_Bit(main_frame[i]);
        }

        // 发送帧间短间隔（20ms高电平）
        IR_Send_Interval(20);

        // 发送辅助数据帧
        for (uint8_t i = 0; i < 32; i++) {
            IR_Send_Bit(aux_frame[i]);
        }

        // 发送帧间长间隔（40ms高电平）
        IR_Send_Interval(40);
    }
    else
    {
        AirGreePowerOff();  // 关闭空调
    }
}

void IR_Send_Test(){
    
}