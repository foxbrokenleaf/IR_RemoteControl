#ifndef REG_MAP_H
#define REG_MAP_H

/*------------------------------------------------
 * 合并特殊功能寄存器（SFR）与扩展特殊功能寄存器（XSFR）映射表
 * 依据表 4.4-1（SFR，地址 80H~FFH）和表 4.4-2（XSFR，地址 8000H~8407H）
 *------------------------------------------------*/

/* ====================================================
 * 第一部分：SFR（特殊功能寄存器）—— 直接寻址，使用 sfr 关键字
 * ==================================================== */

/* ---------- 80H 行 ---------- */
sfr P0     = 0x80;   // P0口
sfr SP     = 0x81;   // 堆栈指针
sfr DPOL   = 0x82;   // 数据指针0低字节
sfr DPOH   = 0x83;   // 数据指针0高字节
sfr DP1L   = 0x84;   // 数据指针1低字节
sfr DP1H   = 0x85;   // 数据指针1高字节
sfr SCCON  = 0x86;   // 串口控制（或SmartCard控制）
sfr PCON   = 0x87;   // 串口控制

/* ---------- 88H 行 ---------- */
sfr TCON   = 0x88;   // 定时器/计数器控制
sfr TMOD   = 0x89;   // 定时器模式
sfr TL0    = 0x8A;   // 定时器0低字节
sfr TL1    = 0x8B;   // 定时器1低字节
sfr TH0    = 0x8C;   // 定时器0高字节
sfr TH1    = 0x8D;   // 定时器1高字节
// 0x8E, 0x8F 保留

/* ---------- 90H 行 ---------- */
sfr P1     = 0x90;   // P1口
// 0x91~0x94 保留
sfr XSCON  = 0x95;   // 外部晶振控制
sfr HRCON  = 0x96;   // 高阻控制
sfr LRCON  = 0x97;   // 低阻控制

/* ---------- 98H 行 ---------- */
sfr SOCON  = 0x98;   // 串口0控制
sfr SOBUF  = 0x99;   // 串口0数据缓冲
sfr SOCFG  = 0x9A;   // 串口0配置
sfr S1CON  = 0x9B;   // 串口1控制
sfr S1BUF  = 0x9C;   // 串口1数据缓冲
sfr S1RELL = 0x9D;   // 串口1重载低字节
sfr S1RELH = 0x9E;   // 串口1重载高字节
sfr RSTFLG = 0x9F;   // 复位标志

/* ---------- A0H 行 ---------- */
sfr P2     = 0xA0;   // P2口
sfr I2CCON = 0xA1;   // I2C控制
sfr I2CADR = 0xA2;   // I2C从机地址
sfr I2CCFG0= 0xA3;   // I2C配置0
sfr I2CFG1 = 0xA4;   // I2C配置1
sfr I2CTXD = 0xA5;   // I2C发送数据
sfr I2CRXD = 0xA6;   // I2C接收数据
sfr I2CFLG = 0xA7;   // I2C标志

/* ---------- A8H 行 ---------- */
sfr IE     = 0xA8;   // 中断使能
sfr WDTCON = 0xA9;   // 看门狗控制
sfr WDTFLG = 0xAA;   // 看门狗标志
// 0xAB~0xAD 保留
sfr TL3    = 0xAE;   // 定时器3低字节
sfr TH3    = 0xAF;   // 定时器3高字节

/* ---------- B0H 行 ---------- */
// 0xB0, 0xB1 保留
sfr T3CON  = 0xB2;   // 定时器3控制
sfr T3MOD  = 0xB3;   // 定时器3模式
sfr T3RL   = 0xB4;   // 定时器3重载低字节
sfr T3RH   = 0xB5;   // 定时器3重载高字节
sfr T3CL   = 0xB6;   // 定时器3捕获/比较低字节
sfr T3CH   = 0xB7;   // 定时器3捕获/比较高字节

/* ---------- B8H 行 ---------- */
sfr IP     = 0xB8;   // 中断优先级
sfr LVDCON = 0xB9;   // 低压检测控制
// 0xBA~0xBE 保留
sfr ADCBGP = 0xBF;   // ADC带隙基准

