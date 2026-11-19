# Software 目录

> 固件、上位机图像处理、Web 前端 —— 全栈代码仓。

---

## 目录结构

```
software/
├── firmware/               # ESP32-S3 固件
│   ├── platformio.ini     # PlatformIO 配置（环境、依赖锁定）
│   ├── src/
│   │   ├── main.cpp       # 入口、FreeRTOS 任务创建
│   │   ├── board_config.h # 引脚映射、硬件版本宏
│   │   ├── motion/
│   │   │   ├── planner.cpp/h     # G-code 解析、S曲线规划、软限位
│   │   │   ├── stepper_tmc.cpp/h # TMC2209 UART 控制、传感器less回零
│   │   │   └── kinematics.cpp/h  # CoreXY 正/逆运动学
│   │   ├── flow/
│   │   │   ├── flow_ctrl.cpp/h   # PWM 占空比更新、查找表插值
│   │   │   ├── vib_seq.cpp/h     # 震动破拱时序状态机
│   │   │   └── jam_protect.cpp/h # 红外堵料检测、急停逻辑
│   │   ├── sensor/
│   │   │   ├── hx711.cpp/h       # 24bit ADC 读取、滑窗滤波、温漂补偿
│   │   │   └── calib_mgr.cpp/h   # 自动标定状态机、多项式拟合、Flash存储
│   │   ├── comm/
│   │   │   ├── web_server.cpp/h  # WebSocket JSON-RPC、HTTP OTA、LittleFS
│   │   │   └── protocol.h        # 指令码、错误码、心跳定义
│   │   ├── storage/
│   │   │   └── littlefs_wrap.cpp/h # 配置/日志/预置图案读写
│   │   └── diag/
│   │       └── monitor.cpp/h     # 电压/电流/温度/看门狗/错误上报
│   ├── include/           # 公共头文件、第三方库配置
│   ├── lib/               # 本地依赖库（如 JSON-RPC 实现）
│   ├── test/              # 单元测试（Unity 框架，Native 运行）
│   ├── data/              # LittleFS 镜像源文件（默认配置、校准系数模板）
│   └── build/             # 编译产物（.bin, .map, partitions.csv）
│
├── img2gcode/             # Python 图像处理 → G-code 管线
│   ├── requirements.txt   # 锁定版本依赖
│   ├── pyproject.toml     # 构建配置
│   ├── src/
│   │   ├── __main__.py    # CLI 入口：img2gcode -i in.jpg -o out.gcode
│   │   ├── pipeline/
│   │   │   ├── preprocess.py   # 灰度、高斯、自适应阈值、去噪
│   │   │   ├── skeletonize.py  # cv2.ximgproc.thinning 骨架提取
│   │   │   ├── vectorize.py    # Douglas-Peucker 简化、最小线段过滤
│   │   │   ├── optimize.py     # 贪心最近邻 + 2-opt 路径优化
│   │   │   └── gcode_gen.py    # G0/G1/M106/M107 生成、流量映射
│   │   ├── utils/
│   │   │   ├── img_io.py       # 统一读写、EXIF 旋转修正
│   │   │   └── preview.py      # matplotlib/plotly 预览调试
│   │   └── config.py           # 参数集中管理（分辨率、容差、流量分级）
│   ├── tests/             # pytest 单测 + 视觉回归测试图集
│   ├── test_images/       # 100 张基准测试图（线稿/照片/手绘/噪声）
│   └── wasm/              # 可选：Emscripten 编译 WASM 供前端直跑
│
├── web/                   # Vue 3 + TS + Vite 前端
│   ├── package.json
│   ├── pnpm-lock.yaml
│   ├── vite.config.ts
│   ├── tsconfig.json
│   ├── index.html
│   ├── src/
│   │   ├── main.ts
│   │   ├── App.vue
│   │   ├── router/
│   │   │   └── index.ts
│   │   ├── stores/
│   │   │   ├── device.ts       # Pinia：设备状态、实时流量曲线数据
│   │   │   ├── gallery.ts      # 图片列表、预览缓存
│   │   │   └── calib.ts        # 标定进度、系数表格
│   │   ├── components/
│   │   │   ├── Dashboard.vue   # 实时曲线 + 状态灯 + 急停
│   │   │   ├── Gallery.vue     # 拖拽上传、骨架 Canvas 预览、G-code 仿真
│   │   │   ├── Calibration.vue # 一键标定进度条、系数可编辑表格、导出 CSV
│   │   │   ├── Settings.vue    # Wi-Fi 配网、OTA 升级、机器参数
│   │   │   └── common/         # 按钮、输入框、模态框、Toast 等
│   │   ├── composables/
│   │   │   ├── useWebSocket.ts # JSON-RPC 封装、心跳、重连指数退避
│   │   │   ├── useCanvas.ts    # Three.js/Canvas 2D 路径渲染
│   │   │   └── useImg2Gcode.ts # 调用 WASM 或后端 API 生成 G-code
│   │   ├── utils/
│   │   │   ├── protocol.ts     # 指令码、类型定义（与固件 protocol.h 同步）
│   │   │   └── helpers.ts
│   │   ├── assets/
│   │   │   ├── styles/         # 全局 SCSS、变量、混入
│   │   │   └── icons/
│   │   └── views/              # 页面级组件（DashboardView 等）
│   ├── public/                 # 静态资源（favicon、manifest、预置图案）
│   └── dist/                   # 构建产物（可直接托管到 ESP32 LittleFS）
│
├── docs/                    # 软件侧文档
│   ├── firmware_arch.md     # 固件架构、任务调度、内存分布
│   ├── protocol_spec.md     # JSON-RPC 协议完整定义
│   ├── calib_algo.md        # 称重闭环标定算法数学推导
│   ├── img2gcode_algo.md    # 图像处理管线算法细节
│   └── ota_update.md        # OTA 分区表、签名、回滚策略
│
└── scripts/                 # 通用脚本
    ├── flash_firmware.sh    # 一键烧录（固件+LittleFS+分区表）
    ├── monitor_serial.py    # 串口监控、日志解析、自动保存
    ├── gen_calib_csv.py     # 标定数据导出 CSV/绘图
    └── ci/                  # GitHub Actions 工作流
        ├── firmware.yml     # 编译、单测、生成 Release 产物
        ├── img2gcode.yml    # pytest + 视觉回归
        └── web.yml          # lint、typecheck、build、deploy preview
```

