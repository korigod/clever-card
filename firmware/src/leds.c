#include "em_gpio.h"

#include "leds.h"
#include "board.h"


void initLeds(void) {
	for (int i = 0; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
		GPIO_PinModeSet(ledAnodes[i].port, ledAnodes[i].id, gpioModePushPull, 0);
	}
}

void disableLeds(void) {
	switchOffCathodes();

	for (int i = 0; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
		GPIO_PinModeSet(ledAnodes[i].port, ledAnodes[i].id, gpioModeDisabled, 0);
	}
}


void switchOnAnode(struct LedAnode anode) {
	GPIO->P[anode.port].DOUTSET = 1 << anode.id;
}

void switchOffAnodes(void) {
	// Anodes are pins 0–5 and 8–10 of port A
	GPIO->P[gpioPortA].DOUTCLR = 0x73F;  // 11100111111b
}