/* ---------- C0H 行 ---------- */
sfr ADCON  = 0xC0;   // ADC控制
sfr ADCFG  = 0xC1;   // ADC配置
sfr ADCHS  = 0xC2;   // ADC通道选择
sfr ADCDL  = 0xC3;   // ADC数据低字节
sfr ADCDH  = 0xC4;   // ADC数据高字节
sfr ADCPC  = 0xC5;   // ADC预分频控制
sfr ADCPDL = 0xC6;   // ADC预分频低字节
sfr ADCPDH = 0xC7;   // ADC预分频高字节

/* ---------- C8H 行 ---------- */
sfr T2CON  = 0xC8;   // 定时器2控制
sfr T2MOD  = 0xC9;   // 定时器2模式
sfr T2CL   = 0xCA;   // 定时器2捕获/比较低字节
sfr T2CH   = 0xCB;   // 定时器2捕获/比较高字节
sfr TL2    = 0xCC;   // 定时器2低字节
sfr TH2    = 0xCD;   // 定时器2高字节
sfr SCMCON = 0xCE;   // 智能卡模块控制
sfr SCMFLG = 0xCF;   // 智能卡模块标志

/* ---------- D0H 行 ---------- */
sfr PSW    = 0xD0;   // 程序状态字
sfr WKTCON = 0xD1;   // 唤醒定时器控制
sfr WKTS   = 0xD2;   // 唤醒定时器状态
sfr OP0CON = 0xD3;   // 运算放大器0控制
sfr OPCFG  = 0xD4;   // 运算放大器配置
// 0xD5~0xD7 保留

/* ---------- D8H 行 ---------- */
sfr EPIE   = 0xD8;   // 扩展外设中断使能
sfr EPIF   = 0xD9;   // 扩展外设中断标志
sfr EP0CON = 0xDA;   // 扩展外设0控制
sfr EP1CON = 0xDB;   // 扩展外设1控制
sfr EP2CON = 0xDC;   // 扩展外设2控制
sfr EP3CON = 0xDD;   // 扩展外设3控制
// 0xDE, 0xDF 保留

/* ---------- E0H 行 ---------- */
sfr ACC    = 0xE0;   // 累加器
sfr SPCON  = 0xE1;   // 串口（或SPI）控制
sfr SPDAT  = 0xE2;   // 串口（或SPI）数据
sfr SPFLG  = 0xE3;   // 串口（或SPI）标志
// 0xE4~0xE7 保留

/* ---------- E8H 行 ---------- */
sfr EXIE   = 0xE8;   // 中断优先级寄存器
// 0xE9~0xEF 保留

/* ---------- F0H 行 ---------- */
sfr B      = 0xF0;   // 通用寄存器 B (PS:需要和 累加器ACC 寄存器搭配使用)
// 0xF1~0xF7 保留

/* ---------- F8H 行 ---------- */
sfr EXIP   = 0xF8;   // 外部中断优先级控制
sfr PWCON  = 0xF9;   // 电源控制
sfr IT0CON = 0xFA;   // 定时器0中断控制
sfr IT1CON = 0xFB;   // 定时器1中断控制
sfr IDLFL  = 0xFC;   // 空闲标志低字节
sfr IDLFH  = 0xFD;   // 空闲标志高字节
sfr STPF   = 0xFE;   // 停止标志
// 0xFF 保留

/* ====================================================
 * 第二部分：XSFR（扩展特殊功能寄存器）—— XDATA 空间，使用绝对指针
 * ==================================================== */
// #define P03F (*(unsigned char volatile xdata *)0x8003)
/* ---------- 8000H 行 ---------- */
#define P00F   (*(unsigned char volatile xdata *)0x8000)   // 端口0引脚功能选择
#define P01F   (*(unsigned char volatile xdata *)0x8001)
#define P02F   (*(unsigned char volatile xdata *)0x8002)
#define P03F   (*(unsigned char volatile xdata *)0x8003)
#define P04F   (*(unsigned char volatile xdata *)0x8004)
#define P05F   (*(unsigned char volatile xdata *)0x8005)
#define P06F   (*(unsigned char volatile xdata *)0x8006)
#define P07F   (*(unsigned char volatile xdata *)0x8007)

