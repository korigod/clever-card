#include <string.h>

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_dma.h"

#include "common.h"
#include "lsm6ds3.h"
#include "lsm6ds3_registers.h"


static DMA_DESCRIPTOR_TypeDef dmaControlBlock[2] __attribute__((aligned(256)));


bool imuCommunicationInProgress(void) {
	return DMA_ChannelEnabled(SPI_RX_DMA_CHANNEL) || DMA_ChannelEnabled(SPI_TX_DMA_CHANNEL);
}


void initDMA(void) {
	// Priority 6 of 7 (medium priority of three RTOS-enabled, which are 5–7)
	NVIC_SetPriority(DMA_IRQn, 6);

	DMA_Init_TypeDef init;
	init.hprot = 0;
	init.controlBlock = dmaControlBlock;
	DMA_Init(&init);
}


void configureTransmitDMA(void) {
	DMA_CfgChannel_TypeDef channelConfigTX;
	channelConfigTX.highPri = false;
	channelConfigTX.enableInt = false;
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


void configureReceiveDMA(void) {
	DMA_CfgChannel_TypeDef channelConfigRX;
	channelConfigRX.highPri = false;
	channelConfigRX.enableInt = false;
	channelConfigRX.select = DMAREQ_USART1_RXDATAV;
	channelConfigRX.cb = NULL;
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


void initUSART1(int baudrate) {
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_USART1, true);

	GPIO_PinModeSet(gpioPortD, 0, gpioModePushPull, 1); // US1_TX (MOSI)
	GPIO_PinModeSet(gpioPortD, 1, gpioModeInput, 1);    // US1_RX (MISO)
	GPIO_PinModeSet(gpioPortD, 2, gpioModePushPull, 1); // US1_CLK
	GPIO_PinModeSet(gpioPortD, 3, gpioModePushPull, 1); // US1_CS

	USART_InitSync_TypeDef config = USART_INITSYNC_DEFAULT;
	config.master = true;
	config.baudrate = baudrate;
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
	uint8_t junkRxBuffer[2];
	uint8_t txBuffer[2] = {address, value};

	while (imuCommunicationInProgress()) {
		// Just to be sure that transfer will never be initiated
		// while previous one is still in progress.
	}

	DMA_ActivateBasic(
		SPI_RX_DMA_CHANNEL,
		true,    // primary (not alternative) descriptor
		false,   // don't use bursts
		(void *) junkRxBuffer,
		(void *) &USART1->RXDATA,
		2 - 1    // Number of DMA transfers minus 1
	);

	DMA_ActivateBasic(
		SPI_TX_DMA_CHANNEL,
		true,
		false,
		(void *) &USART1->TXDATA,
		(void *) txBuffer,
		2 - 1
	);

	while (imuCommunicationInProgress()) {
		// Just to be sure that transfer is completed.
	}
}


uint8_t readRegister(uint8_t address) {
	while (imuCommunicationInProgress()) {
		// Just to be sure that transfer will never be initiated
		// while previous one is still in progress.
	}

	uint8_t registerValueBuffer[2];
	uint8_t registerAddressBuffer[2] = {address | 1 << 7, 0xFF};

	DMA_ActivateBasic(
		SPI_RX_DMA_CHANNEL,
		true,    // primary (not alternative) descriptor
		false,   // don't use bursts
		(void *) registerValueBuffer,
		(void *) &USART1->RXDATA,
		2 - 1    // Number of DMA transfers minus 1
	);

	DMA_ActivateBasic(
		SPI_TX_DMA_CHANNEL,
		true,
		false,
		(void *) &USART1->TXDATA,
		(void *) registerAddressBuffer,
		2 - 1
	);

	while (imuCommunicationInProgress()) {
		// Just to be sure that transfer is completed.
	}

	return registerValueBuffer[1];
}


void readRegistersBlocking(uint8_t startAddress, uint8_t bytesCount, uint8_t * rxBuffer) {
	uint8_t txBuffer[bytesCount + 1];
	uint8_t tempRxBuffer[bytesCount + 1];

	// We transmit register address and then bytesCount of
	// dummy bytes to squeeze the data out of the IMU
	// (address increments automatically by the IMU)
	memset(txBuffer, 0xFF, bytesCount + 1);
	uint8_t startAddressWithReadBitSet = startAddress | 1 << 7;
	txBuffer[0] = startAddressWithReadBitSet;

	while (imuCommunicationInProgress()) {
		// Just to be sure that transfer will never be initiated
		// while previous one is still in progress.
	}

	DMA_ActivateBasic(
		SPI_RX_DMA_CHANNEL,
		true,    // primary (not alternative) descriptor
		false,   // don't use bursts
		(void *) tempRxBuffer,
		(void *) &USART1->RXDATA,
		bytesCount + 1 - 1  // Number of DMA transfers minus 1
	);

	DMA_ActivateBasic(
		SPI_TX_DMA_CHANNEL,
		true,
		false,
		(void *) &USART1->TXDATA,
		(void *) txBuffer,
		bytesCount + 1 - 1
	);

	while (imuCommunicationInProgress()) {
		// Just to be sure that transfer is completed.
	}

	memcpy(rxBuffer, tempRxBuffer + 1, bytesCount);
}


void startRegistersRead(uint8_t startAddress, uint8_t bytesToReadCount, uint8_t * junkByteAndThenRxBuffer) {
	uint8_t txBuffer[bytesToReadCount + 1];

	// We transmit register address and then bytesCount of
	// dummy bytes to squeeze the data out of the IMU
	// (address increments automatically by the IMU)
	memset(txBuffer, 0xFF, bytesToReadCount + 1);
	uint8_t startAddressWithReadBitSet = startAddress | 1 << 7;
	txBuffer[0] = startAddressWithReadBitSet;

	while (imuCommunicationInProgress()) {
		// Just to be sure that transfer will never be initiated
		// while previous one is still in progress.
	}

	DMA_ActivateBasic(
		SPI_RX_DMA_CHANNEL,
		true,    // primary (not alternative) descriptor
		false,   // don't use bursts
		(void *) junkByteAndThenRxBuffer,
		(void *) &USART1->RXDATA,
		bytesToReadCount + 1 - 1  // Number of DMA transfers minus 1
	);

	DMA_ActivateBasic(
		SPI_TX_DMA_CHANNEL,
		true,
		false,
		(void *) &USART1->TXDATA,
		(void *) txBuffer,
		bytesToReadCount + 1 - 1
	);
}


void switchLSM6DS3Power(bool powerEnabled) {
	GPIO_PinModeSet(gpioPortB, 11, gpioModeInputPull, !powerEnabled);
}


void initLSM6DS3(int spi_baudrate) {
	CMU_ClockEnable(cmuClock_GPIO, true);

	switchLSM6DS3Power(true);

	initUSART1(spi_baudrate);

	initDMA();
	configureReceiveDMA();
	configureTransmitDMA();
}
