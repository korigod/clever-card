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
#include "gpio_irq.h"


#define QUERY_IMU_PRIORITY 1


int main(void)
{
	CHIP_Init();

	CMU_ClockEnable(cmuClock_GPIO, true);

	QueueHandle_t imuRawQueue = xQueueCreateStatic(IMU_RAW_QUEUE_LENGTH,
	                                               IMU_RAW_SIZE,
	                                               _imu_raw_queue_items,
	                                               &_imu_raw_queue_service_data);
	configASSERT(imuRawQueue);

	enableLSM6DS3Interrupt();

	initIMU(imuRawQueue);

	vTaskStartScheduler();

	while (1) {
	}
}