/* ---------- 8008H 行 ---------- */
#define P10F   (*(unsigned char volatile xdata *)0x8008)
#define P11F   (*(unsigned char volatile xdata *)0x8009)
#define P12F   (*(unsigned char volatile xdata *)0x800A)
#define P13F   (*(unsigned char volatile xdata *)0x800B)
#define P14F   (*(unsigned char volatile xdata *)0x800C)
#define P15F   (*(unsigned char volatile xdata *)0x800D)
#define P16F   (*(unsigned char volatile xdata *)0x800E)
#define P17F   (*(unsigned char volatile xdata *)0x800F)

/* ---------- 8010H 行 ---------- */
#define P20F   (*(unsigned char volatile xdata *)0x8010)
#define P21F   (*(unsigned char volatile xdata *)0x8011)
// 0x8012~0x8017 保留

/* ---------- 8040H 行 ---------- */
#define PWMUN   (*(unsigned char volatile xdata *)0x8040)   // PWM单元控制
#define PWMCLR  (*(unsigned char volatile xdata *)0x8041)   // PWM清零
// 0x8042 保留
#define PWMUPD  (*(unsigned char volatile xdata *)0x8043)   // PWM更新
#define PWMFLG0 (*(unsigned char volatile xdata *)0x8044)   // PWM标志0
#define PWMFLG1 (*(unsigned char volatile xdata *)0x8045)   // PWM标志1
// 0x8046, 0x8047 保留

/* ---------- 8048H 行 ---------- */
#define PMEN    (*(unsigned char volatile xdata *)0x8048)   // 测量使能
#define PMDAT   (*(unsigned char volatile xdata *)0x8049)   // 测量数据
#define PMSCON  (*(unsigned char volatile xdata *)0x804A)   // 测量控制
#define PMSDL   (*(unsigned char volatile xdata *)0x804B)   // 测量数据低
#define PMSDH   (*(unsigned char volatile xdata *)0x804C)   // 测量数据高
#define PMSML   (*(unsigned char volatile xdata *)0x804D)   // 测量状态低
#define PMSMH   (*(unsigned char volatile xdata *)0x804E)   // 测量状态高
// 0x804F 保留

/* ---------- 8050H 行（PWM0） ---------- */
#define PWM0CFG (*(unsigned char volatile xdata *)0x8050)
#define PWM0PS  (*(unsigned char volatile xdata *)0x8051)
// 0x8052, 0x8053 保留
#define PWM0DUTL (*(unsigned char volatile xdata *)0x8054)
#define PWM0DUTH (*(unsigned char volatile xdata *)0x8055)
#define PWM0DIVL (*(unsigned char volatile xdata *)0x8056)
#define PWM0DIVH (*(unsigned char volatile xdata *)0x8057)

/* ---------- 8058H 行（PWM1） ---------- */
#define PWM1CFG (*(unsigned char volatile xdata *)0x8058)
#define PWM1PS  (*(unsigned char volatile xdata *)0x8059)
// 0x805A, 0x805B 保留
#define PWM1DUTL (*(unsigned char volatile xdata *)0x805C)
#define PWM1DUTH (*(unsigned char volatile xdata *)0x805D)
#define PWM1DIVL (*(unsigned char volatile xdata *)0x805E)
#define PWM1DIVH (*(unsigned char volatile xdata *)0x805F)

/* ---------- 8060H 行（PWM2） ---------- */
#define PWM2CFG (*(unsigned char volatile xdata *)0x8060)
#define PWM2PS  (*(unsigned char volatile xdata *)0x8061)
// 0x8062, 0x8063 保留
#define PWM2DUTL (*(unsigned char volatile xdata *)0x8064)
#define PWM2DUTH (*(unsigned char volatile xdata *)0x8065)
#define PWM2DIVL (*(unsigned char volatile xdata *)0x8066)
#define PWM2DIVH (*(unsigned char volatile xdata *)0x8067)

