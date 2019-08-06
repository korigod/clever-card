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


void test_switchOffAnodes_rightPinsAreCleared(void) {
	switchOffAnodes();

	GPIO_TypeDef gpioResult = gpio;
	resetRegisters();

	for (int i = 0; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
		GPIO->P[ledAnodes[i].port].DOUTCLR |= 1 << ledAnodes[i].id;
	}

	TEST_ASSERT_EQUAL_MEMORY(&gpio, &gpioResult, sizeof(GPIO_TypeDef));
}
