#ifndef BOARD_H_INC
#define BOARD_H_INC

#include "em_gpio.h"

#include "led_driver.h"

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

static const struct LedCathode ledCathodes[] = {
	{gpioPortE, 8},
	{gpioPortE, 9},
	{gpioPortE, 10},
	{gpioPortE, 11},
	{gpioPortE, 12},
	{gpioPortE, 13},
	{gpioPortE, 14},
	{gpioPortE, 15},
	{gpioPortF, 2},
	{gpioPortF, 3},
	{gpioPortF, 4},
	{gpioPortF, 5}
};

#endif /* BOARD_H_INC */
