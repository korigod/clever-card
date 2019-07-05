#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"


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

	// Enable USART1
	USART_Enable(USART1, usartEnable);
}


int main(void) {
	CHIP_Init();

	initUSART1();

	USART_Tx(USART1, 0x10);  // Transmit CTRL1_XL register address
	USART_Tx(USART1, 0x60);  // Enable accelerometer (416 Hz, high performance mode)

	while (1) {
		for (int i = 0; i < 1000; i++) {
			// This snippet is for debugging purposes only. -O0 flag is expected.
		}

		uint8_t RxBuffer[9];

		USART_Tx(USART1, 0xA8);  // Address of accelerometer output register

		for (int i = 0; i < 9; i++) {
			USART_Tx(USART1, 0xFF);
			RxBuffer[i] = USART_Rx(USART1);
		}
		int16_t x = RxBuffer[3] | RxBuffer[4] << 8;
		int16_t y = RxBuffer[5] | RxBuffer[6] << 8;
		int16_t z = RxBuffer[7] | RxBuffer[8] << 8;
		float x_float = x / 16384.0f;
		float y_float = y / 16384.0f;
		float z_float = z / 16384.0f;
	}
}
