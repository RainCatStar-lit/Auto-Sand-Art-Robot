#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>

/**
 * 轻量级多任务调度器（基于 millis()，非阻塞）
 * 每个模块注册自己的处理函数和执行间隔，主循环到点执行。
 * 不使用 FreeRTOS，适合 Arduino 框架下的 ESP32-S3。
 */

typedef void (*TaskFunc)(void);

struct ScheduledTask {
    const char* name;
    TaskFunc func;
    unsigned long interval_ms;   // 0 表示事件驱动，仅在调用 run() 时由外部触发
    unsigned long last_run_ms;
    bool enabled;
};

class Scheduler {
public:
    static Scheduler& getInstance();

    // 注册任务
    void addTask(const char* name, TaskFunc func, unsigned long interval_ms, bool enabled = true);

    // 主循环中调用，检查并执行到点的任务
    void loop();

    // 立即触发一次任务（用于事件驱动任务，如急停、标定触发）
    void trigger(const char* name);

    // 启用/禁用任务
    void setEnabled(const char* name, bool enabled);

private:
    Scheduler() {}
    ScheduledTask tasks[16];
    int taskCount = 0;

    ScheduledTask* findTask(const char* name);
};

extern Scheduler& sched;

#endif // SCHEDULER_H