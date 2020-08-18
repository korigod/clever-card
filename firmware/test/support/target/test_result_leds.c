#include "em_gpio.h"


void prepareTestLeds(void) {
	GPIO_DriveModeSet(gpioPortA, gpioDriveModeLowest);
	GPIO_PinModeSet(gpioPortA, 10, gpioModePushPullDrive, 1);
}

void switchOnGreenLed(void) {
	prepareTestLeds();
	GPIO_PinModeSet(gpioPortE, 8, gpioModeWiredAnd, 0);
}

void switchOnRedLed(void) {
	prepareTestLeds();
	GPIO_PinModeSet(gpioPortE, 9, gpioModeWiredAnd, 0);
}

void disableTestLeds(void) {
	GPIO_PinModeSet(gpioPortA, 10, gpioModeDisabled, 0);
	GPIO_PinModeSet(gpioPortE, 8, gpioModeDisabled, 0);
	GPIO_PinModeSet(gpioPortE, 9, gpioModeDisabled, 0);
}
