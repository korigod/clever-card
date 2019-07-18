#include "em_timer.h"

uint32_t sysClockFreq;
uint32_t sysClockFreqKHz;


void initMicrosecondsTimer(void);

uint32_t usecondsTillBoot(void);

void delayMicroseconds(uint32_t microseconds);

void triggerLedsUpdateCycle(void);
