#ifndef TASKS_H
#define TASKS_H

#include "FreeRTOS.h"
#include "task.h"

// Protótipos das tarefas
void TaskWaitForReaction(void *params);
void TaskButtonMonitor(void *params);
void TaskDisplay(void *params);

#endif