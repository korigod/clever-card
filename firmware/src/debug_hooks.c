#include "FreeRTOS.h"
#include "task.h"

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName) {
	configASSERT(0);
}

void vApplicationMallocFailedHook(void) {
	configASSERT(0);
}
