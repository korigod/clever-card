#include "em_gpio.h"
#include "em_cmu.h"

#include "led_driver.h"
#include "board.h"


void initializeLedDriver(void) {
	CMU_ClockEnable(cmuClock_GPIO, true);

	for (uint8_t i = 0; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
		GPIO_DriveModeSet(ledAnodes[i].port, gpioDriveModeStandard);
	}
}

void disableLeds(void) {
	switchOffAnodes();
	switchOffCathodes();
}


void switchOnAnode(struct LedAnode anode) {
	GPIO_PinModeSet(anode.port, anode.id, gpioModePushPullDrive, 1);
}

void switchOffAnodes(void) {
	// Anodes are pins 0–5 and 8–10 of port A

	// This registry write switches the leds off immediately and simulteneously, then we
	// disable the pins to minimize possible problems if some lines was short-curcuited
	GPIO->P[gpioPortA].DOUTCLR = 0x73F;  // 11100111111b

	GPIO->P[gpioPortA].MODEL &= 0xFF000000;
	GPIO->P[gpioPortA].MODEH &= 0xFFFFF000;
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
