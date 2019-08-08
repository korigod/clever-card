#ifndef BOARD_H_INC
#define BOARD_H_INC

#include "em_gpio.h"

#include "leds.h"

const uint8_t rgLedCount = 49;

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

static const struct LedWires ledWires[rgLedCount * 2] = {
	{ledAnodes[0], ledCathodes[0]},
	{ledAnodes[1], ledCathodes[0]},
	{ledAnodes[2], ledCathodes[0]},
	{ledAnodes[3], ledCathodes[0]},
	{ledAnodes[4], ledCathodes[0]},
	{ledAnodes[5], ledCathodes[0]},
	{ledAnodes[6], ledCathodes[0]},
	{ledAnodes[7], ledCathodes[0]},
	{ledAnodes[8], ledCathodes[0]},

	{ledAnodes[0], ledCathodes[1]},
	{ledAnodes[1], ledCathodes[1]},
	{ledAnodes[2], ledCathodes[1]},
	{ledAnodes[3], ledCathodes[1]},
	{ledAnodes[4], ledCathodes[1]},
	{ledAnodes[5], ledCathodes[1]},
	{ledAnodes[6], ledCathodes[1]},
	{ledAnodes[7], ledCathodes[1]},
	{ledAnodes[8], ledCathodes[1]},

	{ledAnodes[0], ledCathodes[2]},
	{ledAnodes[1], ledCathodes[2]},
	{ledAnodes[2], ledCathodes[2]},
	{ledAnodes[3], ledCathodes[2]},
	{ledAnodes[4], ledCathodes[2]},
	{ledAnodes[5], ledCathodes[2]},
	{ledAnodes[6], ledCathodes[2]},
	{ledAnodes[7], ledCathodes[2]},

	{ledAnodes[0], ledCathodes[3]},
	{ledAnodes[1], ledCathodes[3]},
	{ledAnodes[2], ledCathodes[3]},
	{ledAnodes[3], ledCathodes[3]},
	{ledAnodes[4], ledCathodes[3]},
	{ledAnodes[5], ledCathodes[3]},
	{ledAnodes[6], ledCathodes[3]},
	{ledAnodes[7], ledCathodes[3]},

	{ledAnodes[0], ledCathodes[4]},
	{ledAnodes[1], ledCathodes[4]},
	{ledAnodes[2], ledCathodes[4]},
	{ledAnodes[3], ledCathodes[4]},
	{ledAnodes[4], ledCathodes[4]},
	{ledAnodes[5], ledCathodes[4]},
	{ledAnodes[6], ledCathodes[4]},
	{ledAnodes[7], ledCathodes[4]},

	{ledAnodes[0], ledCathodes[5]},
	{ledAnodes[1], ledCathodes[5]},
	{ledAnodes[2], ledCathodes[5]},
	{ledAnodes[3], ledCathodes[5]},
	{ledAnodes[4], ledCathodes[5]},
	{ledAnodes[5], ledCathodes[5]},
	{ledAnodes[6], ledCathodes[5]},
	{ledAnodes[7], ledCathodes[5]},

	{ledAnodes[0], ledCathodes[6]},
	{ledAnodes[1], ledCathodes[6]},
	{ledAnodes[2], ledCathodes[6]},
	{ledAnodes[3], ledCathodes[6]},
	{ledAnodes[4], ledCathodes[6]},
	{ledAnodes[5], ledCathodes[6]},
	{ledAnodes[6], ledCathodes[6]},
	{ledAnodes[7], ledCathodes[6]},

	{ledAnodes[0], ledCathodes[7]},
	{ledAnodes[1], ledCathodes[7]},
	{ledAnodes[2], ledCathodes[7]},
	{ledAnodes[3], ledCathodes[7]},
	{ledAnodes[4], ledCathodes[7]},
	{ledAnodes[5], ledCathodes[7]},
	{ledAnodes[6], ledCathodes[7]},
	{ledAnodes[7], ledCathodes[7]},

	{ledAnodes[0], ledCathodes[8]},
	{ledAnodes[1], ledCathodes[8]},
	{ledAnodes[2], ledCathodes[8]},
	{ledAnodes[3], ledCathodes[8]},
	{ledAnodes[4], ledCathodes[8]},
	{ledAnodes[5], ledCathodes[8]},
	{ledAnodes[6], ledCathodes[8]},
	{ledAnodes[7], ledCathodes[8]},

	{ledAnodes[0], ledCathodes[9]},
	{ledAnodes[1], ledCathodes[9]},
	{ledAnodes[2], ledCathodes[9]},
	{ledAnodes[3], ledCathodes[9]},
	{ledAnodes[4], ledCathodes[9]},
	{ledAnodes[5], ledCathodes[9]},
	{ledAnodes[6], ledCathodes[9]},
	{ledAnodes[7], ledCathodes[9]},

	{ledAnodes[0], ledCathodes[10]},
	{ledAnodes[1], ledCathodes[10]},
	{ledAnodes[2], ledCathodes[10]},
	{ledAnodes[3], ledCathodes[10]},
	{ledAnodes[4], ledCathodes[10]},
	{ledAnodes[5], ledCathodes[10]},
	{ledAnodes[6], ledCathodes[10]},
	{ledAnodes[7], ledCathodes[10]},

	{ledAnodes[0], ledCathodes[11]},
	{ledAnodes[1], ledCathodes[11]},
	{ledAnodes[2], ledCathodes[11]},
	{ledAnodes[3], ledCathodes[11]},
	{ledAnodes[4], ledCathodes[11]},
	{ledAnodes[5], ledCathodes[11]},
	{ledAnodes[6], ledCathodes[11]},
	{ledAnodes[7], ledCathodes[11]}
};

#endif /* BOARD_H_INC */
