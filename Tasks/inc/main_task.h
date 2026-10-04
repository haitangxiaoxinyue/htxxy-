#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include <stdint.h>

extern volatile uint32_t tick;

void TaskInit (void);
#endif