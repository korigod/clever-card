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


#define QUERY_IMU_PRIORITY 1


int main(void)
{
	CHIP_Init();

	initMicrosecondsDelayTimer();
	initDebugTimer();

	CMU_ClockEnable(cmuClock_GPIO, true);

	QueueHandle_t imuRawQueue = xQueueCreateStatic(IMU_RAW_QUEUE_LENGTH,
	                                               IMU_RAW_SIZE,
	                                               _imu_raw_queue_items,
	                                               &_imu_raw_queue_service_data);
	configASSERT(imuRawQueue);

	TaskHandle_t queryIMUHandle = xTaskCreateStatic(queryIMU,
	                                                (const char *)"queryIMU",
	                                                QUERY_IMU_STACK_SIZE,
	                                                NULL,
	                                                QUERY_IMU_PRIORITY,
	                                                _query_imu_stack,
	                                                &_query_imu_buffer);

	enableLSM6DS3Interrupt(queryIMUHandle);

	initIMU(imuRawQueue);

	vTaskStartScheduler();

	while (1) {
	}
}
