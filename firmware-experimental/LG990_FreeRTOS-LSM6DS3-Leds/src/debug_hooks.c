#include "FreeRTOS.h"
#include "task.h"

TaskStatus_t taskStatuses[10];

void vApplicationIdleHook(void) {
	uxTaskGetSystemState(taskStatuses, 10, NULL);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName) {
	configASSERT(0);
}

void vApplicationMallocFailedHook(void) {
	configASSERT(0);
}
