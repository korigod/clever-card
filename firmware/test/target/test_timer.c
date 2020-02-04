#include <string.h>
#include <stdio.h>
#include "unity.h"
#include "fff.h"
#include "em_timer.h"

#include "test_common.h"
#include "timer.h"

TEST_FILE("timer.c")


volatile uint32_t callbackCalledCount = 0;


void setUp(void) {
	callbackCalledCount = 0;
	initializeTimer();
}

void tearDown(void) {
	NVIC_DisableIRQ(TIMER1_IRQn);
	TIMER_Reset(TIMER1);
}


void test_timerDoesNotMissEvenMinimalInterval(void) {
	setTimerToWaitTicks(timerMinTicksToWait);
	const uint16_t timerCounter = TIMER_CounterGet(TIMER1);
	const uint16_t timerCompareValue = TIMER_CaptureGet(TIMER1, 0);
	TEST_ASSERT_LESS_OR_EQUAL_UINT16_MESSAGE(
		timerMinTicksToWait, timerCompareValue - timerCounter,
		"Timer missed target interval while was setting it!"
	);
}


volatile uint16_t callbackExecutedAtTicks = 0;

void timerMinimalWaitDoesNotExceedDeclaredValue_testCallback(void) {
	if (callbackCalledCount == 1) {
		callbackExecutedAtTicks = TIMER_CounterGet(TIMER1);
	}
	callbackCalledCount += 1;
}

void test_timerMinimalWaitDoesNotExceedDeclaredValue(void) {
	enableAndTriggerTimerInterrupt(timerMinimalWaitDoesNotExceedDeclaredValue_testCallback);

	// The first timer compare event will be processed in ISR here.
	TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, callbackCalledCount, "Unexpectedly, timer has not fired the interrupt.");

	const uint16_t timerSetAtTicks = TIMER_CounterGet(TIMER1);
	setTimerToWaitTicks(timerMinTicksToWait);

	while (callbackCalledCount < 2) {
	}

	TEST_ASSERT_LESS_OR_EQUAL_UINT16_MESSAGE(
		timerMinTicksActuallyWait, callbackExecutedAtTicks - timerSetAtTicks,
		"Timer exceeded its minimal time from setting it to wait for ticks to executing the callback function."
	);
}
