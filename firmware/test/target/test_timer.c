#include <string.h>
#include <stdio.h>
#include "unity.h"
#include "fff.h"
#include "em_timer.h"

#include "test_common.h"
#include "timer.h"

TEST_FILE("timer.c")


void setUp(void) {
	initializeTimer();
}


void tearDown(void) {
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
