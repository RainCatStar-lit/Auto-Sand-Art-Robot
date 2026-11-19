#include "scheduler.h"
#include "board_config.h"

Scheduler& Scheduler::getInstance() {
    static Scheduler instance;
    return instance;
}

Scheduler& sched = Scheduler::getInstance();

void Scheduler::addTask(const char* name, TaskFunc func, unsigned long interval_ms, bool enabled) {
    if (taskCount >= 16) return;
    tasks[taskCount] = {name, func, interval_ms, millis(), enabled};
    taskCount++;
}

void Scheduler::loop() {
    unsigned long now = millis();
    for (int i = 0; i < taskCount; i++) {
        if (!tasks[i].enabled) continue;
        if (tasks[i].interval_ms == 0) continue; // 事件驱动任务，不在 loop 中自动触发
        if (now - tasks[i].last_run_ms >= tasks[i].interval_ms) {
            tasks[i].last_run_ms = now;
            if (tasks[i].func) tasks[i].func();
        }
    }
}

void Scheduler::trigger(const char* name) {
    ScheduledTask* t = findTask(name);
    if (t && t->func) t->func();
}

void Scheduler::setEnabled(const char* name, bool enabled) {
    ScheduledTask* t = findTask(name);
    if (t) t->enabled = enabled;
}

Scheduler::ScheduledTask* Scheduler::findTask(const char* name) {
    for (int i = 0; i < taskCount; i++) {
        if (strcmp(tasks[i].name, name) == 0) return &tasks[i];
    }
    return nullptr;
}