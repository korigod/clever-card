#include <string.h>
#include <stdio.h>
#include "em_gpio.h"
#include "unity.h"

#include "board.h"
#include "led_driver.h"


GPIO_TypeDef gpio;
GPIO_TypeDef * gpioBase = &gpio;


void resetRegisters(void) {
	memset(&gpio, 0, sizeof(gpio));
}


void processGpioSetClearToggle(GPIO_TypeDef * gpio) {
	for (int i = 0; i < sizeof(gpio->P) / sizeof(gpio->P[0]); i++) {
		GPIO->P[i].DOUT |= GPIO->P[i].DOUTSET;
		GPIO->P[i].DOUTSET = 0;

		GPIO->P[i].DOUT &= ~GPIO->P[i].DOUTCLR;
		GPIO->P[i].DOUTCLR = 0;

		GPIO->P[i].DOUT ^= GPIO->P[i].DOUTTGL;
		GPIO->P[i].DOUTTGL = 0;
	}
}


void setUp(void) {
	resetRegisters();
}


void tearDown(void) {
}


void test_switchOffAnodes_rightPinsAreCleared(void) {
	switchOffAnodes();

	GPIO_TypeDef gpioResult = gpio;
	resetRegisters();

	for (int i = 0; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
		GPIO->P[ledAnodes[i].port].DOUTCLR |= 1 << ledAnodes[i].id;
	}

	TEST_ASSERT_EQUAL_MEMORY(&gpio, &gpioResult, sizeof(GPIO_TypeDef));
}


void test_switchOffCathodes_pinsAreDisabled(void) {
	for (int i = 0; i < sizeof(ledCathodes) / sizeof(ledCathodes[0]); i++) {
		GPIO_PinModeSet(ledCathodes[i].port, ledCathodes[i].id, gpioModeWiredAnd, 0);
	}

	switchOffCathodes();

	processGpioSetClearToggle(&gpio);

	GPIO_TypeDef gpioResult = gpio;
	resetRegisters();

	TEST_ASSERT_EQUAL_MEMORY(&gpio, &gpioResult, sizeof(GPIO_TypeDef));
}

void test_switchOffCathodes_otherPinsAreIntact(void) {
	for (int port = 0; port < sizeof(gpio.P) / sizeof(gpio.P[0]); port++) {
		for (int i = 0; i < 16; i++) {
			GPIO_PinModeSet(port, i, gpioModeWiredOr, 1);
		}
	}
	processGpioSetClearToggle(&gpio);

	for (int i = 0; i < sizeof(ledCathodes) / sizeof(ledCathodes[0]); i++) {
		GPIO_PinModeSet(ledCathodes[i].port, ledCathodes[i].id, gpioModeDisabled, 0);
	}
	processGpioSetClearToggle(&gpio);

	GPIO_TypeDef gpioToBePreserved = gpio;

	switchOffCathodes();
	processGpioSetClearToggle(&gpio);

	TEST_ASSERT_EQUAL_MEMORY(&gpioToBePreserved, &gpio, sizeof(GPIO_TypeDef));
}
