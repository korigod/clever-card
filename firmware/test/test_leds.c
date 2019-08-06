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
