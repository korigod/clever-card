#include <string.h>

#include "leds.h"
#include "led_driver.h"


bool anodesAreEqual(struct LedAnode a, struct LedAnode b) {
	return (a.id == b.id) && (a.port == b.port);
}

bool cathodesAreEqual(struct LedCathode a, struct LedCathode b) {
	return (a.id == b.id) && (a.port == b.port);
}

bool ledPinsAreEqual(struct LedPins a, struct LedPins b) {
	return (anodesAreEqual(a.anode, b.anode)) && (cathodesAreEqual(a.cathode, b.cathode));
}


// Must be called from a critical section or protected with mutex
void latchLedOutputs(void) {
	memcpy(ledOutputsLatched, ledOutputs, sizeof(ledOutputs));
}


enum SwitchOnNextLedStatus switchOnNextLed(bool loopIndefinitely) {
	uint8_t previousLedIndex = currentLedIndex;

	do {
		currentLedIndex++;
		if (currentLedIndex >= LED_COUNT) {
			if (loopIndefinitely) {
				currentLedIndex = 0;
			} else {
				currentLedIndex = INVALID_LED;
				switchOffCathodes();
				return NO_MORE_LEDS;
			}
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

	uint16_t ticksToWait = 5 * ledOutputsLatched[currentLedIndex];
	// setTimerToWaitTicks(ticksToWait);
	switchOnAnode(ledPins[currentLedIndex].anode);
	return SUCCESS;
}