---

## 技术栈锁版本（关键依赖）

| 层级 | 组件 | 版本 | 说明 |
|------|------|------|------|
| **固件** | PlatformIO | 6.1.x | 统一构建环境 |
| | ESP-IDF | 5.2.x | ESP32-S3 支持 |
| | FreeRTOS | 内核集成 | 任务优先级、队列、信号量 |
| | TMC2209 lib | 自行维护 | UART 模式、StealthChop、Sensorless |
| | ArduinoJson | 7.x | JSON-RPC 序列化 |
| | LittleFS | ESP-IDF 组件 | 耐磨损文件系统 |
| **Python** | Python | 3.11+ | 类型注解、match-case |
| | OpenCV | 4.9.x | cv2.ximgproc 需 opencv-contrib |
| | NumPy | 1.26.x | 数值计算 |
| | SciPy | 1.12.x | 多项式拟合、插值 |
| | click | 8.x | CLI 框架 |
| | pytest | 7.x | 单测 + 视觉回归 |
| **Web** | Vue | 3.4.x | Composition API + `<script setup>` |
| | TypeScript | 5.4.x | 严格模式 |
| | Vite | 5.x | 极速 HMR |
| | Pinia | 2.1.x | 状态管理 |
| | Vue Router | 4.3.x | SPA 路由 |
| | Socket.io-client | 4.7.x | WebSocket 封装 |
| | Three.js | r160+ | Canvas 3D/2D 渲染 |
| | ECharts | 5.5.x | 实时流量曲线 |
| | pnpm | 9.x | 包管理、monorepo 就绪 |

---

## 固件任务调度表（FreeRTOS）

| 任务名 | 优先级 | 核心 | 周期/触发 | 栈大小 | 关键队列/信号量 |
|--------|--------|------|-----------|--------|-----------------|
| `motion_task` | 20 | 0 | 1 ms 周期 | 8 KB | `gcode_queue` (xQueue), `motion_done` (Semaphore) |
| `flow_task` | 18 | 0 | 5 ms 周期 | 4 KB | `flow_cmd_queue`, `vib_timer` (Timer) |
| `sensor_task` | 15 | 1 | 12.5 ms (80 Hz) | 4 KB | `hx711_data_queue` |
| `web_task` | 10 | 1 | 事件驱动 | 16 KB | `ws_cmd_queue`, `ws_tx_queue` |
| `calib_task` | 8 | 1 | 用户/定时触发 | 8 KB | `calib_state_mutex` |
| `diag_task` | 5 | 1 | 1 s | 4 KB | `health_status_queue` |
| `idle_hook` | 0 | 0/1 | 空闲钩子 | — | 喂看门狗、低功耗管理 |

