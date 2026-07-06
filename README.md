# WS51F6240 单片机 Keil 工程模板

本仓库提供一个完整的 Keil C51 工程模板，专为 **WS51F6240** 8051 内核单片机设计。  
工程包含完整寄存器映射头文件、启动代码和示例主程序，帮助开发者快速上手该芯片的开发。

## 📁 文件结构

```
.
├── ws51f6240.h              # 寄存器映射头文件（SFR + XSFR）
├── main.c                   # 用户主程序示例
├── STARTUP.A51              # Keil 启动文件（清零 DATA/IDATA 等）
├── project.uvproj           # Keil μVision 项目文件
├── project.uvopt            # Keil 项目选项配置
├── project.code-workspace   # VS Code 工作区配置（可选）
├── .clang-format            # 代码格式化规则
├── .gitignore
├── build/                   # 编译输出目录
├── Listings/                # 汇编列表文件
└── Objects/                 # 目标文件（.obj, .hex 等）
```

## ✨ 主要特性

- **完整的寄存器定义**：  
  - `ws51f6240.h` 包含表 4.4‑1（SFR，地址 80H~FFH）和表 4.4‑2（XSFR，地址 8000H~8407H）的所有寄存器。
  - SFR 使用 `sfr` 关键字直接寻址，XSFR 使用绝对指针宏访问。
- **启动代码**：标准 `STARTUP.A51` 配置 DATA/IDATA 清零，确保 C 环境正常。
- **示例程序**：`main.c` 演示基本外设访问（可自行扩展）。
- **Keil 项目就绪**：直接打开 `project.uvproj` 即可编译、烧录。
- **VS Code 支持**：工作区文件包含，方便使用编辑器。

## 🚀 快速开始

### 1. 环境要求
- **Keil C51**（推荐 μVision 5）或 **SDCC**（需自行适配链接脚本）。
- **WS51F6240 芯片** 及对应的烧录工具（如 ISP 或调试器）。

### 2. 打开项目
- 双击 `project.uvproj` 启动 Keil μVision。
- 如需使用 VS Code，安装 `Keil Assistant` 或 `EIDE` 插件，打开工作区文件 `project.code-workspace`。

### 3. 编译与烧录
- 在 Keil 中点击 **Build**（或按 F7）编译工程，生成 HEX 文件（位于 `Objects/` 目录）。
- 使用烧录软件（如官方 ISP 工具）将 HEX 文件下载到芯片。

### 4. 修改代码
- 编辑 `main.c` 编写用户程序。
- 若需要使用额外寄存器，直接在 `ws51f6240.h` 中按格式补充即可。

## 📖 寄存器访问示例

```c
#include "ws51f6240.h"

void main(void) {
    // ----- SFR 访问（直接读写） -----
    P0 = 0x00;          // 设置 P0 口输出低电平
    IE  = 0x80;         // 使能总中断（EA = 1）

    // 位操作（需自行定义 sbit）
    sbit TR0 = TCON^4;  // 定时器0启动位
    TR0 = 1;            // 启动定时器

    // ----- XSFR 访问（宏方式） -----
    P00F = 0x01;        // 配置 P0.0 为第二功能
    uint8_t val = P00F; // 读取当前设置

    while(1) {
        // 主循环
    }
}
```

> **注意**：头文件中未预定义 `sbit` 位变量，用户可根据需要自行添加（参考芯片手册位定义）。

## ⚙️ 高级配置

### 优化 XSFR 访问效率
默认 XSFR 使用通用指针，若希望生成更高效的 `MOVX` 指令，可将宏定义中的指针改为 `__xdata` 修饰（Keil 特有）：
```c
#define P00F (*(volatile unsigned char __xdata *)0x8000)
```
或者对于 SDCC 编译器，使用 `xdata`：
```c
#define P00F (*(volatile unsigned char xdata *)0x8000)
```

### 添加位定义
在 `ws51f6240.h` 末尾添加常用的 `sbit`，例如：
```c
sbit EA   = IE^7;    // 总中断
sbit ET0  = IE^1;    // 定时器0中断
sbit TR0  = TCON^4;  // 定时器0运行控制
```

## 📚 寄存器映射参考

| 类别 | 地址范围   | 访问方式      | 说明                       |
|------|------------|---------------|----------------------------|
| SFR  | 80H~FFH    | `sfr` 关键字  | 直接寻址，速度最快         |
| XSFR | 8000H~8407H| 绝对指针宏    | XDATA 空间，需通过 `MOVX` 访问 |

完整映射请参阅芯片数据手册（表 4.4‑1 和 4.4‑2）或查看头文件内注释。

## ❗ 注意事项

- **保留地址**：头文件中已跳过所有标记为 `-` 的保留地址，切勿访问，否则可能导致不可预知行为。
- **外设时钟**：部分 XSFR 对应的外设需先使能时钟（如 PWM、I2C 等），请参考手册配置相关控制寄存器。
- **堆栈配置**：`STARTUP.A51` 默认设置堆栈指针为 `IDATALEN`（通常 0x80），可根据 RAM 大小调整。
- **晶振频率**：定时器计算中，默认使用 12MHz 晶振，若不同请调整相应的重载值。

## 📝 许可证

本项目采用 **MIT 许可证**，允许自由使用、修改和分发。详见 [LICENSE](LICENSE) 文件（若不存在，则默认 MIT）。

## 🤝 贡献

欢迎提交 Issue 或 Pull Request 来改进此模板。如有任何使用问题，也请在 Issues 中提出。

---
**Happy Coding!** 🎉
```