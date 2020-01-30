#include "em_chip.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "timer.h"
#include "leds.h"
#include "led_control.h"

#define LED_CONTROL_PRIORITY 1
#define LED_CONTROL_STACK_SIZE configMINIMAL_STACK_SIZE

static StaticTask_t ledControlTaskControlBlock;
static StackType_t ledControlTaskStack[LED_CONTROL_STACK_SIZE];


int main(void)
{
	CHIP_Init();

	initializeLeds();

	enableAndTriggerTimerInterrupt(switchToNextLed);

	TaskHandle_t ledControlTaskHandle = xTaskCreateStatic(
		ledControlTask,
		(const char *)"ledControl",
		LED_CONTROL_STACK_SIZE,
		NULL,
		LED_CONTROL_PRIORITY,
		ledControlTaskStack,
		&ledControlTaskControlBlock
	);

	vTaskStartScheduler();

	while (1) {
	}
}