> **核心亲和性**：实时性要求高的任务绑 Core 0，通讯/存储/诊断绑 Core 1，避免抢占抖动。

---

## 通讯协议：JSON-RPC 2.0 over WebSocket

```json
// 指令示例：设置目标流量
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "flow.set_target",
  "params": { "flow_g_per_s": 0.85 }
}

// 响应
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": { "pwm": 512, "actual_flow": 0.847 }
}

// 推送：实时流量/称重
{
  "jsonrpc": "2.0",
  "method": "sensor.push",
  "params": { "flow": 0.84, "mass_g": 12.3, "timestamp_ms": 1234567 }
}
```

完整协议定义见 `docs/protocol_spec.md`。

---

## 编译 & 烧录流程

```bash
# 1. 固件编译
cd software/firmware
pio run -e release          # 产物在 .pio/build/release/

# 2. 烧录固件 + LittleFS + 分区表
./scripts/flash_firmware.sh /dev/ttyUSB0   # Linux/macOS
./scripts/flash_firmware.ps1 COM3          # Windows

# 3. 串口监控（带时间戳、颜色、自动保存）
python scripts/monitor_serial.py /dev/ttyUSB0 115200
```

---

## Python 管线 CLI

```bash
cd software/img2gcode
pip install -r requirements.txt

# 单张转换
python -m src -i test_images/portrait.jpg -o out.gcode --preview

# 批量回归测试
pytest tests/ -v --visual-regression
```

---

## Web 前端开发 & 部署

```bash
cd software/web
pnpm install

# 开发模式（热重载 + WebSocket 代理到 ESP32）
pnpm dev

# 生产构建（产物在 dist/，可直接上传 ESP32 LittleFS）
pnpm build

# 预览生产构建
pnpm preview
```

**部署到设备**：
1. `pnpm build` 生成 `dist/`
2. 通过 Web「Settings」页面上传 `dist/` 打包后的 `webfs.bin`（LittleFS 镜像）
3. 设备自动解压、校验、原子切换、刷新浏览器

---

## CI/CD（GitHub Actions）

| Workflow | 触发 | 关键 Job |
|----------|------|----------|
| `firmware.yml` | push/main, PR | `build` (PlatformIO) → `test_native` (Unity) → `release_assets` (上传 .bin/.map) |
| `img2gcode.yml` | push/main, PR | `lint` (ruff) → `typecheck` (mypy) → `test` (pytest + visual) |
| `web.yml` | push/main, PR | `lint` (eslint) → `typecheck` (tsc) → `build` → `deploy_preview` (GitHub Pages) |

---

## 版本规范

- **固件**：语义化版本 `vMAJOR.MINOR.PATCH` + Git SHA 短码（如 `v1.2.3-gabc1234`）
- **Python 包**：`pyproject.toml` 版本同步固件 MAJOR.MINOR
- **Web**：`package.json` 版本同步固件 MAJOR.MINOR，构建时注入 `VITE_APP_VERSION`

---

## 调试技巧合集

| 场景 | 工具/命令 |
|------|-----------|
| 固件崩溃定位 | `pio run -e release -t coredump` + `esp_coredump.py info_coredump` |
| 实时任务栈水位 | `diag_task` 定期打印 `uxTaskGetStackHighWaterMark` |
| WebSocket 抓包 | Chrome DevTools Network → WS 帧，或 `websocat` CLI |
| 运动轨迹可视化 | 固件开启 `M111 S1` 调试流，Web 端 Canvas 回放 |
| 称重噪声分析 | `scripts/gen_calib_csv.py --plot` 生成时域/频域图 |
| 图像处理中间结果 | `img2gcode -i in.jpg --save-stages` 保存每步图片 |

---

## 待办 & 技术债（持续更新）

- [ ] 固件：实现 G2/G3 圆弧插补（当前仅 G0/G1）
- [ ] 固件：支持多图案队列、断点续传（掉电恢复）
- [ ] Python：WASM 编译优化体积 < 500 KB gzipped
- [ ] Web：PWA 支持、离线缓存、后台同步
- [ ] 协议：增加二进制模式（CBOR/MessagePack）降低带宽
- [ ] 测试：引入硬件在环（HIL）测试，模拟 HX711/电磁铁/步进电机

---

> **Code is liability, tests are assets.**  
> —— 写每一行代码前，先想好怎么测、怎么回滚、怎么被替换。  
> —— RainCatStar