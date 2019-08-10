#include <string.h>

#include "leds.h"
#include "led_driver.h"

bool cathodesAreEqual(struct LedCathode a, struct LedCathode b) {
	return (a.id == b.id) && (a.port == b.port);
}


// Must be called from a critical section or protected with mutex
void latchLedOutputs(void) {
	memcpy(ledOutputsLatched, ledOutputs, sizeof(ledOutputs));
}


enum SwitchOnNextLedStatus switchOnNextLed(bool loopIndefinitely) {
	uint8_t previousLedIndex = currentLedIndex;

	do {
		currentLedIndex++;
		if (currentLedIndex >= ledCount) {
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
