#include "em_timer.h"

#define MICROSEC_DELAY_TIMER      TIMER2
#define MICROSEC_DELAY_TIMER_CLK  cmuClock_TIMER2

#define DEBUG_TIMER               TIMER3
#define DEBUG_TIMER_CLK           cmuClock_TIMER3

uint32_t sysClockFreq;
uint32_t ticksPerMicrosecond;

void initMicrosecondsDelayTimer(void);
void delayMicroseconds(uint16_t microseconds);

void initDebugTimer(void);
