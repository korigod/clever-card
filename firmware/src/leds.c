#include <string.h>

#include "board.h"
#include "leds.h"
#include "led_driver.h"

const uint8_t INVALID_LED = UINT8_MAX;
uint8_t currentLedIndex = UINT8_MAX;

uint8_t ledOutputsLatched[LED_COUNT] = {0};
uint8_t ledOutputs[LED_COUNT] = {0};


bool anodesAreEqual(struct LedAnode a, struct LedAnode b) {
	return (a.id == b.id) && (a.port == b.port);
}

bool cathodesAreEqual(struct LedCathode a, struct LedCathode b) {
	return (a.id == b.id) && (a.port == b.port);
}

bool ledPinsAreEqual(struct LedPins a, struct LedPins b) {
	return (anodesAreEqual(a.anode, b.anode)) && (cathodesAreEqual(a.cathode, b.cathode));
}


void initializeLeds(void) {
	currentLedIndex = INVALID_LED;
	for (int i = 0; i < LED_COUNT; i++) {
		ledOutputs[i] = 0;
		ledOutputsLatched[i] = 0;
	}
	initializeLedDriver();
	switchOffCathodes();
}


// Must be called from a critical section or protected with mutex
void latchLedOutputs(void) {
	//ASSERT(sizeof(ledOutputs) == sizeof(ledOutputsLatched));
	memcpy(ledOutputsLatched, ledOutputs, sizeof(ledOutputs));
}


struct PrepareNextLedResult prepareNextLed(bool loopIndefinitely) {
	uint8_t previousLedIndex = currentLedIndex;

	do {
		currentLedIndex++;
		if (currentLedIndex >= LED_COUNT) {
			if (loopIndefinitely) {
				currentLedIndex = 0;
				loopIndefinitely = false;
			} else {
				currentLedIndex = INVALID_LED;
				switchOffCathodes();
				struct PrepareNextLedResult result = {NO_MORE_LEDS, {{0, 0}, {0, 0}}, 0};
				return result;
			}
		}
		if (currentLedIndex == 0) {
			latchLedOutputs();
		}
		// We iterate to skip LEDs with zero brightness
	} while (ledOutputsLatched[currentLedIndex] == 0);

	if (previousLedIndex == INVALID_LED ||
	    !cathodesAreEqual(ledPins[previousLedIndex].cathode,
	                      ledPins[currentLedIndex].cathode))
	{
		switchOffCathodes();
		switchOnCathode(ledPins[currentLedIndex].cathode);
	}

	uint16_t ticksToKeepLedOn = 5 * ledOutputsLatched[currentLedIndex];

	struct PrepareNextLedResult result = {
		SUCCESS, ledPins[currentLedIndex], ticksToKeepLedOn
	};
	return result;
}


void switchOnPreparedLed(struct LedPins led) {
	switchOnAnode(led.anode);
}
