#ifndef __IR_SEND_H__
#define __IR_SEND_H__

#include "gd32f10x.h"
#include "gd32f10x_it.h"
#include "systick.h"

typedef enum{
    MODE_AUTO = 0,
    MODE_COLD,
    MODE_DRY,
    MODE_FAN,
    MODE_HOT
}U_MODE;

typedef enum{
    FAN_AUTO = 0,
    FAN_SLOW,
    FAN_MEDIUM,
    FAN_HIGH
}U_FANSPEED;

typedef enum{
    NOSWING = 0,
    H_SWING,
    V_SWING,
    HV_SWING
}U_SWING;

// 空调控制功能主数据
typedef struct {
    uint8_t mode;           // a:模式（可表示不同模式编号，如1=制冷、2=制热等）
    uint8_t power;          // b:开关（0=关闭，1=开启）
    uint8_t fan_speed;      // c:风速（可表示档位，如1=低速、2=中速、3=高速）
    uint8_t swing;          // d:扫风（0=关闭，1=开启）
    uint8_t sleep_mode;     // e:睡眠（0=关闭，1=开启）
    uint8_t temperature;    // f:温度（保留浮点型以表示精确温度值）
    uint8_t timer_hours;    // g:定时（定时小时数，0表示无定时）
    uint8_t muscle_function;// h:肌肉（0=关闭，1=开启）
    uint8_t light;          // i:灯光（0=关闭，1=开启）
    uint8_t negative_ion;   // j:负离子（0=关闭，1=开启）
    uint8_t auxiliary_heat; // k:辅热（0=关闭，1=开启）
    uint8_t ventilation;    // l:换气（0=关闭，1=开启
    // uint8_t UNKOWN_1 = 0b0001010010;
} AirConditionerControlsMainData;

typedef struct {
    uint8_t vertical_swing;
    uint8_t horizontal_swing;
    uint8_t temperature_read;
    uint8_t checksum;
}AirConditionerControlsAuxData;

typedef struct U_AirData{
    AirConditionerControlsMainData MainData;
    AirConditionerControlsAuxData AuxData;
} AirData;

void IR_Send_Init();
void IR_Send_Test();
void AirDataInit(AirData *ad);
// 主数据设置函数
void AirDataSetMode(AirData *ad, uint8_t value);
void AirDataSetPower(AirData *ad, uint8_t value);
void AirDataSetFanSpeed(AirData *ad, uint8_t value);
void AirDataSetSwing(AirData *ad, uint8_t value);
void AirDataSetSleepMode(AirData *ad, uint8_t value);
void AirDataSetTemperature(AirData *ad, uint8_t value);
void AirDataSetTimerHours(AirData *ad, uint8_t value);
void AirDataSetMuscleFunction(AirData *ad, uint8_t value);
void AirDataSetLight(AirData *ad, uint8_t value);
void AirDataSetNegativeIon(AirData *ad, uint8_t value);
void AirDataSetAuxiliaryHeat(AirData *ad, uint8_t value);
void AirDataSetVentilation(AirData *ad, uint8_t value);

// 辅助数据设置函数
void AirDataSetVerticalSwing(AirData *ad, uint8_t value);
void AirDataSetHorizontalSwing(AirData *ad, uint8_t value);
void AirDataSetTemperatureRead(AirData *ad, uint8_t value);
void AirDataSetChecksum(AirData *ad, uint8_t value);
void AirGree(uint8_t mode, uint8_t power, uint8_t speed, uint8_t swing, uint8_t sleep, uint8_t temp, uint8_t timer, uint8_t muscle, uint8_t light, uint8_t anion, uint8_t heat, uint8_t ventilate, uint8_t upDownSwing, uint8_t leftRightSwing, uint8_t tempMeasure);

#endif