/* ---------- 8068H 行（PWM3） ---------- */
#define PWM3CFG (*(unsigned char volatile xdata *)0x8068)
#define PWM3PS  (*(unsigned char volatile xdata *)0x8069)
// 0x806A, 0x806B 保留
#define PWM3DUTL (*(unsigned char volatile xdata *)0x806C)
#define PWM3DUTH (*(unsigned char volatile xdata *)0x806D)
#define PWM3DIVL (*(unsigned char volatile xdata *)0x806E)
#define PWM3DIVH (*(unsigned char volatile xdata *)0x806F)

/* ---------- 8070H 行（PWM4） ---------- */
#define PWM4CFG (*(unsigned char volatile xdata *)0x8070)
#define PWM4PS  (*(unsigned char volatile xdata *)0x8071)
// 0x8072, 0x8073 保留
#define PWM4DUTL (*(unsigned char volatile xdata *)0x8074)
#define PWM4DUTH (*(unsigned char volatile xdata *)0x8075)
#define PWM4DIVL (*(unsigned char volatile xdata *)0x8076)
#define PWM4DIVH (*(unsigned char volatile xdata *)0x8077)

/* ---------- 8078H 行（PWM5） ---------- */
#define PWM5CFG (*(unsigned char volatile xdata *)0x8078)
#define PWM5PS  (*(unsigned char volatile xdata *)0x8079)
// 0x807A, 0x807B 保留
#define PWM5DUTL (*(unsigned char volatile xdata *)0x807C)
#define PWM5DUTH (*(unsigned char volatile xdata *)0x807D)
#define PWM5DIVL (*(unsigned char volatile xdata *)0x807E)
#define PWM5DIVH (*(unsigned char volatile xdata *)0x807F)

/* ---------- 8080H 行（PWM6） ---------- */
#define PWM6CFG (*(unsigned char volatile xdata *)0x8080)
#define PWM6PS  (*(unsigned char volatile xdata *)0x8081)
// 0x8082, 0x8083 保留
#define PWM6DUTL (*(unsigned char volatile xdata *)0x8084)
#define PWM6DUTH (*(unsigned char volatile xdata *)0x8085)
#define PWM6DIVL (*(unsigned char volatile xdata *)0x8086)
#define PWM6DIVH (*(unsigned char volatile xdata *)0x8087)

/* ---------- 8088H 行（PWM7） ---------- */
#define PWM7CFG (*(unsigned char volatile xdata *)0x8088)
#define PWM7PS  (*(unsigned char volatile xdata *)0x8089)
// 0x808A, 0x808B 保留
#define PWM7DUTL (*(unsigned char volatile xdata *)0x808C)
#define PWM7DUTH (*(unsigned char volatile xdata *)0x808D)
#define PWM7DIVL (*(unsigned char volatile xdata *)0x808E)
#define PWM7DIVH (*(unsigned char volatile xdata *)0x808F)

/* ---------- 80D0H 行 ---------- */
#define LEDCFG  (*(unsigned char volatile xdata *)0x80D0)   // LED配置
// 0x80D1 保留
#define LED0D0  (*(unsigned char volatile xdata *)0x80D2)   // LED0数据0
#define LED0D1  (*(unsigned char volatile xdata *)0x80D3)   // LED0数据1
#define LED0D2  (*(unsigned char volatile xdata *)0x80D4)   // LED0数据2
// 0x80D5~0x80D7 保留

/* ---------- 80D8H 行 ---------- */
// 0x80D8, 0x80D9 保留
#define LED1D0  (*(unsigned char volatile xdata *)0x80DA)   // LED1数据0
#define LED1D1  (*(unsigned char volatile xdata *)0x80DB)   // LED1数据1
#define LED1D2  (*(unsigned char volatile xdata *)0x80DC)   // LED1数据2
// 0x80DD~0x80DF 保留

