#include "em_timer.h"

uint32_t sysClockFreq;
uint32_t sysClockFreqKHz;

static uint32_t overflowsTillBoot;
static uint32_t timerTicksPeriod;
static uint32_t usecondsBeforeLastTimerOverflow;
static void (*timerCallback)(void);

extern uint8_t currentCathode;
extern uint8_t currentLed;

void initMicrosecondsTimer(void);

uint32_t usecondsTillBoot(void);

void delayMicroseconds(uint32_t microseconds);

void triggerLedsUpdateCycle(void);
