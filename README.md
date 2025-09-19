# 红外遥控器 · 格力空调（当前版本）
## 成品图
正面
![FBL_0874](https://github.com/user-attachments/assets/6bef2835-e8c6-4f06-a035-9b938bfb3747)
反面
![FBL_0876](https://github.com/user-attachments/assets/74a450b0-f74d-4450-a722-967568e5f4ba)
## 界面介绍
+ 初始化界面
![FBL_0855](https://github.com/user-attachments/assets/92ae5c28-5335-4eb3-a992-b04245e67b34)
+ 欢迎界面
![FBL_0856](https://github.com/user-attachments/assets/e1486476-e5ec-4153-a226-47d8900ac7c2)
+ 主界面
  - 开关
![FBL_0857](https://github.com/user-attachments/assets/0f15045f-b375-4337-9ad5-cd98ee378f05)
    - 开
![power](https://github.com/user-attachments/assets/bdea2717-3041-4727-8406-1fe3e39f20fc)
    - 关
![power_close](https://github.com/user-attachments/assets/488d6690-0fcd-4e02-bb47-43cc5fe1adf0)
  - 模式
![FBL_0858](https://github.com/user-attachments/assets/e2b63ca0-03e2-49d1-823f-f6d3beb3e7fa)
    - 自动
![auto](https://github.com/user-attachments/assets/30e6e9c0-0589-401b-9346-a70ced7d2637)
    - 制冷
![snow-32](https://github.com/user-attachments/assets/4e902346-dd71-45d9-9e53-802cb763aab5)
    - 除湿
![dry-32](https://github.com/user-attachments/assets/dce4df8e-e131-4f55-8158-85e8908f1619)
    - 送风
![huanqi](https://github.com/user-attachments/assets/244d5ec7-49a3-4183-9445-2f5524a9befa)
    - 制热
![sun-32](https://github.com/user-attachments/assets/fa725feb-b5f4-43bf-80c2-0ad24d95ca86)
  - 温度
![FBL_0859](https://github.com/user-attachments/assets/1f038874-70e2-4fe8-9608-a541881f5190)
  - 风速
![FBL_0860](https://github.com/user-attachments/assets/96da8a88-7f71-4466-9ae6-c889c70375ae)
    - 自动
![Fan-32](https://github.com/user-attachments/assets/0467fc36-204e-45d9-8832-f3a3d35c8e8a)
    - 一档：1条线
    - 二档：2条线
    - 三档：3条线
  - 灯光
![FBL_0861](https://github.com/user-attachments/assets/b50bb41a-5a4c-441e-86d9-8ae6b8f40a7b)
  - 扫风
![FBL_0862](https://github.com/user-attachments/assets/0537ab79-9482-4563-bb9a-1e9289590d05)
  - 睡眠
![FBL_0863](https://github.com/user-attachments/assets/67afb339-a42d-4102-8a98-c4f2f822146f)
  - 辅热/干燥
![FBL_0864](https://github.com/user-attachments/assets/13813ea7-4301-4359-82b5-748f42090e00)
  - WiFi
![FBL_0865](https://github.com/user-attachments/assets/9792d35b-165d-4a82-8d34-c62a4f4aa1f6)
![FBL_0869](https://github.com/user-attachments/assets/364c1ac7-d459-4a6c-9d64-1bd7ac6d4fef)
  - 升级
![FBL_0866](https://github.com/user-attachments/assets/4d8156b4-e3f1-483a-b118-22306ea89787)
![FBL_0870](https://github.com/user-attachments/assets/3f7c5f1c-b5cb-4416-a7e8-a6306e2ac242)
  - 信息(芯片UID)
![FBL_0867](https://github.com/user-attachments/assets/46d2a9e3-27c6-4e19-a3f3-9e4cfa5b8055)
![FBL_0868](https://github.com/user-attachments/assets/311120cd-033d-424b-a863-c92a8dd02249)
## 使用说明
+ 图标下方的蝴蝶结指示器是指示当前选中的图标，同时起到表示系统正在运行的功能（约1秒闪烁一次）
+ 在主界面任意（除了Wi-Fi、升级、信息）图标时按下中间的按键即可发射红外数据，发射时同时伴随着如下图红圈中的图标出现
![FBL_0877](https://github.com/user-attachments/assets/a62d53f6-f5bc-4167-a7c7-10eeed3cb8cb)
+ 在主界面按下左/右按键时，对应方向的小箭头会亮起
![FBL_0857](https://github.com/user-attachments/assets/fc71c3a6-344e-4ae9-86fa-6861a026aaf3)
+ 在主界面任意（除了Wi-Fi、升级、信息）图标时按下上/下按键可以修改当前图标的状态/数值/模式
## 原理图
<img width="1275" height="845" alt="SCH_Schematic1_1-P1_2025-09-19" src="https://github.com/user-attachments/assets/d758cc13-5b15-4d87-8070-6baa2dd1a5f2" />

## PCB Layout图
+ 顶层
<img width="576" height="1095" alt="屏幕截图 2025-09-19 193312" src="https://github.com/user-attachments/assets/89434b24-3d8a-488a-bc85-c25c0a0598b8" />

+ 底层
<img width="613" height="1116" alt="屏幕截图 2025-09-19 193317" src="https://github.com/user-attachments/assets/3a6ae05e-59af-4319-ae23-0e64540133ff" />

## 开发计划
+ Wi-Fi
+ 电池电量检测
+ 板载下载器
## 计划移除
+ 红外接收器
+ 主控晶振
+ NE555 -> 红外发射管间的MOS管
## 开源声明 GNU2.0
