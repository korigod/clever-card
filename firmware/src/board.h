#ifndef BOARD_H_INC
#define BOARD_H_INC

#include "em_gpio.h"

#include "leds.h"

#define RG_LED_COUNT 49
#define LED_COUNT (RG_LED_COUNT * 2)

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

static const struct LedPins ledPins[LED_COUNT] = {
	{{gpioPortA, 0}, {gpioPortE, 8}},
	{{gpioPortA, 1}, {gpioPortE, 8}},
	{{gpioPortA, 2}, {gpioPortE, 8}},
	{{gpioPortA, 3}, {gpioPortE, 8}},
	{{gpioPortA, 4}, {gpioPortE, 8}},
	{{gpioPortA, 5}, {gpioPortE, 8}},
	{{gpioPortA, 8}, {gpioPortE, 8}},
	{{gpioPortA, 9}, {gpioPortE, 8}},
	{{gpioPortA, 10}, {gpioPortE, 8}},

	{{gpioPortA, 10}, {gpioPortE, 9}},
	{{gpioPortA, 0}, {gpioPortE, 9}},
	{{gpioPortA, 1}, {gpioPortE, 9}},
	{{gpioPortA, 2}, {gpioPortE, 9}},
	{{gpioPortA, 3}, {gpioPortE, 9}},
	{{gpioPortA, 4}, {gpioPortE, 9}},
	{{gpioPortA, 5}, {gpioPortE, 9}},
	{{gpioPortA, 8}, {gpioPortE, 9}},
	{{gpioPortA, 9}, {gpioPortE, 9}},

	{{gpioPortA, 0}, {gpioPortE, 10}},
	{{gpioPortA, 1}, {gpioPortE, 10}},
	{{gpioPortA, 2}, {gpioPortE, 10}},
	{{gpioPortA, 3}, {gpioPortE, 10}},
	{{gpioPortA, 4}, {gpioPortE, 10}},
	{{gpioPortA, 5}, {gpioPortE, 10}},
	{{gpioPortA, 8}, {gpioPortE, 10}},
	{{gpioPortA, 9}, {gpioPortE, 10}},

	{{gpioPortA, 0}, {gpioPortE, 11}},
	{{gpioPortA, 1}, {gpioPortE, 11}},
	{{gpioPortA, 2}, {gpioPortE, 11}},
	{{gpioPortA, 3}, {gpioPortE, 11}},
	{{gpioPortA, 4}, {gpioPortE, 11}},
	{{gpioPortA, 5}, {gpioPortE, 11}},
	{{gpioPortA, 8}, {gpioPortE, 11}},
	{{gpioPortA, 9}, {gpioPortE, 11}},

	{{gpioPortA, 0}, {gpioPortE, 12}},
	{{gpioPortA, 1}, {gpioPortE, 12}},
	{{gpioPortA, 2}, {gpioPortE, 12}},
	{{gpioPortA, 3}, {gpioPortE, 12}},
	{{gpioPortA, 4}, {gpioPortE, 12}},
	{{gpioPortA, 5}, {gpioPortE, 12}},
	{{gpioPortA, 8}, {gpioPortE, 12}},
	{{gpioPortA, 9}, {gpioPortE, 12}},

	{{gpioPortA, 0}, {gpioPortE, 13}},
	{{gpioPortA, 1}, {gpioPortE, 13}},
	{{gpioPortA, 2}, {gpioPortE, 13}},
	{{gpioPortA, 3}, {gpioPortE, 13}},
	{{gpioPortA, 4}, {gpioPortE, 13}},
	{{gpioPortA, 5}, {gpioPortE, 13}},
	{{gpioPortA, 8}, {gpioPortE, 13}},
	{{gpioPortA, 9}, {gpioPortE, 13}},

	{{gpioPortA, 0}, {gpioPortE, 14}},
	{{gpioPortA, 1}, {gpioPortE, 14}},
	{{gpioPortA, 2}, {gpioPortE, 14}},
	{{gpioPortA, 3}, {gpioPortE, 14}},
	{{gpioPortA, 4}, {gpioPortE, 14}},
	{{gpioPortA, 5}, {gpioPortE, 14}},
	{{gpioPortA, 8}, {gpioPortE, 14}},
	{{gpioPortA, 9}, {gpioPortE, 14}},

	{{gpioPortA, 0}, {gpioPortE, 15}},
	{{gpioPortA, 1}, {gpioPortE, 15}},
	{{gpioPortA, 2}, {gpioPortE, 15}},
	{{gpioPortA, 3}, {gpioPortE, 15}},
	{{gpioPortA, 4}, {gpioPortE, 15}},
	{{gpioPortA, 5}, {gpioPortE, 15}},
	{{gpioPortA, 8}, {gpioPortE, 15}},
	{{gpioPortA, 9}, {gpioPortE, 15}},

	{{gpioPortA, 0}, {gpioPortF, 2}},
	{{gpioPortA, 1}, {gpioPortF, 2}},
	{{gpioPortA, 2}, {gpioPortF, 2}},
	{{gpioPortA, 3}, {gpioPortF, 2}},
	{{gpioPortA, 4}, {gpioPortF, 2}},
	{{gpioPortA, 5}, {gpioPortF, 2}},
	{{gpioPortA, 8}, {gpioPortF, 2}},
	{{gpioPortA, 9}, {gpioPortF, 2}},

	{{gpioPortA, 0}, {gpioPortF, 3}},
	{{gpioPortA, 1}, {gpioPortF, 3}},
	{{gpioPortA, 2}, {gpioPortF, 3}},
	{{gpioPortA, 3}, {gpioPortF, 3}},
	{{gpioPortA, 4}, {gpioPortF, 3}},
	{{gpioPortA, 5}, {gpioPortF, 3}},
	{{gpioPortA, 8}, {gpioPortF, 3}},
	{{gpioPortA, 9}, {gpioPortF, 3}},

	{{gpioPortA, 0}, {gpioPortF, 4}},
	{{gpioPortA, 1}, {gpioPortF, 4}},
	{{gpioPortA, 2}, {gpioPortF, 4}},
	{{gpioPortA, 3}, {gpioPortF, 4}},
	{{gpioPortA, 4}, {gpioPortF, 4}},
	{{gpioPortA, 5}, {gpioPortF, 4}},
	{{gpioPortA, 8}, {gpioPortF, 4}},
	{{gpioPortA, 9}, {gpioPortF, 4}},

	{{gpioPortA, 0}, {gpioPortF, 5}},
	{{gpioPortA, 1}, {gpioPortF, 5}},
	{{gpioPortA, 2}, {gpioPortF, 5}},
	{{gpioPortA, 3}, {gpioPortF, 5}},
	{{gpioPortA, 4}, {gpioPortF, 5}},
	{{gpioPortA, 5}, {gpioPortF, 5}},
	{{gpioPortA, 8}, {gpioPortF, 5}},
	{{gpioPortA, 9}, {gpioPortF, 5}}
};

#endif /* BOARD_H_INC */
