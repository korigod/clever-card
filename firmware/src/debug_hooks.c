#include "FreeRTOS.h"
#include "task.h"

#define TASK_STATUSES_ARRAY_SIZE 8

TaskStatus_t taskStatuses[TASK_STATUSES_ARRAY_SIZE];

void vApplicationIdleHook(void) {
	uxTaskGetSystemState(taskStatuses, TASK_STATUSES_ARRAY_SIZE, NULL);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName) {
	configASSERT(0);
}

void vApplicationMallocFailedHook(void) {
	configASSERT(0);
}
