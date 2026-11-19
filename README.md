# Auto-Sand-Art-Robot 🏜️

> **嘉立创「星火计划」悬赏赛道参赛项目** | CoreXY 架构 | ESP32 嵌入式控制 | 称重闭环流量标定 | PWM 电磁铁精准流控

![ESP32](https://img.shields.io/badge/MCU-ESP32-green?logo=espressif)
![CoreXY](https://img.shields.io/badge/Motion-CoreXY-orange)
![EDA](https://img.shields.io/badge/PCB-JLC--EDA_Pro-blue?logo=github)
![CAD](https://img.shields.io/badge/CAD-Fusion_360-yellow?logo=autodesk)
![License](https://img.shields.io/badge/License-MIT-purple)

---

## 🎯 项目简介

这是一台 **全自动构图沙画机器人**，目标在 80 天内完成从 0 到 1 的工程落地，角逐嘉立创「星火计划」悬赏赛道。

**核心看点：**
- 🏗️ **CoreXY + 轻量化出沙头** —— 借用 A1 打印件，移动质量极致压缩
- ⚖️ **称重闭环流量标定（专利级创新）** —— HX711 实测质量反推流量，复刻 3D 打印机流量校准逻辑
- ⚡ **PWM 电磁铁 + 锥形塞零漏沙密封** —— 占空比调流、断电弹簧复位、震动破拱协同，攻克粉体流控终极难题
- 🎨 **图片 → 骨架 → G-code 全链路** —— Python + OpenCV 提取轮廓，Web 前端一键传图开画

---

## 📁 仓库结构

```
.
├── docs/           # 设计方案 & 80天计划
├── hardware/       # 嘉立创EDA工程、原理图、PCB、BOM
├── software/       # ESP32 固件、Python 图像处理、Web 前端
├── models/         # Fusion 360 源文件、STL 导出
└── README.md
```

---

## 🚀 快速上手（待完善）

> 项目处于竞标 / 早期研发阶段，完整构建指南将在 `docs/PLAN.md` 里的里程碑节点同步释放。

```bash
# 克隆仓库
git clone https://github.com/your-name/Auto-Sand-Art-Robot.git
cd Auto-Sand-Art-Robot
```

---

## 🤝 贡献 & 联系

- **作者**：RainCatStar（广东实验中学 SSIR 机器人社）
- **邮箱**：your-email@example.com
- **License**：MIT — 随便折腾，保留署名即可

> **极客不设限，沙画也能玩出工程美学。**