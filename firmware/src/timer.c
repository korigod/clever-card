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


void delayMicroseconds(uint32_t microseconds) {
	uint32_t startTickCount = TIMER_CounterGet(MICROSEC_DELAY_TIMER);
	uint32_t ticksToWait = microseconds * ticksPerMicrosecond;
	uint32_t finishTickCount = startTickCount + ticksToWait;

	while(1) {
		if (finishTickCount < TIMER_CounterGet(MICROSEC_DELAY_TIMER)) {
			break;
		}

		// If the timer has overflowed, we reduce the target tick count
		// by the timer period.
		if (TIMER_IntGet(MICROSEC_DELAY_TIMER) & TIMER_IF_OF) {

			TIMER_IntClear(MICROSEC_DELAY_TIMER, TIMER_IF_OF);

			uint32_t timerTopValue = TIMER_TopGet(MICROSEC_DELAY_TIMER);

			if (finishTickCount > timerTopValue) {
				finishTickCount = finishTickCount - timerTopValue;
			} else {
				break;
			}
		}
	}
}
