#include "em_timer.h"

uint32_t sysClockFreq;
uint32_t sysClockFreqKHz;

static uint32_t overflowsTillBoot;
static uint32_t timerTicksPeriod;
static uint32_t usecondsBeforeLastTimerOverflow;
static void (*timerCallback)(void);

void initMicrosecondsTimer(void);

uint32_t usecondsTillBoot(void);

void delayMicroseconds(uint32_t microseconds);

typedef enum TimerCallbackReturnValue {
	success = 2,
	delayTooLong,
	delayTooShort,
	previousCallbackPending
} TimerCallbackReturn_t;
// This function is non-blocking. The callback will be executed in the context of the
// ISR of highest priority. Max delay is about 74000 us if clock frequency is 14 MHz.
TimerCallbackReturn_t callAfterDelay(void (*callback)(void), uint32_t microseconds);
