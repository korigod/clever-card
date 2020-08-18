#ifndef IMU_H_INC
#define IMU_H_INC

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"

#define LSM6DS3_SPI_BAUDRATE 1000000
#define SPI_RX_DMA_CHANNEL 0
#define SPI_TX_DMA_CHANNEL 1

#define IMU_RAW_QUEUE_LENGTH 1
#define IMU_RAW_SIZE 4  // sizeof(struct ImuRaw) == 16
struct ImuRaw {
	int16_t acceleration[3];
	int16_t angularRate[3];
	TickType_t timestamp;
};

#define QUERY_IMU_STACK_SIZE 80
void queryIMU(void * imuRawQueueHandle);

#endif /* IMU_H_INC */
