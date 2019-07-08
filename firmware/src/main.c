#include <stdio.h>
#include <stdlib.h>

#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "imu.h"


#define QUERY_IMU_PRIORITY 1


int main(void)
{
	CHIP_Init();

	QueueHandle_t imuRawQueue = xQueueCreateStatic(IMU_RAW_QUEUE_LENGTH,
	                                               IMU_RAW_SIZE,
	                                               _imu_raw_queue_items,
	                                               &_imu_raw_queue_service_data);
	configASSERT(imuRawQueue);

	TaskHandle_t queryIMUHandle = xTaskCreateStatic(queryIMU,
	                                                (const char *)"queryIMU",
	                                                QUERY_IMU_STACK_SIZE,
	                                                imuRawQueue,
	                                                QUERY_IMU_PRIORITY,
	                                                _query_imu_stack,
	                                                &_query_imu_buffer);

	initIMU(imuRawQueue);

	vTaskStartScheduler();

	while (1) {
	}
}
