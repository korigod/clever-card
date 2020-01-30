#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "leds.h"


void ledControlTask(void * pvParameters) {
	TickType_t previousWakeTime = xTaskGetTickCount();
	while (1) {
		for (int i = 0; i < 98; i++) {
			vTaskDelayUntil(&previousWakeTime, pdMS_TO_TICKS(1000 / 2));
			clearLeds();
			ledOutputs[i] = 5;
			latchLedOutputs();
		}
	}
}
