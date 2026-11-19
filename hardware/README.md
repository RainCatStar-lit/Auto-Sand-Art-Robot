# Hardware 目录

> **嘉立创 EDA 专业版**工程文件、原理图、PCB、BOM、Gerber 等均放在此目录。

---

## 目录规划

```
hardware/
├── eda/                    # 嘉立创 EDA 专业版工程源文件
│   ├── SSAP_ASR.pro       # 工程文件
│   ├── schematic/         # 原理图页面
│   │   ├── main.sch       # 主控+电源
│   │   ├── stepper.sch    # 步进驱动×2
│   │   ├── hx711.sch      # 称重模块
│   │   ├── solenoid.sch   # 电磁铁驱动
│   │   ├── vib_motor.sch  # 震动马达驱动
│   │   └── sensors.sch    # 红外对管、状态灯、按键
│   ├── pcb/               # PCB Layout
│   │   ├── SSAP_ASR.pcb   # 4层板文件
│   │   └── rules/         # 设计规则、阻抗约束
│   ├── lib/               # 自建封装/原理图符号库
│   │   ├── ESP32-S3-WROOM.lib
│   │   ├── TMC2209.lib
│   │   ├── HX711.lib
│   │   ├── AO3400.lib
│   │   ├── DRV8837.lib
│   │   └── Spark_Logo.lib # 星火计划 Logo 足迹
│   └── output/            # 导出生产文件
│       ├── gerber/        # Gerber RS-274X
│       ├── drill/         # 钻孔文件
│       ├── pick_place/    # SMT 坐标文件
│       └── bom/           # BOM 表（JLC 格式）
├── mechanical/            # 机电接口定义
│   ├── connector_map.md   # 连接器对应表（PH2.0/JST/XH 等）
│   ├── pinout.md          # ESP32 引脚分配表
│   └── wiring_harness.md  # 线束规格、长度、标号
├── simulation/            # 仿真/验证
│   ├── power_si/          # 电源完整性仿真报告
│   ├── emc/               # EMC 预测试报告
│   └── thermal/           # 热仿真/实测数据
├── BOM_v0.1.xlsx          # 总 BOM（含采购链接、备选型号）
├── BOM_SMT.csv            # JLC SMT 贴片单
├── schematic.pdf          # 原理图导出 PDF（带网络标号）
├── pcb_3d_preview.png     # PCB 3D 效果图
└── assembly_drawing.pdf   # 组装图（元器件编号+极性标识）
```

---

## 关键设计规则（摘录）

| 规则项 | 要求 |
|--------|------|
| 层叠 | Signal (Top) - GND - PWR - Signal (Bottom) |
| 板厚 | 1.6 mm |
| 最小线宽/间距 | 0.127 mm / 0.127 mm (5/5 mil) |
| 过孔 | 0.3 mm 机械孔，0.15/0.3 mm 激光微孔（盲埋孔不用） |
| 阻抗控制 | USB D+/D- 90Ω 差分、天线 50Ω 单端 |
| 铺铜 | Top/Bottom 尽量铺 GND，内层完整 GND/PWR 平面 |
| 丝印 | 必须印制 **嘉立创星火计划 Logo** + `SSAP-ASR v0.1` + 版本日期 |
| 表面处理 | 沉金 (ENIG) —— 便于后续手焊 BGA/细间距 QFN |

---

## 电源树概览

```
24V DC IN (DC-Jack / PH2.0-2P)
    │
    ├─→ 5A 贴片保险丝 (F1)
    │     │
    │     ├─→ TMC2209 VM (×2) ──────────────────► 步进电机
    │     │
    │     ├─→ 电磁铁线圈 (Q1 AO3400 低边) ──────► 24V/1A 峰值
    │     │
    │     └─→ MP2307 (U6) 24V→5V/3A ───────────┬─→ 5V 总线
    │                                           │
    │                                           ├─→ XC6206 (U7) 5V→3.3V/800mA ──► ESP32、HX711、传感器
    │                                           │
    │                                           └─→ DRV8837 (U5) 震动马达驱动 (5V 供电)
    │
    └─→ 电源指示 LED (绿)
```

---

## ESP32-S3 引脚分配（初版，以 `mechanical/pinout.md` 为准）

| GPIO | 功能 | 备注 |
|------|------|------|
| GPIO 0 | Boot 直靴 | 留测试点 |
| GPIO 1/3 | USB CDC TX/RX | 复用 USB-C |
| GPIO 2 | 状态灯 WS2812 数据 | 共用 1 线 |
| GPIO 4/5 | I2C (保留扩展) | 4.7k 上拉 |
| GPIO 6/7 | SPI Flash (PSRAM) | 禁用 |
| GPIO 8/9/10/11 | SPI0/1 (Flash) | 禁用 |
| GPIO 12 | HX711 DOUT | 输入 |
| GPIO 13 | HX711 PD_SCK | 输出 |
| GPIO 14 | 电磁铁 PWM (LEDC ch0) | 5 kHz |
| GPIO 15 | 震动马达 PWM (LEDC ch1) | 200 Hz |
| GPIO 16/17 | TMC2209 UART (X 轴) | 硬件串口 |
| GPIO 18/19 | TMC2209 UART (Y 轴) | 硬件串口 |
| GPIO 20/21 | I2C 备用 / GPIO |  |
| GPIO 22 | 红外对管 ADC | ADC1_CH0 |
| GPIO 23 | 急停按键 (中断) | 上拉 + 滤波 |
| GPIO 24 | 限位开关 X (中断) |  |
| GPIO 25 | 限位开关 Y (中断) |  |
| GPIO 26/27 | 步进脉冲/方向 (备用并行) | TMC UART 模式下释放 |
| GPIO 32/33/34/35/36/37/38/39 | 保留 / ADC / 触摸 |  |

> **注意**：最终引脚表以 PCB 原理图网络标号为准，软件配置 `sdkconfig` / `board_config.h` 需同步更新。

---

## 版本迭代记录

| 版本 | 日期 | 主要变更 | 负责人 |
|------|------|----------|--------|
| v0.1 | 2026-XX-XX | 首版原理图完成，PCB Layout 进行中 | 硬件组 |
| v0.2 |  |  |  |
| v1.0 |  | 定版送样 |  |

---

## 交付清单（星火计划要求）

- [ ] 嘉立创 EDA 专业版工程源文件 (`.zip`)
- [ ] Gerber + 钻孔 + 坐标文件 (制造包)
- [ ] BOM 表 (JLC 格式 `.csv` + 可读 `.xlsx`)
- [ ] 原理图 PDF (带网络标号)
- [ ] PCB 3D 预览图 (渲染图 + 尺寸标注)
- [ ] 组装图 PDF (元器件编号、极性、测试点)
- [ ] 电源完整性/EMC/热仿真报告 (可选加分项)
- [ ] **PCB 丝印含星火计划 Logo** ✅

---

> **Hardware is hard, but we make it reliable.**  
> —— RainCatStar