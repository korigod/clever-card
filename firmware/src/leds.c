#include "em_gpio.h"

#include "leds.h"
#include "board.h"


void initLeds(void) {
	for (int i = 0; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
		GPIO_PinModeSet(ledAnodes[i].port, ledAnodes[i].id, gpioModePushPull, 0);
	}
}
