#include <string.h>

#include "leds.h"

bool cathodesAreEqual(struct LedCathode a, struct LedCathode b) {
	return (a.id == b.id) && (a.port == b.port);
}


// Must be called from a critical section or protected with mutex
void latchLedOutputs(void) {
	memcpy(ledOutputsLatched, ledOutputs, sizeof(ledOutputs));
}
