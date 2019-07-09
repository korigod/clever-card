#include <limits.h>
#include "em_cmu.h"
#include "em_timer.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "timer.h"


void initMicrosecondsDelayTimer(void) {
	sysClockFreq = CMU_ClockFreqGet(cmuClock_CORE);
	configASSERT(sysClockFreq >= 1000000);
	ticksPerMicrosecond = sysClockFreq / 1000000;

	CMU_ClockEnable(MICROSEC_DELAY_TIMER_CLK, true);
	TIMER_Init_TypeDef timerInit = { .enable     = true,
	                                 .debugRun   = false,
	                                 .prescale   = timerPrescale1,
	                                 .clkSel     = timerClkSelHFPerClk,
	                                 .count2x    = false,
	                                 .ati        = false,
	                                 .fallAction = timerInputActionNone,
	                                 .riseAction = timerInputActionNone,
	                                 .mode       = timerModeUp,
	                                 .dmaClrAct  = false,
	                                 .quadModeX4 = false,
	                                 .oneShot    = false,
	                                 .sync       = false };
	TIMER_Init(MICROSEC_DELAY_TIMER, &timerInit);
}


void initDebugTimer(void) {
	CMU_ClockEnable(DEBUG_TIMER_CLK, true);
	TIMER_Init_TypeDef timerInit = { .enable     = true,
	                                 .debugRun   = false,
	                                 .prescale   = timerPrescale16,
	                                 .clkSel     = timerClkSelHFPerClk,
	                                 .count2x    = false,
	                                 .ati        = false,
	                                 .fallAction = timerInputActionNone,
	                                 .riseAction = timerInputActionNone,
	                                 .mode       = timerModeUp,
	                                 .dmaClrAct  = false,
	                                 .quadModeX4 = false,
	                                 .oneShot    = false,
	                                 .sync       = false };
	TIMER_Init(DEBUG_TIMER, &timerInit);
}


void delayMicroseconds(uint16_t microseconds) {
	uint16_t startTickCount = (uint16_t)TIMER_CounterGet(MICROSEC_DELAY_TIMER);

	uint32_t ticksToWait = microseconds * ticksPerMicrosecond;
	configASSERT(ticksToWait < (uint32_t)USHRT_MAX);
	if (ticksToWait > USHRT_MAX) {
		ticksToWait = USHRT_MAX;
	}

	uint16_t ticksPassed;
	while(1) {
		ticksPassed = (uint16_t)TIMER_CounterGet(MICROSEC_DELAY_TIMER) - startTickCount;
		if (ticksPassed >= ticksToWait) {
			break;
		}
	}
}
