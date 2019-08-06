#include <string.h>
#include <stdio.h>
#include "em_gpio.h"
#include "unity.h"

#include "board.h"
#include "leds.h"


GPIO_TypeDef gpio;
GPIO_TypeDef * gpioBase = &gpio;


void resetRegisters(void) {
	memset(&gpio, 0, sizeof(gpio));
}


void setUp(void) {
	resetRegisters();
}


void tearDown(void) {
}


void test_initLeds_rightAnodesAreInitialized(void) {
	initLeds();

	GPIO_TypeDef gpioResult = gpio;

	resetRegisters();

	for (int i = 0; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
		GPIO_PinModeSet(ledAnodes[i].port, ledAnodes[i].id, gpioModePushPull, 0);
	}

	TEST_ASSERT(memcmp(&gpio, &gpioResult, sizeof(GPIO_TypeDef)) == 0);
}
