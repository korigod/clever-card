#include <stdio.h>
#include <stdlib.h>

#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "imu.h"
#include "gpio_irq.h"
#include "timer.h"
#include "leds.h"


#define QUERY_IMU_PRIORITY 1
#define UPDATE_LEDS_PRIORITY 1


static StaticQueue_t imuRawQueueDataStructure;
static uint8_t imuRawQueueStorage[IMU_RAW_QUEUE_LENGTH * IMU_RAW_SIZE];


static StaticTask_t queryIMUTaskControlBlock;
static StackType_t queryIMUTaskStack[QUERY_IMU_STACK_SIZE];

static StaticTask_t updateLedsTaskControlBlock;
static StackType_t updateLedsTaskStack[UPDATE_LEDS_STACK_SIZE];


int main(void)
{
	CHIP_Init();

	initMicrosecondsTimer();

	CMU_ClockEnable(cmuClock_GPIO, true);

	QueueHandle_t imuRawQueue = xQueueCreateStatic(IMU_RAW_QUEUE_LENGTH,
	                                               IMU_RAW_SIZE,
	                                               imuRawQueueStorage,
	                                               &imuRawQueueDataStructure);
	configASSERT(imuRawQueue);

	TaskHandle_t queryIMUHandle = xTaskCreateStatic(queryIMU,
	                                                (const char *)"queryIMU",
	                                                QUERY_IMU_STACK_SIZE,
	                                                NULL,
	                                                QUERY_IMU_PRIORITY,
	                                                queryIMUTaskStack,
	                                                &queryIMUTaskControlBlock);

	TaskHandle_t updateLedsHandle = xTaskCreateStatic(updateLeds,
	                                                  (const char *)"updateLeds",
	                                                  UPDATE_LEDS_STACK_SIZE,
	                                                  NULL,
	                                                  UPDATE_LEDS_PRIORITY,
	                                                  updateLedsTaskStack,
	                                                  &updateLedsTaskControlBlock);

	enableLSM6DS3Interrupt(queryIMUHandle);

	initIMU(imuRawQueue);

	vTaskStartScheduler();

	while (1) {
	}
}
