#include "em_chip.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "common.h"
#include "timer.h"
#include "leds.h"
#include "led_control.h"
#include "imu.h"
#include "lsm6ds3.h"
#include "gpio_irq.h"

#define LED_CONTROL_PRIORITY 1
#define LED_CONTROL_STACK_SIZE 128

#define QUERY_IMU_PRIORITY 1


static StaticQueue_t imuRawQueueDataStructure;
static uint8_t imuRawQueueStorage[IMU_RAW_QUEUE_LENGTH * IMU_RAW_SIZE];


static StaticTask_t queryIMUTaskControlBlock;
static StackType_t queryIMUTaskStack[QUERY_IMU_STACK_SIZE];

static StaticTask_t ledControlTaskControlBlock;
static StackType_t ledControlTaskStack[LED_CONTROL_STACK_SIZE];


int main(void)
{
	CHIP_Init();

	QueueHandle_t imuRawQueue = xQueueCreateStatic(
		IMU_RAW_QUEUE_LENGTH,
		IMU_RAW_SIZE,
		imuRawQueueStorage,
		&imuRawQueueDataStructure
	);

	TaskHandle_t ledControlTaskHandle = xTaskCreateStatic(
		ledControlTask,
		(const char *)"ledControl",
		LED_CONTROL_STACK_SIZE,
		imuRawQueue,
		LED_CONTROL_PRIORITY,
		ledControlTaskStack,
		&ledControlTaskControlBlock
	);

	TaskHandle_t queryIMUTaskHandle = xTaskCreateStatic(
		queryIMU,
		(const char *)"queryIMU",
		QUERY_IMU_STACK_SIZE,
		imuRawQueue,
		QUERY_IMU_PRIORITY,
		queryIMUTaskStack,
		&queryIMUTaskControlBlock
	);

	initializeLeds();

	enableLSM6DS3Interrupt(queryIMUTaskHandle);

	initLSM6DS3(LSM6DS3_SPI_BAUDRATE);

	enableAndTriggerTimerInterrupt(switchToNextLed);

	vTaskStartScheduler();

	while (1) {
	}
}
