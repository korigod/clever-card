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
#include "timer.h"

static DMA_DESCRIPTOR_TypeDef dmaControlBlock[2] __attribute__((aligned(256)));

uint8_t TxBuffer[SPI_BUFFER_SIZE] = { 0xA2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };


void rxCompletedCallback(uint32_t channelNum, bool isPrimaryDescriptor, void *queueHandle) {
	int16_t accelX = RxBuffer[7] | RxBuffer[8] << 8;
	int16_t accelY = RxBuffer[9] | RxBuffer[10] << 8;
	int16_t accelZ = RxBuffer[11] | RxBuffer[12] << 8;

	// Default acceleration limits are -2g/+2g, so 1g is max_int16 / 2
	float xFloat = accelX / 16384.0f;
	float yFloat = accelY / 16384.0f;
	float zFloat = accelZ / 16384.0f;

	int16_t gyroX = RxBuffer[1] | RxBuffer[2] << 8;
	int16_t gyroY = RxBuffer[3] | RxBuffer[4] << 8;
	int16_t gyroZ = RxBuffer[5] | RxBuffer[6] << 8;

	struct ImuRaw dataToSend = {
		{ accelX, accelY, accelZ },
		{ gyroX, gyroY, gyroZ },
		xTaskGetTickCountFromISR()
	};

	xQueueOverwriteFromISR((QueueHandle_t) queueHandle, &dataToSend, NULL);
}


void configureReceiveDMA(QueueHandle_t imuRawQueueHandle) {
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


void configureTransmitDMA(void) {
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
	config.baudrate = LSM6DS3_SPI_BAUDRATE;
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


void writeRegister(uint8_t address, uint8_t value) {
	USART_Tx(USART1, address);
	USART_Tx(USART1, value);

	// Dump two bytes of "response" (junk Rx)
	USART_Rx(USART1);
	USART_Rx(USART1);

	delayMicroseconds(8 * 1000000 / LSM6DS3_SPI_BAUDRATE);
}


void initIMU(QueueHandle_t imuRawQueueHandle) {
	initUSART1();

	// Priority 6 of 7 (medium priority of three RTOS-enabled, which are 5–7)
	NVIC_SetPriority(DMA_IRQn, 0xDF);

	DMA_Init_TypeDef init;
	init.hprot = 0;
	init.controlBlock = dmaControlBlock;
	DMA_Init(&init);

	configureReceiveDMA(imuRawQueueHandle);
	configureTransmitDMA();

	// Enable accelerometer (12.5 Hz, low-power mode)
	writeRegister(0x10, 0x10);

	// Enable angular rate sensor (12.5 Hz, low-power mode)
	writeRegister(0x11, 0x10);

	// Enable accelerometer Data Ready signal on INT1 line
	writeRegister(0x0D, 0x01);
}


void queryIMU(void * pvParameters) {
	for ( ;; ) {
		uint32_t blockedTime = usecondsTillBoot();

		// The timeout value should be decreased in production
		xTaskNotifyWait(0, 0, NULL, pdMS_TO_TICKS(200));

		if (usecondsTillBoot() - blockedTime < 1000) {
			// Throttle IMU queries
			vTaskDelay(pdMS_TO_TICKS(1));
		}

		while (DMA_ChannelEnabled(SPI_RX_DMA_CHANNEL) ||
		       DMA_ChannelEnabled(SPI_TX_DMA_CHANNEL)) {
			// Just to be sure that transfer will never be initiated
			// while previous one is still in progress.
			vTaskDelay(1);
		}

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
