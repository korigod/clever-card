#ifndef LEDS_H_INC
#define LEDS_H_INC

#include "em_gpio.h"

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

// RgLed struct holds indices of individual leds in an RG LED package
// in ledPins, ledOutputs and ledOutputsLatched arrays
struct RgLed {
	uint8_t red;
	uint8_t green;
};

enum PrepareNextLedStatus {
	SUCCESS,
	NO_MORE_LEDS
};

struct PrepareNextLedResult {
	enum PrepareNextLedStatus status;
	struct LedPins led;
	uint16_t ticksToKeepLedOn;
};

void initializeLeds(void);

void latchLedOutputs(void);

struct PrepareNextLedResult prepareNextLed(bool loopIndefinitely);

void switchOnPreparedLed(struct LedPins led);

#endif /* LEDS_H_INC */
