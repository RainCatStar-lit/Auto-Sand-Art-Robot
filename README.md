# Auto-Sand-Art-Robot (星火计划悬赏项目)

> 嘉立创「星火计划」悬赏赛道参赛项目 | CoreXY 架构 | ESP32 | JLC-EDA 双层板 | Fusion360

![ESP32](https://img.shields.io/badge/MCU-ESP32-green?logo=espressif)
![CoreXY](https://img.shields.io/badge/Motion-CoreXY-orange)
![EDA](https://img.shields.io/badge/PCB-JLC--EDA-blue)
![CAD](https://img.shields.io/badge/CAD-Fusion_360-yellow?logo=autodesk)

## 我是谁，为什么做这个

我是 RainCatStar，广东实验中学高二学生，SSIR 机器人社成员。以前玩过 VEX 机器人，也用 Bambu A1 改过不少 3D 打印件，但一直没做过一台完整的运动控制平台。

这个沙画机器人，我做它纯粹是为了死磕两件事：一是平面运动控制（CoreXY 架构下的精准轨迹），二是粉体流控（沙子怎么匀速出、怎么不拖尾、怎么绝对不漏）。不是为了做什么炫酷产品，就是想把这些硬核工程问题一一啃下来。

## 技术栈

- **ESP32** —— 主控，Arduino 框架开发
- **CoreXY** —— 平面运动架构，双电机协同
- **JLC-EDA** —— 双层 PCB，我从零开始自学
- **Fusion360** —— 机械结构建模与 3D 打印件设计

## 现状与约束

- 80 天从 0 到 1 完成整机
- 此前未接触过嘉立创 EDA 和 RTOS，方案已降级：**嘉立创 EDA 双层板（自学中）** + **ESP32 Arduino 框架（millis() 多任务 + 状态机，不使用 RTOS）**
- 硬件电路板将在 80 天内从零开始自学嘉立创 EDA 完成双层板设计

## 核心技术推演（保留）

- **称重闭环流量标定**：HX711 称重，记录不同 PWM 下的出沙量，拟合流量曲线
- **毫秒级防漏阀**：电磁铁 + 30° 倒圆锥喉部 + 锥形硅胶塞，断电弹簧自锁，绝对防漏
- **震动流化协同**：关阀前 100 ms 震动马达满功率震散卡沙，随后电磁铁断电闭锁，解决拖尾
- **分离式供料**：料仓固定，软管连接移动端超轻出沙头

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

## 联系

- 作者：RainCatStar（广东实验中学 SSIR 机器人社）
- License：MIT