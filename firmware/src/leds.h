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

struct LedPins {
	struct LedAnode anode;
	struct LedCathode cathode;
};

enum SwitchOnNextLedStatus {
	SUCCESS,
	NO_MORE_LEDS
};

uint8_t ledOutputsLatched[LED_COUNT];
uint8_t ledOutputs[LED_COUNT];

#define INVALID_LED UINT8_MAX
uint8_t currentLedIndex = INVALID_LED;

bool cathodesAreEqual(struct LedCathode a, struct LedCathode b);

void latchLedOutputs(void);

enum SwitchOnNextLedStatus switchOnNextLed(bool loopIndefinitely);

#endif /* LEDS_H_INC */
