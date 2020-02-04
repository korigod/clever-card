#include <string.h>

#include "common.h"
#include "board.h"
#include "leds.h"
#include "led_driver.h"
#include "timer.h"

#define MIN_TICKS_TO_KEEP_LED_ON timerMinTicksToWait

const uint8_t INVALID_LED = UINT8_MAX;
uint8_t currentLedIndex = UINT8_MAX;

uint8_t ledOutputsLatched[LED_COUNT] = {0};
uint8_t ledOutputs[LED_COUNT] = {0};


bool anodesAreEqual(const struct LedAnode a, const struct LedAnode b) {
	return (a.id == b.id) && (a.port == b.port);
}

bool cathodesAreEqual(const struct LedCathode a, const struct LedCathode b) {
	return (a.id == b.id) && (a.port == b.port);
}

bool ledPinsAreEqual(const struct LedPins a, const struct LedPins b) {
	return (anodesAreEqual(a.anode, b.anode)) && (cathodesAreEqual(a.cathode, b.cathode));
}


void initializeLeds(void) {
	currentLedIndex = INVALID_LED;
	clearLeds();
	latchLedOutputs();
	initializeTimer();
	initializeLedDriver();
	switchOffCathodes();
}


// Must be called from a critical section or protected with mutex
void latchLedOutputs(void) {
	ASSERT(sizeof(ledOutputs) == sizeof(ledOutputsLatched));
	memcpy(ledOutputsLatched, ledOutputs, sizeof(ledOutputs));
}


void clearLeds(void) {
	memset(ledOutputs, 0, sizeof(ledOutputs));
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
		// We iterate to skip LEDs with zero brightness
	} while (ledOutputsLatched[currentLedIndex] == 0);

	if (previousLedIndex == INVALID_LED ||
	    !cathodesAreEqual(ledPins[previousLedIndex].cathode,
	                      ledPins[currentLedIndex].cathode))
	{
		switchOffCathodes();
		switchOnCathode(ledPins[currentLedIndex].cathode);
	}

	prepareAnode(ledPins[currentLedIndex].anode);

	const uint16_t ledLuminance = ledOutputsLatched[currentLedIndex] * ledOutputsLatched[currentLedIndex];
	const uint16_t ticksToKeepLedOn = min(64000, timerMinTicksActuallyWait * ledLuminance - timerWaitingTicksOverhead);

	struct PrepareNextLedResult result = {
		SUCCESS, ledPins[currentLedIndex], ticksToKeepLedOn
	};
	return result;
}


void switchOnPreparedLed(const struct LedPins led) {
	switchOnAnode(led.anode);
}


void switchToNextLed(void) {
	switchOffAnodes();

	const struct PrepareNextLedResult result = prepareNextLed(false);

	if (result.status == NO_MORE_LEDS) {
		setTimerToWaitTicks(2000);
	} else if (result.status == SUCCESS) {
		setTimerToWaitTicks(result.ticksToKeepLedOn);
		switchOnPreparedLed(result.led);
	}
}
