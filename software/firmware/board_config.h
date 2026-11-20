#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

// ==================== 主控与框架 ====================
#define USE_ARDUINO_FRAMEWORK 1
#define USE_FREERTOS 0
#define SERIAL_BAUD 115200

// ==================== ESP32-S3 GPIO 分配（对应 hardware/pinout.md） ====================
// TMC2209 UART（CoreXY 双轴）
#define PIN_TMC_X_TX  GPIO_NUM_4
#define PIN_TMC_X_RX  GPIO_NUM_5
#define PIN_TMC_Y_TX  GPIO_NUM_6
#define PIN_TMC_Y_RX  GPIO_NUM_7
#define PIN_STEPPER_EN GPIO_NUM_8

// 称重模块 HX711
#define PIN_HX711_DOUT GPIO_NUM_12
#define PIN_HX711_SCK  GPIO_NUM_13

// 阀门与震动
#define PIN_SOLENOID_PWM GPIO_NUM_14   // 12V 电磁阀，NMOS 低边驱动
#define PIN_VIB_MOTOR_PWM GPIO_NUM_15  // 震动马达 PWM 调速

// 限位与安全
#define PIN_LIMIT_X GPIO_NUM_16
#define PIN_LIMIT_Y GPIO_NUM_17
#define PIN_IR_JAM GPIO_NUM_18         // 红外对管堵料检测
#define PIN_ESTOP GPIO_NUM_19          // 急停按钮

// ==================== 运动学常数（256×256 mm 工作区） ====================
#define STEPS_PER_MM_X 80.0f   // 待实测标定（20T 带轮，2GT，1/16 细分）
#define STEPS_PER_MM_Y 80.0f
#define MAX_SPEED_MM_S 300.0f
#define MAX_ACCEL_MM_S2 1500.0f

// ==================== 流量标定（仿照 3D 打印机动态校准） ====================
#define CALIB_PWM_POINTS 4           // {20%, 40%, 60%, 80%}
#define CALIB_SAMPLE_MS 5000         // 每点出沙 5 秒
#define HX711_SAMPLE_HZ 80           // 采样率
#define CALIB_POLY_ORDER 3           // 三次多项式拟合 flow = f(PWM)

// ==================== 阀门与震动时序 ====================
#define VIB_PRE_SHAKE_MS 100         // 关阀前 100 ms 震动满功率
#define VIB_POST_SHAKE_MS 5          // 阀门关闭后恢复低功率等待时间
#define SOLENOID_PWM_FREQ 1000       // 电磁阀驱动 PWM 频率（Hz）

// ==================== WebServer ====================
#define WEBSERVER_PORT 80

#endif // BOARD_CONFIG_H