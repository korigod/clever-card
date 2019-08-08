#ifndef LEDS_H_INC
#define LEDS_H_INC

#include "em_gpio.h"

#include "board.h"

struct LedAnode {
	GPIO_Port_TypeDef port : 4;
	uint8_t id : 4;
};

struct LedCathode {
	GPIO_Port_TypeDef port : 4;
	uint8_t id : 4;
};

struct LedWires {
	struct LedAnode anode;
	struct LedCathode cathode;
};

uint8_t ledOutputsLatched[rgLedCount * 2];
uint8_t ledOutputs[rgLedCount * 2];

void latchLedOutputs(void);

#endif /* LEDS_H_INC */
