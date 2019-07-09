#include "em_timer.h"

uint32_t sysClockFreq;
uint32_t sysClockFreqKHz;

static uint32_t overflowsTillBoot;
static uint32_t timerTicksPeriod;
static uint32_t usecondsBeforeLastTimerOverflow;

void initMicrosecondsTimer(void);

uint32_t usecondsTillBoot(void);

void delayMicroseconds(uint32_t microseconds);
