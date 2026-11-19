# Models 目录

> Fusion 360 参数化设计源文件、STL 导出、切片配置、打印件清单。

---

## 目录结构

```
models/
├── fusion360/              # Fusion 360 源文件（.f3d 仅本地编辑，Git 存导出格式）
│   ├── SSAP_ASR_Main.f3d   # 总装配（参数化驱动）
│   ├── components/         # 子组件（各自独立时间线）
│   │   ├── Frame/          # 机架、型材切割图
│   │   ├── CoreXY/         # X/Y 线性模组、同步带张紧器
│   │   ├── SandHead/       # 出沙头 V1~V3 迭代历史
│   │   ├── Hopper/         # 料仓、管路固定件
│   │   ├── WeighBin/       # 称重回收盒、传感器座
│   │   ├── Electronics/    # PCB 固定座、线束扎带座、风扇导风罩
│   │   └── Enclosure/      # 一体化防尘外壳、Logo 面板
│   └── drawings/           # 2D 工程图（.pdf/.dwg）
│       ├── Frame_CutList.pdf
│       ├── CoreXY_Assembly.pdf
│       └── SandHead_Detail.pdf
│
├── stl/                    # 切片就绪 STL（按版本标签归档）
│   ├── v0.1_M1/            # Phase 1 机械验证版
│   │   ├── Frame_*.stl
│   │   ├── CoreXY_*.stl
│   │   └── ...
│   ├── v0.2_M2/            # Phase 2 电气集成版
│   ├── v0.3_M3/            # Phase 3 出沙头 V3 定型版
│   └── v1.0_Final/         # 最终交付版
│
├── slicer/                 # 切片器配置与项目文件
│   ├── bambu_studio/       # Bambu Studio 项目文件
│   │   ├── SSAP_ASR_v1.0.3mf
│   │   └── filament_profiles/  # PLA-CF / PETG-CF 专用工艺包
│   └── orca_slicer/        # OrcaSlicer 备用配置
│
├── bom/                    # 打印件 BOM（自动从 Fusion 导出）
│   ├── print_bom_v1.0.csv  # 件号、名称、数量、材料、填充、预估重量、打印时间
│   └── cut_list_v1.0.csv   # 型材切割清单（长度、数量、攻丝孔位）
│
├── reference/              # 参考模型/标准件（STEP/STL）
│   ├── MGN12H.step
│   ├── NEMA17_42_40.step
│   ├── GT2_20T_Pulley.step
│   └── Spark_Logo.step     # 星火计划 Logo 3D 模型
│
└── renders/                # 关键节点渲染图（用于文档/答辩）
    ├── v0.1_exploded.png
    ├── v0.3_assembled.png
    └── v1.0_final_hero.png
```

---

## 参数化设计核心参数（在 Main.f3d 顶层参数表驱动）

| 参数名 | 单位 | 默认值 | 说明 |
|--------|------|--------|------|
| `bed_size_x` | mm | 320 | X 有效行程（含余量） |
| `bed_size_y` | mm | 320 | Y 有效行程 |
| `profile_series` | - | "2020" | 型材规格 |
| `linear_rail` | - | "MGN12H" | 导轨型号，驱动滑块座孔距 |
| `belt_pitch` | mm | 2 | GT2 齿距 |
| `pulley_teeth` | - | 20 | 同步带轮齿数 |
| `motor_flange` | mm | 42 | 步进电机法兰 |
| `hopper_volume` | mL | 500 | 料仓容积 |
| `tube_id` | mm | 4 | 硅胶管内径 |
| `nozzle_cone_angle` | deg | 30 | 倒圆锥喉部半锥角 |
| `seal_cone_angle` | deg | 30 | 锥形塞匹配角度 |
| `silicone_pad_thickness` | mm | 0.5 | 硅胶垫厚度 |
| `spring_preload` | N | 0.3 | 弹簧预压力 |
| `vib_motor_mount` | - | "coin_10mm" | 扁平马达安装座规格 |
| `pcb_outline` | mm | 120x80 | PCB 外形尺寸 |
| `enclosure_thickness` | mm | 3 | 外壳壁厚 |

> **修改参数即可全局联动更新**，所有子组件通过「链接参数」引用顶层参数。

---

## 打印工艺规范（Bambu Lab A1 专用）

