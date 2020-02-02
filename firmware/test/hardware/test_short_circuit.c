#include <stdio.h>
#include "em_gpio.h"
#include "unity.h"

#include "test_common.h"
#include "board.h"
#include "led_driver.h"


enum InputPullDirection {
	PULL_DOWN = 0,
	PULL_UP = 1,
	NO_PULL
};

void configureAllAnodesAsInputs(const enum InputPullDirection pullDirection) {
	for (uint8_t i = 0; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
		if (pullDirection == NO_PULL) {
			GPIO_PinModeSet(ledAnodes[i].port, ledAnodes[i].id, gpioModeInput, 0);
		} else {
			GPIO_PinModeSet(ledAnodes[i].port, ledAnodes[i].id, gpioModeInputPull, pullDirection);
		}
	}
}

void configureAllCathodesAsInputs(const enum InputPullDirection pullDirection) {
	for (uint8_t i = 0; i < sizeof(ledCathodes) / sizeof(ledCathodes[0]); i++) {
		if (pullDirection == NO_PULL) {
			GPIO_PinModeSet(ledCathodes[i].port, ledCathodes[i].id, gpioModeInput, 0);
		} else {
			GPIO_PinModeSet(ledCathodes[i].port, ledCathodes[i].id, gpioModeInputPull, pullDirection);
		}
	}
}


void additionalSuiteSetUp(void) {
	initializeLedDriver();
	switchOffAnodes();
	switchOffCathodes();

	for (uint8_t i = 0; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
		GPIO_DriveModeSet(ledAnodes[i].port, gpioDriveModeLow);
	}

	for (uint8_t i = 0; i < sizeof(ledCathodes) / sizeof(ledCathodes[0]); i++) {
		GPIO_DriveModeSet(ledCathodes[i].port, gpioDriveModeLow);
	}
}

void setUp(void) {
}

void tearDown(void) {
	switchOffAnodes();
	switchOffCathodes();
}


void test_ledAnodes(void) {
	const char messageTemplate[] = "Short circuit: anodes %u (port %u, pin %u) and %u (port %u, pin %u).";

	for (uint8_t i = 0; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
		configureAllAnodesAsInputs(PULL_DOWN);
		GPIO_PinModeSet(ledAnodes[i].port, ledAnodes[i].id, gpioModePushPullDrive, 1);
		for (uint8_t j = 0; j < sizeof(ledAnodes) / sizeof(ledAnodes[0]); j++) {
			if (j != i) {
				const unsigned int pinInput = GPIO_PinInGet(ledAnodes[j].port, ledAnodes[j].id);
				if (pinInput != 0) {
					char message[sizeof(messageTemplate)];
					snprintf(
						message, sizeof(messageTemplate), messageTemplate,
						i, ledAnodes[i].port, ledAnodes[i].id,
						j, ledAnodes[j].port, ledAnodes[j].id
					);
					TEST_FAIL_MESSAGE(message);
				}
			}
		}
	}
}


void test_ledCathodes(void) {
	const char messageTemplate[] = "Short circuit: cathodes %u (port %u, pin %u) and %u (port %u, pin %u).";

	for (uint8_t i = 0; i < sizeof(ledCathodes) / sizeof(ledCathodes[0]); i++) {
		configureAllCathodesAsInputs(PULL_DOWN);
		GPIO_PinModeSet(ledCathodes[i].port, ledCathodes[i].id, gpioModePushPullDrive, 1);
		for (uint8_t j = 0; j < sizeof(ledCathodes) / sizeof(ledCathodes[0]); j++) {
			if (j != i) {
				const unsigned int pinInput = GPIO_PinInGet(ledCathodes[j].port, ledCathodes[j].id);
				if (pinInput != 0) {
					char message[sizeof(messageTemplate)];
					snprintf(
						message, sizeof(messageTemplate), messageTemplate,
						i, ledCathodes[i].port, ledCathodes[i].id,
						j, ledCathodes[j].port, ledCathodes[j].id
					);
					TEST_FAIL_MESSAGE(message);
				}
			}
		}
	}
}
