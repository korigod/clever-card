#include "em_cmu.h"
#include "em_timer.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "timer.h"


void TIMER1_IRQHandler(void) {
	if (TIMER_IntGet(TIMER1) & TIMER_IF_CC0) {
		if (timerCallback != NULL) {
			timerCallback();
			timerCallback = NULL;
		}
		TIMER_IntClear(TIMER1, TIMER_IF_CC0);
		TIMER_IntDisable(TIMER1, TIMER_IEN_CC0);
	}
	if (TIMER_IntGet(TIMER1) & TIMER_IF_OF) {
		TIMER_IntClear(TIMER1, TIMER_IF_OF);
		overflowsTillBoot += 1;

		usecondsBeforeLastTimerOverflow =
			((uint64_t)overflowsTillBoot * timerTicksPeriod * 16 * 1000000) / sysClockFreq;
	}
}


void initMicrosecondsTimer(void) {
	sysClockFreq = CMU_ClockFreqGet(cmuClock_CORE);
	sysClockFreqKHz = sysClockFreq / 1000;
	usecondsBeforeLastTimerOverflow = 0;
	overflowsTillBoot = 0;

	CMU_ClockEnable(cmuClock_TIMER1, true);
	TIMER_IntEnable(TIMER1, TIMER_IEN_OF);
	NVIC_EnableIRQ(TIMER1_IRQn);

	// Timer is inialized with max top value (0xFFFF) by default
	timerTicksPeriod = TIMER_TopGet(TIMER1) + 1;

	TIMER_InitCC_TypeDef timerCCInit = { .eventCtrl  = timerEventEveryEdge,
	                                     .edge       = timerEdgeNone,
	                                     .prsSel     = 0,
	                                     .cufoa      = timerOutputActionNone,
	                                     .cofoa      = timerOutputActionNone,
	                                     .cmoa       = timerOutputActionNone,
	                                     .mode       = timerCCModeCompare,
	                                     .filter     = false,
	                                     .prsInput   = false,
	                                     .coist      = false,
	                                     .outInvert  = false,
	                                     .prsOutput  = timerPrsOutputDefault };
	TIMER_InitCC(TIMER1, 0, &timerCCInit);

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
	TIMER_Init(TIMER1, &timerInit);
}


uint32_t usecondsTillBoot(void) {
	// Pay attention to uint32_t overflow as well as to the loss
	// of precision with low clock frequencies when editing this

	uint32_t sinceLastTimerOverflow =
		(TIMER_CounterGet(TIMER1) * 16 * 1000) / sysClockFreqKHz;

	return usecondsBeforeLastTimerOverflow + sinceLastTimerOverflow;
}


void delayMicroseconds(uint32_t microseconds) {
	uint32_t startTime = usecondsTillBoot();

	while(usecondsTillBoot() - startTime < microseconds) {
	}
}


TimerCallbackReturn_t callAfterDelay(void (*callback)(void), uint32_t microseconds) {
	if (timerCallback != NULL) {
		return previousCallbackPending;
	}
	uint32_t ticksToWait = (microseconds * sysClockFreqKHz) / 1000 / 16;
	if (ticksToWait >= timerTicksPeriod) {
		return delayTooLong;
	} else if (ticksToWait < 5) {
		return delayTooShort;
	}
	timerCallback = callback;
	TIMER_IntClear(TIMER1, TIMER_IF_CC0);
	uint16_t compareValue = (uint16_t)TIMER_CounterGet(TIMER1) + (uint16_t)ticksToWait - 2;
	TIMER_CompareSet(TIMER1, 0, compareValue);
	TIMER_IntEnable(TIMER1, TIMER_IEN_CC0);
	return success;
}
