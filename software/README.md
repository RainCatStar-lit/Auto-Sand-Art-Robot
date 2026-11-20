# software

ESP32 固件、Python 图像处理管线、Web 端控制界面。

## 软件规划

基于 Arduino IDE 开发，放弃复杂的 RTOS，采用基于 `millis()` 的轻量级多任务调度与状态机。图像→G-code 的计算在手机或电脑端完成（Python + OpenCV 提取图片骨架生成 G-code），ESP32-S3 仅负责通过 WebServer 接收并执行 G-code，Web 端保持极简。

## 模块概览（预计）

```
software/
├── firmware/               # ESP32-S3 固件（Arduino IDE）
│   ├── motion/             # CoreXY 运动学、步进脉冲生成、软限位
│   ├── flow/               # PWM 流量控制、标定查找表
│   ├── valve/              # 阀门状态机、震动破拱时序（关阀前 100 ms 震动）
│   ├── sensor/             # HX711 称重读取、滑窗滤波
│   ├── scheduler/          # 基于 millis() 的轻量级多任务调度
│   └── webserver/          # ESP32 WebServer，局域网 G-code 下发与状态上报
├── img2gcode/              # Python + OpenCV（运行在手机/电脑端），图片→骨架→G-code
└── web/                    # 极简网页控制页（ESP32 WebServer 托管，局域网访问）
```

## 调度方式（核心）

不用 FreeRTOS，改用 Arduino 框架下的非阻塞调度：

- 每个子系统注册自己的处理函数和执行间隔；
- 主循环里用 `millis()` 判断是否到点，到点执行一小段后立即返回；
- 阀门/震动/出沙/停沙 用状态机串联，保证“关阀前 100 ms 震动满功率 → 电磁铁断电闭锁”的时序不串。

## 上位机与 Web

- **图像处理（端外）**：Python + OpenCV 在手机或电脑端运行，提取图片骨架，经向量化与路径优化后生成 G-code，通过局域网发送给 ESP32-S3。
- **Web 端（端内）**：ESP32-S3 自带 WebServer 托管极简网页，仅承担 G-code 下发、状态显示、急停等基础功能，**不搭建 Vue3/Three.js 等复杂前端**。

> 代码量不会很大，但每一行都要能在 Arduino IDE 里编译通过、能在 ESP32-S3 上跑稳。  
> —— RainCatStar