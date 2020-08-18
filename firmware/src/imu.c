#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "imu.h"
#include "lsm6ds3.h"
#include "lsm6ds3_registers.h"


void queryIMU(void * imuRawQueueHandle) {
	#ifdef TEST
	{
	#else
	for ( ;; ) {
	#endif
		#ifdef DEBUG
			const int imuTimeoutMs = 10000;
		#else
			const int imuTimeoutMs = 20;
		#endif

		xTaskNotifyWait(0, 0, NULL, pdMS_TO_TICKS(imuTimeoutMs));

		uint8_t rxBuffer[12 + 1];

		startRegistersRead(LSM6DS3_OUTX_L_G, 12, rxBuffer);

		while(imuCommunicationInProgress()) {
			#ifndef TEST
				taskYIELD();
			#endif
		}

		int16_t accelX = rxBuffer[7] | rxBuffer[8] << 8;
		int16_t accelY = rxBuffer[9] | rxBuffer[10] << 8;
		int16_t accelZ = rxBuffer[11] | rxBuffer[12] << 8;

		// Default acceleration limits are -2g/+2g, so 1g is max_int16 / 2
		float xFloat = accelX / 16384.0f;
		float yFloat = accelY / 16384.0f;
		float zFloat = accelZ / 16384.0f;

		int16_t gyroX = rxBuffer[1] | rxBuffer[2] << 8;
		int16_t gyroY = rxBuffer[3] | rxBuffer[4] << 8;
		int16_t gyroZ = rxBuffer[5] | rxBuffer[6] << 8;

		struct ImuRaw dataToSend = {
			{ accelX, accelY, accelZ },
			{ gyroX, gyroY, gyroZ },
			xTaskGetTickCount()
		};

		xQueueOverwrite((QueueHandle_t)imuRawQueueHandle, &dataToSend);
	}
}
