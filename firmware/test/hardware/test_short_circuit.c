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


TEST_VALUE(0)
TEST_VALUE(1)
TEST_VALUE(2)
TEST_VALUE(3)
TEST_VALUE(4)
TEST_VALUE(5)
TEST_VALUE(6)
TEST_VALUE(7)
void test_ledAnodes(uint16_t anodeIndex) {
	const char messageTemplate[] = "Short circuit: anodes %u (port %u, pin %u) and %u (port %u, pin %u).";

	configureAllAnodesAsInputs(PULL_DOWN);
	GPIO_PinModeSet(ledAnodes[anodeIndex].port, ledAnodes[anodeIndex].id, gpioModePushPullDrive, 1);
	for (uint16_t i = anodeIndex + 1; i < sizeof(ledAnodes) / sizeof(ledAnodes[0]); i++) {
		if (i != anodeIndex) {
			const unsigned int pinInput = GPIO_PinInGet(ledAnodes[i].port, ledAnodes[i].id);
			if (pinInput != 0) {
				char message[sizeof(messageTemplate)];
				snprintf(
					message, sizeof(messageTemplate), messageTemplate,
					anodeIndex, ledAnodes[anodeIndex].port, ledAnodes[anodeIndex].id,
					i, ledAnodes[i].port, ledAnodes[i].id
				);
				TEST_FAIL_MESSAGE(message);
			}
		}
	}
}


TEST_VALUE(0)
TEST_VALUE(1)
TEST_VALUE(2)
TEST_VALUE(3)
TEST_VALUE(4)
TEST_VALUE(5)
TEST_VALUE(6)
TEST_VALUE(7)
TEST_VALUE(8)
TEST_VALUE(9)
TEST_VALUE(10)
void test_ledCathodes(uint16_t cathodeIndex) {
	const char messageTemplate[] = "Short circuit: cathodes %u (port %u, pin %u) and %u (port %u, pin %u).";

	configureAllCathodesAsInputs(PULL_DOWN);
	GPIO_PinModeSet(ledCathodes[cathodeIndex].port, ledCathodes[cathodeIndex].id, gpioModePushPullDrive, 1);
	for (uint16_t i = cathodeIndex + 1; i < sizeof(ledCathodes) / sizeof(ledCathodes[0]); i++) {
		if (i != cathodeIndex) {
			const unsigned int pinInput = GPIO_PinInGet(ledCathodes[i].port, ledCathodes[i].id);
			if (pinInput != 0) {
				char message[sizeof(messageTemplate)];
				snprintf(
					message, sizeof(messageTemplate), messageTemplate,
					cathodeIndex, ledCathodes[cathodeIndex].port, ledCathodes[cathodeIndex].id,
					i, ledCathodes[i].port, ledCathodes[i].id
				);
				TEST_FAIL_MESSAGE(message);
			}
		}
	}
}
