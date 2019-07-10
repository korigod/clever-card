#include <stdint.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"


#define LSM6DS3_SPI_BAUDRATE 1000000
#define SPI_RX_DMA_CHANNEL 0
#define SPI_TX_DMA_CHANNEL 1
#define SPI_BUFFER_SIZE 14
extern uint8_t TxBuffer[SPI_BUFFER_SIZE];
uint8_t RxBuffer[SPI_BUFFER_SIZE];


#define IMU_RAW_QUEUE_LENGTH 1
#define IMU_RAW_SIZE 16  // sizeof(struct ImuRaw) == 16
static StaticQueue_t imuRawQueueDataStructure;
static uint8_t imuRawQueueStorage[IMU_RAW_QUEUE_LENGTH * IMU_RAW_SIZE];
struct ImuRaw {
	int16_t acceleration[3];
	int16_t angularVel[3];
	TickType_t timestamp;
};


#define QUERY_IMU_STACK_SIZE configMINIMAL_STACK_SIZE
static StaticTask_t queryIMUTaskControlBlock;
static StackType_t queryIMUTaskStack[QUERY_IMU_STACK_SIZE];
void queryIMU(void * pvParameters);


void initIMU();
