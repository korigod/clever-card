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

static const struct RgLed rgLeds[] = {
	{1, 0}, {3, 2}, {5, 4}, {7, 6}, {11, 10}, {13, 12}, {15, 14}, {17, 16},
	{19, 18}, {21, 20}, {23, 22}, {25, 24}, {27, 26}, {29, 28}, {31, 30}, {33, 32},
	{35, 34}, {37, 36}, {39, 38}, {41, 40}, {43, 42}, {45, 44}, {47, 46}, {49, 48},
	{50, 51}, {52, 53}, {54, 55}, {56, 57}, {58, 59}, {60, 61}, {62, 63}, {64, 65},
	{66, 67}, {68, 69}, {70, 71}, {72, 73}, {74, 75}, {76, 77}, {78, 79}, {80, 81},
	{82, 83}, {84, 85}, {86, 87}, {88, 89}, {90, 91}, {92, 93}, {94, 95}, {96, 97},
	{9, 8}
};

static const RgLedId_t horizontalLeds[] = {14, 10, 6, 2, 48, 0, 4, 8, 12};

static const RgLedId_t verticalLeds[] = {13, 9, 5, 1, 48, 3, 7, 11, 15};

static const RgLedId_t topLeftArcLeds[] = {18, 22, 26, 30, 34, 38, 42, 46};

static const RgLedId_t topRightArcLeds[] = {19, 23, 27, 31, 35, 39, 43, 47};

static const RgLedId_t bottomRightArcLeds[] = {16, 20, 24, 28, 32, 36, 40, 44};

static const RgLedId_t bottomLeftArcLeds[] = {17, 21, 25, 29, 33, 37, 41, 45};

static const RgLedId_t allArcLeds[] = {
	18, 22, 26, 30, 34, 38, 42, 46, 19, 23, 27, 31, 35, 39, 43, 47,
	16, 20, 24, 28, 32, 36, 40, 44, 17, 21, 25, 29, 33, 37, 41, 45
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
