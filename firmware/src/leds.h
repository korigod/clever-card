#ifndef LEDS_H_INC
#define LEDS_H_INC

#include "em_gpio.h"

struct LedAnode {
	GPIO_Port_TypeDef port;
	uint8_t id;
};

static const struct LedAnode ledAnodes[] = {
	{gpioPortA, 0},
	{gpioPortA, 1},
	{gpioPortA, 2},
	{gpioPortA, 3},
	{gpioPortA, 4},
	{gpioPortA, 5},
	{gpioPortA, 8},
	{gpioPortA, 9},
	{gpioPortA, 10}
};

void initLeds();

#endif /* LEDS_H_INC */
