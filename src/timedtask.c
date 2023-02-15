#include <stdbool.h>

typedef void (*timedtaskFuncPointer)(void);

typedef struct 
{
    unsigned long int beginTime;
    unsigned long int Interval;
    bool enabled;
    timedtaskFuncPointer func;
} timedtask;

timedtask timedtask_create_task(unsigned long int interval, timedtaskFuncPointer finishTask){
    timedtask task;
    task.beginTime = millis();
    if(interval < 100)
        interval = 100;
    task.Interval = interval;
    task.enabled = false;
    task.func = finishTask; 
    return task;
}

void timedtask_enable_task(timedtask *task){
    task->enabled = true;
    task->beginTime = millis();
    return;
}

void timedtask_disable_task(timedtask *task){
    task->enabled = false;
    return;
}

void timedtask_check_task(timedtask *task){
    if(!task->enabled) return;
    if((task->beginTime + task->Interval) <= millis()){
        (*(task->func))(); // execute task function
        task->beginTime = millis();
    }
    return;
}

void timedtask_force_run_task(timedtask *task){
    (*(task->func))(); // execute task function
    task->beginTime = millis();
    return;
}

void timedtask_reset_task(timedtask *task){
    task->beginTime = millis();
    return;
}