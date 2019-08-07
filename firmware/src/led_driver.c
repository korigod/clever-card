#include "em_gpio.h"

#include "led_driver.h"
#include "board.h"


void initLeds(void) {
	for (uint8_t i = 0; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
		GPIO_PinModeSet(ledAnodes[i].port, ledAnodes[i].id, gpioModePushPull, 0);
	}
}

void disableLeds(void) {
	switchOffCathodes();

	for (uint8_t i = 0; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
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


void switchOnCathode(struct LedCathode cathode) {
	GPIO_PinModeSet(cathode.port, cathode.id, gpioModeWiredAnd, 0);
}

// Set mode of all the LED cathode lines to DISABLED
void switchOffCathodes(void) {
	// The whole high half of port E (pins 8—15) are cathode lines
	GPIO->P[gpioPortE].MODEH = 0x0;

	// Only pins 2—5 of port F are cathodes, so we preserve the mode of the others
	GPIO->P[gpioPortF].MODEL &= 0xFF0000FF;
}
