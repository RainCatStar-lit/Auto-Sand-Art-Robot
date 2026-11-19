/**
 * Auto-Sand-Art-Robot ESP32-S3 固件（Arduino 框架）
 * 基于 millis() 轻量级多任务调度 + 状态机，不使用 FreeRTOS
 *
 * 模块划分：
 *   Motion    - CoreXY 运动学、步进脉冲生成、TMC2209 UART 配置
 *   FlowCtrl  - PWM 流量控制、HX711 称重读取、标定状态机
 *   Valve     - 12V 电磁阀开合、关阀前 100 ms 震动破拱时序
 *   SensorMon - IR 堵料检测、电压、温度监控
 *   WebEvent  - WebServer HTTP 接收 G-code、状态上报、急停
 *   Calib     - HX711 多点 PWM 流量标定、Flash/NVS 写入
 *   DiagLog   - 健康度上报、看门狗喂狗、错误码上报
 */

#include <Arduino.h>
#include "board_config.h"
#include "scheduler.h"

// ---------- 模块前置声明（后续分文件实现） ----------
void taskMotion();
void taskFlowCtrl();
void taskValve();
void taskSensorMon();
void taskWebEvent();
void taskCalib();
void taskDiagLog();

// ---------- 系统状态 ----------
enum class SysState {
    IDLE,
    DRAWING,
    CALIBRATING,
    ESTOP
};
SysState g_sysState = SysState::IDLE;

// ---------- setup ----------
void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(500);
    Serial.println(F("[Boot] Auto-Sand-Art-Robot ESP32-S3 Firmware"));
    Serial.println(F("[Boot] Framework: Arduino + millis() scheduler (no FreeRTOS)"));

    // GPIO 初始化（以 board_config.h 为准）
    pinMode(PIN_LIMIT_X, INPUT_PULLUP);
    pinMode(PIN_LIMIT_Y, INPUT_PULLUP);
    pinMode(PIN_IR_JAM, INPUT);
    pinMode(PIN_ESTOP, INPUT_PULLUP);
    pinMode(PIN_SOLENOID_PWM, OUTPUT);
    pinMode(PIN_VIB_MOTOR_PWM, OUTPUT);
    digitalWrite(PIN_SOLENOID_PWM, LOW);
    digitalWrite(PIN_VIB_MOTOR_PWM, LOW);

    // 注册定时任务（interval_ms）
    sched.addTask("Motion",    taskMotion,    1);   // 1 ms 步进脉冲
    sched.addTask("FlowCtrl",  taskFlowCtrl,  5);   // 5 ms 流量与称重
    sched.addTask("SensorMon", taskSensorMon, 100); // 100 ms 环境监测
    sched.addTask("DiagLog",   taskDiagLog,   1000);// 1 s 诊断上报

    // 注册事件驱动任务（interval_ms = 0，由 trigger() 手动调用）
    sched.addTask("Valve",     taskValve,     0);
    sched.addTask("WebEvent",  taskWebEvent,  0);
    sched.addTask("Calib",     taskCalib,     0);

    // 初始化 WebServer（HTTP 接收 G-code，后续升级 WebSocket 不改硬件）
    // TODO: 在 taskWebEvent 内部实现 WebServer 启动与路由注册

    Serial.println(F("[Boot] Scheduler ready. Entering main loop."));
}

// ---------- loop ----------
void loop() {
    sched.loop();

    // 急停检查（最高优先级，每轮轮询）
    if (digitalRead(PIN_ESTOP) == LOW) {
        g_sysState = SysState::ESTOP;
        sched.trigger("Valve");    // 立即关阀
        Serial.println(F("[ESTOP] 急停触发，阀门闭锁"));
        while (digitalRead(PIN_ESTOP) == LOW) {
            delay(10); // 等待急停释放
        }
        g_sysState = SysState::IDLE;
        Serial.println(F("[ESTOP] 急停已释放，回到空闲"));
    }

    // 看门狗喂狗（DiagLog 任务内也会处理，这里可额外补充）
    // ESP32 看门狗可在 taskDiagLog 中喂狗
}

// ---------- 占位任务实现（后续细化） ----------
void taskMotion() {
    // CoreXY 插补、TMC2209 UART 配置、步进脉冲生成
    // 每 1 ms 执行一次，产生步进脉冲
}

void taskFlowCtrl() {
    // PWM 占空比更新、HX711 读取、标定状态机
}

void taskValve() {
    // 12V 电磁阀开合、关阀前 100 ms 震动破拱时序
}

void taskSensorMon() {
    // IR 堵料检测、电压、温度监控
}

void taskWebEvent() {
    // WebServer HTTP 接收 G-code、状态上报、急停
}

void taskCalib() {
    // HX711 多点 PWM 流量标定、Flash/NVS 写入
}

void taskDiagLog() {
    // 健康度上报、看门狗喂狗、错误码上报
}