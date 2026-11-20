# Models 目录 — 3D 打印件 BOM 规划

> Fusion 360 参数化设计源文件、STL 导出、切片配置、打印件清单。  
> 所有打印件由 Bambu Lab A1 完成；机械设计定稿详见 `docs/DESIGN.md`。

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
│   │   ├── Squeegee/       # 刮板机构（悬臂、SG90 底座、TPU 刮片压板）
│   │   └── Enclosure/      # 一体化防尘外壳、Logo 面板
│   └── drawings/           # 2D 工程图（.pdf/.dwg）
│       ├── Frame_CutList.pdf
│       ├── CoreXY_Assembly.pdf
│       └── SandHead_Detail.pdf
│
├── stl/                    # 切片就绪 STL（按版本标签归档）
│   ├── v0.1_M1/            # Phase 1 机械验证版
│   ├── v0.2_M2/            # Phase 2 电气集成版
│   ├── v0.3_M3/            # Phase 3 出沙头 V3 定型版
│   └── v1.0_Final/         # 最终交付版
│
├── slicer/                 # 切片器配置与项目文件
│   ├── bambu_studio/       # Bambu Studio 项目文件
│   │   ├── SSAP_ASR_v1.0.3mf
│   │   └── filament_profiles/
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

## A1 打印件 BOM（需由 Bambu Lab A1 完成）

以下清单为机械设计规格书 (`docs/DESIGN.md`) 中各子系统对应的打印件，按模块归组，优先在 Phase 1~3 批量打印。

| 件号 | 名称 | 数量 | 材料 | 备用位置 | 说明 |
|------|------|------|------|----------|------|
| `SH-MAIN-01` | `Toolhead_Main_Body` (出沙头主壳体) | 1 | PETG-CF | 工具头 | 含锥形阀腔与侧面进沙口，电磁铁固定座，O 型圈压槽，红外对管槽 |
| `SH-SPC-02` | `Spool_Spacer` (软管限位环) | 2 | PETG-CF | 料盒/工具头软管沿线 | 限位食品级硅胶软管 φ4-5 mm，配合弹簧张力管理器 |
| `SQ-HLD-01` | `Squeegee_Holder` (刮板支架) | 1 | PETG-CF | 龙门横梁后方 | 独立挂载，安装 SG90 舵机与 TPU 柔性刮片悬臂 |
| `GT-BKT-01` | `Gantry_Bracket` (龙门架连接件) | 4 | PLA-CF | 龙门横梁四角 | 2020 铝型材与 MGN12H 滑块座刚性连接，M3 热熔铜螺母安装孔 |

> BOM 将随 Fusion 360 参数化设计冻结后，由 Fusion API 自动导出为 `bom/print_bom_v1.0.csv`（件号、名称、数量、材料、填充、预估重量、打印时间）。

---

## 打印工艺规范（Bambu Lab A1 专用）

| 项目 | PLA-CF (结构件) | PETG-CF (受力/耐热件) | TPU (软性密封/刮片) |
|------|-----------------|----------------------|------------------|
| 适用件号 | `GT-BKT-01` | `SH-MAIN-01`, `SH-SPC-02`, `SQ-HLD-01` | 刮片、减震垫、O 型圈压板 |
| 层高 | 0.20 mm | 0.20 mm | 0.25 mm |
| 填充率 | 15% 陀螺 | 20% 陀螺 | 30% 网格 |
| 壁厚 | 3 层 (0.6 mm) | 4 层 (0.8 mm) | 2 层 |
| 底/顶层 | 4/6 层 | 5/7 层 | 3/4 层 |
| 打印温度 | 250°C | 260°C | 230°C |
| 热床温度 | 50°C | 70°C | 40°C |
| 打印速度 | 外壁 150 mm/s | 外壁 120 mm/s | 80 mm/s |
| 回抽 | 0.8 mm / 40 mm/s | 1.0 mm / 35 mm/s | 0.5 mm / 20 mm/s |
| 干燥 | 60°C 8h (必烘) | 65°C 8h (必烘) | 50°C 6h |

**支撑策略**：
- 仅对悬垂 > 45° 且无法通过桥接解决的区域启用树形支撑；接触面 0.15 mm，易剥离。
- 关键配合面（滑块座、电磁铁座、传感器座、锥形阀腔）**禁用支撑**，设计自支撑倒扣结构，确保配合面质量。

---

## 公差要求

| 配合对 | 公差要求 | 实施方式 | 备注 |
|--------|----------|----------|------|
| 导轨安装孔（MGN12H 滑块座） | M3 热熔铜螺母 | 打印件预留 M3 螺母座孔，热熔压入 M3 铜螺母 | 滑块座孔 H7/g6，手推顺滑、无侧向晃动 |
| 锥形塞 × 倒圆锥喉部 | 锥度配合 | 半锥角 30°～60°，±0.2° | 镜面抛光 Ra<0.4，配研色标 > 80% |
| 电磁铁线圈 × 固定座 | H7/h6 | 线圈外径 -0.05 | 热装/冷压入，防旋转销定位 |
| 20T 带轮 × 电机轴 (D 型 5 mm) | 过盈 0.02 | 带轮孔 4.98 mm | 螺钉锁死，同心度 < 0.03 mm |
| 硅胶管 × 快拆接头 | 过盈 0.3 mm | 管内径 4.0 mm，接头 4.3 mm | 扎带二次锁紧 |
| PCB × 固定座 (M3 沉头) | 间隙 0.2 mm | 孔径 3.2 mm | 防短路绝缘垫片 0.5 mm |

> 导轨安装孔必须使用 **M3 热熔铜螺母**，严禁直接自攻塑料螺纹，以保证 MGN12H 导轨反复拆装后的螺纹可靠性与导轨安装精度。

---

## 版本发布流程

1. Fusion 360 中完成设计冻结 → 创建版本标签 `vX.Y_MZ`
2. 导出 STEP + STL → 放入 `stl/vX.Y_MZ/`，文件名规范：`<模块>_<子模块>_<件号>_<版本>.stl`
3. 运行 BOM 自动导出脚本（Fusion API）→ 更新 `bom/print_bom_vX.Y.csv`
4. Bambu Studio 导入 STL、套用工艺包、切片验证 → 保存 `.3mf` 到 `slicer/bambu_studio/`
5. 渲染关键视图 → 存入 `renders/`
6. Git 提交：
   ```bash
   git add models/stl/vX.Y_MZ models/bom/print_bom_vX.Y.csv models/slicer/...
   git commit -m "chore(models): release vX.Y_MZ for Phase Z"
   git tag -a "hw/vX.Y_MZ" -m "Hardware freeze for Milestone Z"
   ```

---

> Design for manufacturing, print for performance.  
> —— RainCatStar