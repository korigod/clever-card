#ifndef LEDS_H_INC
#define LEDS_H_INC

#include "em_gpio.h"

struct LedAnode {
	const GPIO_Port_TypeDef port : 4;
	const uint8_t id : 4;
};

struct LedCathode {
	const GPIO_Port_TypeDef port : 4;
	const uint8_t id : 4;
};

struct LedPins {
	const struct LedAnode anode;
	const struct LedCathode cathode;
};

// LedIds are indices of individual leds in RG LED package
// in ledPins, ledOutputs and ledOutputsLatched arrays.
typedef uint8_t LedId_t;

struct RgLed {
	const LedId_t red;
	const LedId_t green;
};

// RgLedIds are indices of the RG LED packages (each consists of
// a red and a green light emitting junctions) in rgLeds array.
typedef uint8_t RgLedId_t;

enum PrepareNextLedStatus {
	SUCCESS,
	NO_MORE_LEDS
};

struct PrepareNextLedResult {
	const enum PrepareNextLedStatus status;
	const struct LedPins led;
	const uint16_t ticksToKeepLedOn;
};

extern uint8_t ledOutputs[];

void initializeLeds(void);

void switchToNextLed(void);

void latchLedOutputs(void);

// It only clears ledOutputs buffer, doesn't affect latched values
void clearLeds(void);

#endif /* LEDS_H_INC */
