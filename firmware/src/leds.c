#include "em_gpio.h"

#include "leds.h"


void initLeds(void) {
	// Set mode of anode lines (0-5, 8-10) to push-pull
	GPIO->P[gpioPortA].MODEL = 0x00444444;
	GPIO->P[gpioPortA].MODEH = 0x00000444;
}
