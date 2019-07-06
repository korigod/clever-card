#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_dma.h"

#define TX_BUFFER_SIZE   8
#define RX_BUFFER_SIZE   TX_BUFFER_SIZE

uint8_t TxBuffer[TX_BUFFER_SIZE] = { 0xA8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t RxBuffer[RX_BUFFER_SIZE];

DMA_DESCRIPTOR_TypeDef dmaControlBlock[32] __attribute__((aligned(256)));

volatile uint32_t msTicks = 0;
uint32_t startTransferTime = 0;

void SysTick_Handler(void) {
	msTicks++;

	if (msTicks == startTransferTime) {
		DMA_ActivateBasic(1,      // DMA Channel #1
		                  true,   // primary (not alternative) descriptor
		                  false,  // don't use bursts
		                  (void *) &USART1->TXDATA,
		                  (void *) TxBuffer,
		                  RX_BUFFER_SIZE - 1);
	}
}

// Beware of jitter (up to 1 ms)
void delay(uint32_t delayMilliseconds) {
	uint32_t curTicks;
	curTicks = msTicks;
	while ((msTicks - curTicks) < delayMilliseconds) {
	}
}


void rxCompletedCallback(uint32_t channelNum, bool isPrimaryDescriptor, void *userPtr) {
	// RxBuffer[1] contains the LSB of X axis acceleration reading,
	// and RxBuffer[2] contains the MSB.
	int16_t x = RxBuffer[1] | RxBuffer[2] << 8;
	int16_t y = RxBuffer[3] | RxBuffer[4] << 8;
	int16_t z = RxBuffer[5] | RxBuffer[6] << 8;
	// There should be 0x00 in RxBuffer[7] if the data is read correctly.
	// RxBuffer[0] is junk data and should be discarded.

	// Default acceleration limits are -2g/+2g, so 1g is max_int16 / 2
	float x_float = x / 16384.0f;
	float y_float = y / 16384.0f;
	float z_float = z / 16384.0f;

	// Place breakpoint here and observe accelerometer data

	DMA_ActivateBasic(channelNum,
	                  isPrimaryDescriptor,
	                  false,  // don't use bursts
	                  (void *) RxBuffer,
	                  (void *) &USART1->RXDATA,
	                  RX_BUFFER_SIZE - 1);
}


void txCompletedCallback(uint32_t channelNum, bool isPrimaryDescriptor, void *userPtr) {
	// Schedule next acceleration measurement data request in 5 ms
	startTransferTime = msTicks + 5;
}


void initReceiveDma(void) {
	static DMA_CB_TypeDef callbackRX;
	callbackRX.cbFunc = (DMA_FuncPtr_TypeDef) rxCompletedCallback;
	callbackRX.userPtr = NULL;

	DMA_CfgChannel_TypeDef channelConfigRX;
	channelConfigRX.highPri = false;
	channelConfigRX.enableInt = true;
	channelConfigRX.select = DMAREQ_USART1_RXDATAV;
	channelConfigRX.cb = &callbackRX;
	uint32_t channelNumRX = 0;
	DMA_CfgChannel(channelNumRX, &channelConfigRX);

	static DMA_CfgDescr_TypeDef descriptorConfigRX;
	descriptorConfigRX.dstInc = dmaDataInc1;
	descriptorConfigRX.srcInc = dmaDataIncNone;
	descriptorConfigRX.size = dmaDataSize1;
	descriptorConfigRX.arbRate = dmaArbitrate1;
	descriptorConfigRX.hprot = 0;
	bool isPrimaryDescriptor = true;
	DMA_CfgDescr(channelNumRX, isPrimaryDescriptor, &descriptorConfigRX);

	DMA_ActivateBasic(channelNumRX,
	                  isPrimaryDescriptor,
	                  false,
	                  (void *) RxBuffer,
	                  (void *) &USART1->RXDATA,
	                  RX_BUFFER_SIZE - 1);
}


void initTransmitDma(void) {
	static DMA_CB_TypeDef callbackTX;
	callbackTX.cbFunc = (DMA_FuncPtr_TypeDef) txCompletedCallback;
	callbackTX.userPtr = NULL;

	DMA_CfgChannel_TypeDef channelConfigTX;
	channelConfigTX.highPri = false;
	channelConfigTX.enableInt = true;
	channelConfigTX.select = DMAREQ_USART1_TXBL;
	channelConfigTX.cb = &callbackTX;
	uint32_t channelNumTX = 1;
	DMA_CfgChannel(channelNumTX, &channelConfigTX);

	static DMA_CfgDescr_TypeDef descriptorConfigTX;
	descriptorConfigTX.dstInc = dmaDataIncNone;
	descriptorConfigTX.srcInc = dmaDataInc1;
	descriptorConfigTX.size = dmaDataSize1;
	descriptorConfigTX.arbRate = dmaArbitrate1;
	descriptorConfigTX.hprot = 0;
	bool isPrimaryDescriptor = true;
	DMA_CfgDescr(channelNumTX, isPrimaryDescriptor, &descriptorConfigTX);

	DMA_ActivateBasic(channelNumTX,
	                  isPrimaryDescriptor,
	                  false,
	                  (void *) &USART1->TXDATA,
	                  (void *) TxBuffer,
	                  TX_BUFFER_SIZE - 1);
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


int main(void) {
	CHIP_Init();

	// Setup SysTick Timer to generate 1 ms interrupts
	if (SysTick_Config(14000UL)) {
		while (1) {
		}
	}

	initUSART1();

	USART_Tx(USART1, 0x10);  // Transmit CTRL1_XL register address
	USART_Tx(USART1, 0x60);  // Enable accelerometer (416 Hz, high performance mode)
	USART_Rx(USART1);  // Dump two bytes of "response" (junk Rx)
	USART_Rx(USART1);

	delay(2);

	DMA_Init_TypeDef init;
	init.hprot = 0;
	init.controlBlock = dmaControlBlock;
	DMA_Init(&init);

	initReceiveDma();
	initTransmitDma();

	while (1) {
	}
}
