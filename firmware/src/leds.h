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

// LedIds are indices of individual leds in RG LED package
// in ledPins, ledOutputs and ledOutputsLatched arrays.
typedef uint8_t LedId_t;

struct RgLed {
	LedId_t red;
	LedId_t green;
};

// RgLedIds are indices of the RG LED packages (each consists of
// a red and a green light emitting junctions) in rgLeds array.
typedef uint8_t RgLedId_t;

enum PrepareNextLedStatus {
	SUCCESS,
	NO_MORE_LEDS
};

struct PrepareNextLedResult {
	enum PrepareNextLedStatus status;
	struct LedPins led;
	uint16_t ticksToKeepLedOn;
};

extern uint8_t ledOutputs[];

void initializeLeds(void);

void switchToNextLed(void);

void latchLedOutputs(void);

// It only clears ledOutputs buffer, doesn't affect latched values
void clearLeds(void);

#endif /* LEDS_H_INC */