| 项目 | PLA-CF (结构件) | PETG-CF (受力/耐热件) | TPU (软性密封件) |
|------|-----------------|----------------------|------------------|
| 层高 | 0.20 mm | 0.20 mm | 0.25 mm |
| 填充率 | 15% 陀螺 | 20% 陀螺 | 30% 网格 |
| 壁厚 | 3 层 (0.6 mm) | 4 层 (0.8 mm) | 2 层 |
| 底/顶层 | 4/6 层 | 5/7 层 | 3/4 层 |
| 打印温度 | 250°C | 260°C | 230°C |
| 热床温度 | 50°C | 70°C | 40°C |
| 打印速度 | 外壁 150 mm/s | 外壁 120 mm/s | 80 mm/s |
| 回抽 | 0.8 mm / 40 mm/s | 1.0 mm / 35 mm/s | 0.5 mm / 20 mm/s |
| 干燥 | 60°C 8h (必烘) | 65°C 8h (必烘) | 50°C 6h |
| 备注 | 抗拉强度高、不易翘曲 | 耐热 80°C、层间结合强 | 仅用于硅胶垫压板等软性件 |

**支撑策略**：
- 仅对悬垂 > 45° 且无法通过桥接解决的区域启用树形支撑
- 接触面设为 0.15 mm，易剥离
- 关键配合面（滑块座、电磁铁座、传感器座）**禁用支撑**，设计自支撑倒扣结构

---

## 版本发布流程

1. **Fusion 360 中完成设计冻结** → 创建版本标签 `vX.Y_MZ`
2. **导出 STEP + STL** → 放入 `stl/vX.Y_MZ/`，文件名规范：`<组件>_<件号>.stl`
3. **运行 BOM 自动导出脚本**（Fusion API） → 更新 `bom/print_bom_vX.Y.csv`
4. **Bambu Studio 导入 STL、套用工艺包、切片验证** → 保存 `.3mf` 到 `slicer/bambu_studio/`
5. **渲染关键视图** → 存入 `renders/`
6. **Git 提交**：
   ```bash
   git add models/stl/vX.Y_MZ models/bom/print_bom_vX.Y.csv models/slicer/...
   git commit -m "chore(models): release vX.Y_MZ for Phase Z"
   git tag -a "hw/vX.Y_MZ" -m "Hardware freeze for Milestone Z"
   ```

---

## 关键配合公差（实测调整值，已写入参数表）

| 配合对 | 公差带 | 实测建议值 | 备注 |
|--------|--------|------------|------|
| MGN12H 滑块 × 滑块座 | H7/g6 | 滑块座孔 +0.02/+0.04 | 手推顺滑、无侧向晃动 |
| 20T 带轮 × 电机轴 (D型 5mm) | 过盈 0.02 | 带轮孔 4.98mm | 螺钉锁死、同心度 < 0.03mm |
| 电磁铁线圈 × 固定座 | H7/h6 | 线圈外径 -0.05 | 热装/冷压入，防旋转销定位 |
| 锥形塞 × 喉部锥面 | 锥度配合 | 30° ±0.2° | 镜面抛光 Ra<0.4，配研色标 > 80% |
| 硅胶管 × 快拆接头 | 过盈 0.3mm | 管内径 4.0, 接头 4.3 | 扎带二次锁紧 |
| PCB × 固定座 (M3 沉头) | 间隙 0.2 | 孔径 3.2mm | 防短路绝缘垫片 0.5mm |

---

## 星火计划 Logo 规范

- **来源**：嘉立创官方提供矢量图 → Fusion 360 导入 SVG → 拉伸 0.5mm 做浮雕
- **位置**：
  1. PCB 丝印层（Top/Bottom 均印，白油/黑油对比）
  2. 机身正面面板浮雕（高 0.5mm，侧向打光效果佳）
  3. 料仓侧面浮雕
  4. 防尘外壳顶面浮雕
- **尺寸**：长 20mm × 高 8mm（保持比例缩放）
- **颜色**：实体打印件建议双色打印（底色 + Logo 色），或贴纸/丝印后处理

---

## 文件命名规范

```
<模块>_<子模块>_<件号>_<版本>.<ext>

例：
CoreXY_XAxis_SliderSeat_v0.3.step
SandHead_V3_NozzleCone_v1.0.stl
Frame_2020_Beam_300mm_v1.0.dwg
```

---

## 协作约定

- **Fusion 360 协作**：团队共享项目，各模块建立「组件」→「启用版本控制」
- **主分支保护**：`Main.f3d` 仅结构组长合并，PR 需附「干涉检查报告」截图
- **外发加工（SLA/CNC）**：导出 STEP，附 `fabrication_notes.md`（材料、公差、表面处理、数量）
- **文档同步**：关键结构变更需同步更新 `docs/DESIGN.md` 对应章节

---

> **Design for manufacturing, print for performance.**  
> —— 每个倒扣、每个倒角、每个螺纹孔，都是为了一次装配成功。  
> —— RainCatStar