/* ---------- 80E0H 行 ---------- */
#define FBCON   (*(unsigned char volatile xdata *)0x80E0)   // 反馈控制
#define FBDAT   (*(unsigned char volatile xdata *)0x80E1)   // 反馈数据
#define FBFLG   (*(unsigned char volatile xdata *)0x80E2)   // 反馈标志
// 0x80E3~0x80E7 保留

/* ---------- 80E8H 行 ---------- */
#define PIMOD   (*(unsigned char volatile xdata *)0x80E8)   // 引脚中断模式
#define POIMSK  (*(unsigned char volatile xdata *)0x80E9)   // P0中断屏蔽
#define P1IMSK  (*(unsigned char volatile xdata *)0x80EA)   // P1中断屏蔽
#define P2IMSK  (*(unsigned char volatile xdata *)0x80EB)   // P2中断屏蔽
// 0x80EC~0x80EF 保留

/* ---------- 80F0H 行 ---------- */
#define BZCON   (*(unsigned char volatile xdata *)0x80F0)   // 蜂鸣器控制
#define BZPSC   (*(unsigned char volatile xdata *)0x80F1)   // 蜂鸣器预分频
#define BZDUT   (*(unsigned char volatile xdata *)0x80F2)   // 蜂鸣器占空比
#define BDDIV   (*(unsigned char volatile xdata *)0x80F3)   // 蜂鸣器分频
// 0x80F4~0x80F7 保留

/* ---------- 82B0H 行 ---------- */
#define ACBCON  (*(unsigned char volatile xdata *)0x82B0)   // ACB控制
// 0x82B1 保留
#define AKDAT   (*(unsigned char volatile xdata *)0x82B2)   // AK数据
// 0x82B3 保留
#define AMDAT   (*(unsigned char volatile xdata *)0x82B4)   // AM数据
// 0x82B5~0x82B7 保留

/* ---------- 8300H 行 ---------- */
#define MECON   (*(unsigned char volatile xdata *)0x8300)   // ME控制
#define FSCMD   (*(unsigned char volatile xdata *)0x8301)   // FS命令
#define BTMASK  (*(unsigned char volatile xdata *)0x8302)   // BT掩码
#define FSDAT   (*(unsigned char volatile xdata *)0x8303)   // FS数据
#define FSFLG   (*(unsigned char volatile xdata *)0x8304)   // FS标志
#define FSMSK   (*(unsigned char volatile xdata *)0x8305)   // FS掩码
#define FSADRL  (*(unsigned char volatile xdata *)0x8306)   // FS地址低
#define FSADRH  (*(unsigned char volatile xdata *)0x8307)   // FS地址高

/* ---------- 8308H 行 ---------- */
#define CKSUML  (*(unsigned char volatile xdata *)0x8308)   // 校验和低
#define CKSUMH  (*(unsigned char volatile xdata *)0x8309)   // 校验和高
#define IDCODE0 (*(unsigned char volatile xdata *)0x830A)   // ID码0
#define IDCODE1 (*(unsigned char volatile xdata *)0x830B)   // ID码1
#define IDCODE2 (*(unsigned char volatile xdata *)0x830C)   // ID码2
// 0x830D~0x830F 保留

/* ---------- 8400H 行 ---------- */
// 0x8400~0x8406 保留
#define SPMAX   (*(unsigned char volatile xdata *)0x8407)   // 堆栈最大地址

/*------------------------------------------------
 * 使用说明：
 * 1. SFR 部分可直接使用寄存器名进行读写，如 P0 = 0x55;
 * 2. XSFR 部分通过宏访问，如 P00F = 0x01; 或 val = P00F;
 * 3. 为提高 XSFR 访问效率，可改为 xdata 指针：
 *    #define P00F (*(volatile unsigned char __xdata *)0x8000)
 * 4. 位操作可配合位掩码宏或直接使用位运算符。
 * 5. 保留地址未定义，访问可能导致不可预知行为。
 *------------------------------------------------*/

#endif /* REG_MAP_H */