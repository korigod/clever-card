#include <string.h>
#include <stdio.h>
#include "em_gpio.h"
#include "unity.h"
#include "fff.h"

#include "board.h"
#include "leds.h"

TEST_FILE("led_driver.c")

FAKE_VOID_FUNC2(CMU_ClockEnable, int, bool);

extern uint8_t currentLedIndex;
extern const uint8_t INVALID_LED;
extern uint8_t ledOutputsLatched[LED_COUNT];
extern uint8_t ledOutputs[LED_COUNT];
extern bool ledPinsAreEqual(struct LedPins a, struct LedPins b);


GPIO_TypeDef gpio;
GPIO_TypeDef *gpioBase = &gpio;


void setAllLedOutputs(uint8_t output_value) {
	memset(ledOutputsLatched, output_value, LED_COUNT);
	memset(ledOutputs, output_value, LED_COUNT);
}


void setUp(void) {
	initializeLeds();
}


void tearDown(void) {
}


void test_initializeLeds_ledOutputsLatchedAreCleared(void) {
	setAllLedOutputs(0xAA);

	initializeLeds();

	TEST_ASSERT_EACH_EQUAL_UINT8(0, ledOutputsLatched, LED_COUNT);
}

void test_initializeLeds_ledOutputsAreCleared(void) {
	setAllLedOutputs(0xAA);

	initializeLeds();

	TEST_ASSERT_EACH_EQUAL_UINT8(0, ledOutputs, LED_COUNT);
}

void test_initializeLeds_afterInitialization_noLedIsChosenToSwitchOn(void) {
	setAllLedOutputs(0xAA);
	initializeLeds();

	struct PrepareNextLedResult result = prepareNextLed(true);

	TEST_ASSERT_EQUAL(NO_MORE_LEDS, result.status);
}


void test_prepareNextLed_whenCurrentLedInvalid_firstLedSwitchedOn(void) {
	currentLedIndex = INVALID_LED;
	ledOutputsLatched[0] = 30;

	struct PrepareNextLedResult result = prepareNextLed(true);

	TEST_ASSERT_EQUAL(SUCCESS, result.status);
	TEST_ASSERT(ledPinsAreEqual(result.led, ledPins[0]));
}

void test_prepareNextLed_zeroBrightnessLedSkipped(void) {
	currentLedIndex = INVALID_LED;
	ledOutputsLatched[1] = 30;

	struct PrepareNextLedResult result = prepareNextLed(true);

	TEST_ASSERT_EQUAL(SUCCESS, result.status);
	TEST_ASSERT(ledPinsAreEqual(result.led, ledPins[1]));
}

void test_prepareNextLed_whenLoopEnabled_loops(void) {
	currentLedIndex = 50;
	ledOutputsLatched[1] = 30;

	struct PrepareNextLedResult result = prepareNextLed(true);

	TEST_ASSERT_EQUAL(SUCCESS, result.status);
	TEST_ASSERT(ledPinsAreEqual(result.led, ledPins[1]));
}

void test_prepareNextLed_whenLoopDisabled_returnsNoMoreLeds(void) {
	currentLedIndex = 50;
	ledOutputsLatched[1] = 30;

	struct PrepareNextLedResult result = prepareNextLed(false);

	TEST_ASSERT_EQUAL(NO_MORE_LEDS, result.status);
}

void test_prepareNextLed_whenNoMoreLeds_currentLedIsSetInvalid(void) {
	currentLedIndex = 50;
	ledOutputsLatched[1] = 30;

	struct PrepareNextLedResult result = prepareNextLed(false);

	TEST_ASSERT_EQUAL(INVALID_LED, currentLedIndex);
}

void test_prepareNextLed_whenAllLedsHaveZeroBrightness_noInfiniteLoop(void) {
	currentLedIndex = INVALID_LED;

	struct PrepareNextLedResult result = prepareNextLed(true);

	// The test will hang if there is an infinite loop
	TEST_ASSERT_EQUAL(NO_MORE_LEDS, result.status);
}
