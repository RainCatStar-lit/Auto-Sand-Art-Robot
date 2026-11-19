# Auto-Sand-Art-Robot (星火计划悬赏项目)

> 嘉立创「星火计划」悬赏赛道参赛项目 | CoreXY 架构 | ESP32 | JLC-EDA 双层板 | Fusion360

![ESP32](https://img.shields.io/badge/MCU-ESP32-green?logo=espressif)
![CoreXY](https://img.shields.io/badge/Motion-CoreXY-orange)
![EDA](https://img.shields.io/badge/PCB-JLC--EDA-blue)
![CAD](https://img.shields.io/badge/CAD-Fusion_360-yellow?logo=autodesk)

## 我是谁，为什么做这个

我是 RainCatStar，UTC+8时区 高中学生， 在 VEX 机器人赛场与 PID 闭环、运动规划打交道；自学 Fusion 360 参数化设计，拥有一台 Bambu Lab A1，在长期改装 3D 打印机的过程中积累了从切片策略到结构减重、共振抑制的工程经验；自学 ESP32 开发，并开始用 GitHub 规范管理项目。

我擅长的技术领域包括：机械结构设计与 3D 打印（Fusion 360）、嵌入式开发（ESP32/Arduino）、运动控制逻辑以及快速应用开发。我具备强大的自学能力，能在极短时间内掌握嘉立创 EDA 的基础操作并完成双层 PCB 设计。

做这个沙画机器人的初衷很简单：想拥有一个完全属于自己的平面运动控制探索平台，把脑海里的设计做成真正能跑通的工程样机。

## 项目概况

- **周期**：80 天，从 0 到 1
- **预算**：约 2000-3000 元
- **有效工作区**：256 × 256 mm 沙画工作区
- **主控**：ESP32（Arduino 框架，多任务定时器）
- **PCB**：嘉立创 EDA 专业版双层板，印有星火计划 Logo
- **授权**：全部代码、3D 模型与电路图开源，MIT License

## 硬件架构

运动平台采用 CoreXY 架构，利用皮带与 TMC2209 等步进电机实现精度平面移动。

供料系统采用分离式设计：机架固定料仓，通过软管连接移动质量极轻的出沙头，减少移动惯性。

工具头计划使用震动出料和电磁阀断料的方案——前者通过调整震动电机的输出频率来改变出砂流量，后者通过电磁铁控制阀门闭合。

考虑到粉体对电子的破坏，整机将进行模块化防尘隔离设计。

## 核心创新

1. **称重闭环流量标定**  
   在回收盒底部安装称重传感器，通过不同 PWM 频率出沙记录质量变化，系统自动拟合流量曲线，完美复现 3D 打印机的流量校准逻辑。

2. **锥形防漏阀**  
   采用电磁铁结合锥形硅胶塞，断电时利用弹簧自锁阀门，防止漏沙。

3. **震动流化协同**  
   在关阀前一定时间内，震动马达瞬间满功率震散卡沙，随后电磁锁死，解决漏沙拖尾问题。

## 软件层面

主控采用 ESP32（基于 Arduino 框架），利用多任务定时器处理运动与流量控制。

图像转 G 代码链路由 Python 脚本实现，提取图片轮廓并生成控制指令。

交付时将开源所有代码、3D 模型与电路图，最后邮寄验收。

## 现状与约束

- 此前未接触过嘉立创 EDA 和 RTOS，方案已降级：**嘉立创 EDA 双层板（自学中）** + **ESP32 Arduino 框架（millis() 多任务 + 状态机，不使用 RTOS）**
- 硬件电路板将在 80 天内从零开始自学嘉立创 EDA 完成双层板设计

## 仓库结构

```
.
├── docs/           # 设计方案（DESIGN.md）与学习笔记
├── hardware/       # 嘉立创 EDA 工程、原理图、PCB、BOM（双层板）
├── software/       # ESP32 固件（Arduino IDE）、Python 图像处理、Web 端
├── models/         # Fusion 360 源文件、STL 导出
└── README.md
```

## 快速上手

项目处于立项/早期研发阶段，完整构建指南将随 80 天里程碑同步释放。

```bash
git clone https://github.com/RainCatStar-lit/Auto-Sand-Art-Robot.git
cd Auto-Sand-Art-Robot
```

---


## 🤝 贡献 & 联系

- **作者**：RainCatStar（广东实验中学 SSIR 机器人社）
- **邮箱**：zhaojoy2022d@163.com
- **License**：MIT — 随便折腾，保留署名即可

> **rcs希望沙画也能玩出工程美学。**
