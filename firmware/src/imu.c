#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_dma.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "imu.h"


#define SPI_BUFFER_SIZE 8

uint8_t TxBuffer[SPI_BUFFER_SIZE] = { 0xA8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t RxBuffer[SPI_BUFFER_SIZE];

DMA_DESCRIPTOR_TypeDef dmaControlBlock[32] __attribute__((aligned(256)));


void rxCompletedCallback(uint32_t channelNum, bool isPrimaryDescriptor, void *queueHandle) {
	// RxBuffer[1] contains the LSB of X axis acceleration reading,
	// and RxBuffer[2] contains the MSB.
	int16_t accel_x = RxBuffer[1] | RxBuffer[2] << 8;
	int16_t accel_y = RxBuffer[3] | RxBuffer[4] << 8;
	int16_t accel_z = RxBuffer[5] | RxBuffer[6] << 8;
	// There should be 0x00 in RxBuffer[7] if the data is read correctly.
	// RxBuffer[0] is junk data and should be discarded.

	// Default acceleration limits are -2g/+2g, so 1g is max_int16 / 2
	float x_float = accel_x / 16384.0f;
	float y_float = accel_y / 16384.0f;
	float z_float = accel_z / 16384.0f;

	int16_t gyro_x = 0;
	int16_t gyro_y = 0;
	int16_t gyro_z = 0;

	struct ImuRaw data_to_send = {
		{ accel_x, accel_y, accel_z },
		{ gyro_x, gyro_y, gyro_z },
		xTaskGetTickCountFromISR()
	};
	
	int a = xQueueOverwriteFromISR((QueueHandle_t) queueHandle, &data_to_send, NULL);
	int b = a;
}


void initReceiveDma(QueueHandle_t imuRawQueueHandle) {
	static DMA_CB_TypeDef callbackRX;
	callbackRX.cbFunc = (DMA_FuncPtr_TypeDef) rxCompletedCallback;
	callbackRX.userPtr = imuRawQueueHandle;

	DMA_CfgChannel_TypeDef channelConfigRX;
	channelConfigRX.highPri = false;
	channelConfigRX.enableInt = true;
	channelConfigRX.select = DMAREQ_USART1_RXDATAV;
	channelConfigRX.cb = &callbackRX;
	DMA_CfgChannel(SPI_RX_DMA_CHANNEL, &channelConfigRX);

	static DMA_CfgDescr_TypeDef descriptorConfigRX;
	descriptorConfigRX.dstInc = dmaDataInc1;
	descriptorConfigRX.srcInc = dmaDataIncNone;
	descriptorConfigRX.size = dmaDataSize1;
	descriptorConfigRX.arbRate = dmaArbitrate1;
	descriptorConfigRX.hprot = 0;
	bool isPrimaryDescriptor = true;
	DMA_CfgDescr(SPI_RX_DMA_CHANNEL, isPrimaryDescriptor, &descriptorConfigRX);
}


void initTransmitDma(void) {
	DMA_CfgChannel_TypeDef channelConfigTX;
	channelConfigTX.highPri = false;
	channelConfigTX.enableInt = true;
	channelConfigTX.select = DMAREQ_USART1_TXBL;
	channelConfigTX.cb = NULL;
	DMA_CfgChannel(SPI_TX_DMA_CHANNEL, &channelConfigTX);

	static DMA_CfgDescr_TypeDef descriptorConfigTX;
	descriptorConfigTX.dstInc = dmaDataIncNone;
	descriptorConfigTX.srcInc = dmaDataInc1;
	descriptorConfigTX.size = dmaDataSize1;
	descriptorConfigTX.arbRate = dmaArbitrate1;
	descriptorConfigTX.hprot = 0;
	bool isPrimaryDescriptor = true;
	DMA_CfgDescr(SPI_TX_DMA_CHANNEL, isPrimaryDescriptor, &descriptorConfigTX);
}


void initUSART1(void) {
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_USART1, true);

	GPIO_PinModeSet(gpioPortD, 0, gpioModePushPull, 1); // US1_TX (MOSI)
	GPIO_PinModeSet(gpioPortD, 1, gpioModeInput, 1);    // US1_RX (MISO)
	GPIO_PinModeSet(gpioPortD, 2, gpioModePushPull, 1); // US1_CLK
	GPIO_PinModeSet(gpioPortD, 3, gpioModePushPull, 1); // US1_CS

	USART_InitSync_TypeDef config = USART_INITSYNC_DEFAULT;
	config.master = true;
	config.baudrate = 1000000;          // CLK freq is 1 MHz
	config.autoCsEnable = true;         // CS pin controlled by hardware, not firmware
	config.clockMode = usartClockMode3; // clock idle high, sample on rising/trailing edge
	config.msbf = true;                 // send MSb first
	config.enable = usartDisable;       // keep USART disabled until we've set everything up
	USART_InitSync(USART1, &config);

	// Set and enable USART pin locations
	USART1->ROUTE = USART_ROUTE_CLKPEN |
	                USART_ROUTE_CSPEN |
	                USART_ROUTE_TXPEN |
	                USART_ROUTE_RXPEN |
	                USART_ROUTE_LOCATION_LOC1;

	USART_Enable(USART1, usartEnable);
}


void initIMU(QueueHandle_t imuRawQueueHandle) {
	initUSART1();

	USART_Tx(USART1, 0x10);  // Transmit CTRL1_XL register address
	USART_Tx(USART1, 0x60);  // Enable accelerometer (416 Hz, high performance mode)
	USART_Rx(USART1);  // Dump two bytes of "response" (junk Rx)
	USART_Rx(USART1);

	// Priority 6 of 7 (medium priority of three RTOS-enabled, which are 5–7)
	NVIC_SetPriority(DMA_IRQn, 0xDF);

	DMA_Init_TypeDef init;
	init.hprot = 0;
	init.controlBlock = dmaControlBlock;
	DMA_Init(&init);

	initReceiveDma(imuRawQueueHandle);
	initTransmitDma();
}


void queryIMU(void * pvParameters) {
	for ( ;; ) {
		// xTaskNotifyWait(0, ULONG_MAX, NULL, portMAX_DELAY);

		vTaskDelay(pdMS_TO_TICKS(100));  // For debug purposes

		DMA_ActivateBasic(SPI_RX_DMA_CHANNEL,
		                  true,   // primary (not alternative) descriptor
		                  false,  // don't use bursts
		                  (void *) RxBuffer,
		                  (void *) &USART1->RXDATA,
		                  SPI_BUFFER_SIZE - 1);

		DMA_ActivateBasic(SPI_TX_DMA_CHANNEL,
		                  true,
		                  false,
		                  (void *) &USART1->TXDATA,
		                  (void *) TxBuffer,
		                  SPI_BUFFER_SIZE - 1);
	}
}
