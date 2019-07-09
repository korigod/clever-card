#include <stdint.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"

#define ULONG_MAX 0xFFFFFFFFUL


#define SPI_RX_DMA_CHANNEL 0
#define SPI_TX_DMA_CHANNEL 1
#define SPI_BUFFER_SIZE 8
extern uint8_t TxBuffer[SPI_BUFFER_SIZE];
uint8_t RxBuffer[SPI_BUFFER_SIZE];


#define IMU_RAW_QUEUE_LENGTH 1
#define IMU_RAW_SIZE 16  // sizeof(struct ImuRaw) == 16
static StaticQueue_t _imu_raw_queue_service_data;
static uint8_t _imu_raw_queue_items[IMU_RAW_QUEUE_LENGTH * IMU_RAW_SIZE];
struct ImuRaw {
	int16_t acceleration[3];
	int16_t angular_vel[3];
	TickType_t timestamp;
};


#define QUERY_IMU_STACK_SIZE configMINIMAL_STACK_SIZE
static StaticTask_t _query_imu_buffer;
static StackType_t _query_imu_stack[QUERY_IMU_STACK_SIZE];
void queryIMU(void * pvParameters);


void initIMU();
