#include <string.h>

#include "leds.h"

// Must be called from a critical section or protected with mutex
void latchLedOutputs(void) {
	memcpy(ledOutputsLatched, ledOutputs, sizeof(ledOutputs));
}
