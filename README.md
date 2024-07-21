<!--
 * @Author: 2353919304@qq.com
 * @Date: 2024-07-21 21:31:13
 * @LastEditors: 2353919304@qq.com
 * @LastEditTime: 2024-07-21 23:13:58
 * @FilePath: \stm32Template\README.md
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
-->
#基于STM32F103系列的格力空调遥控器
    ##开发环境
    +Visual Studio Code + EIDE + DAP
    +stm32f103c8t6
    +红外线发射器
    ##项目背景
    +宿舍没有空调遥控器
    +手机自身不内置红外遥控功能，每次开关空调都要插上外置红外遥控器（十分麻烦！）
    ##引脚使用情况
    +GPIOA的Pin_0用于空调的开
    +GPIOA的Pin_1用于空调的关
    +GPIOC的Pin_13用于提示按键按下
    +GPIOC的Pin_14用于红外遥控
    ##思路启发
    +爱上半导体的红外遥控工作原理[https://www.bilibili.com/video/BV1AM411t7Nc/?spm_id_from=333.337.search-card.all.click&vd_source=1d445b1c3b05069f509cd96e77d440c7]
    ##红外遥控数据来源
        ###使用逻辑分析仪 + HS0038捕获
            ####捕获到的功能"开"的数据
            功能"开"的前半段
            ![开的前半段](/IR_Data/Start_Befor.png)
            功能"开"的后半段
            ![开的后半段](/IR_Data/Start_After.png)
            ####捕获到的功能"关"的数据
            功能"关"的前半段
            ![关的前半段](/IR_Data/Stop_Befor.png)
            功能"关"的后半段
            ![关的后半段](/IR_Data/Stop_After.png)
    ##协议NEC(注意：不是普通的NEC协议)            