#include "em_cmu.h"
#include "em_timer.h"

#include "common.h"
#include "timer.h"


#ifdef DEBUG
	const uint16_t timerMinTicksToWait = 110 / TIMER1_PRESCALER_VALUE + 2;
#else
	const uint16_t timerMinTicksToWait = 20 / TIMER1_PRESCALER_VALUE + 1;
#endif


void (*timerCallback)(void) = 0;


void TIMER1_IRQHandler(void) {
	if (TIMER_IntGet(TIMER1) & TIMER_IF_CC0) {
		TIMER_IntClear(TIMER1, TIMER_IF_CC0);

		timerCallback();
	}
}


void enableAndTriggerTimerInterrupt(void(*timerInterruptCallback)(void)) {
	if (timerInterruptCallback != 0) {
		timerCallback = timerInterruptCallback;

		// If we set the flag after we enable the interrupt, then we must be
		// certain that the flag is not set at the moment of enabling, otherwise
		// the ISR will be executed twice: immediately after it's enabled and
		// when the flag is set. So it's easier just to set the flag beforehand.
		TIMER_IntSet(TIMER1, TIMER_IF_CC0);
		TIMER_IntEnable(TIMER1, TIMER_IEN_CC0);
	}
}


void disableTimerInterrupt(void) {
	TIMER_IntDisable(TIMER1, TIMER_IF_CC0);
}


void setTimerToWaitTicks(const uint16_t ticks) {
	// Performance of this function is critical, so we disable the assertion in -O3 builds
	ASSERT_DEBUG(ticks >= timerMinTicksToWait);
	const uint16_t switchOffTime = (uint16_t)TIMER_CounterGet(TIMER1) + ticks;
	TIMER_CompareSet(TIMER1, 0, switchOffTime);
}


void initializeTimer(void) {
	CMU_ClockEnable(cmuClock_TIMER1, true);
	NVIC_EnableIRQ(TIMER1_IRQn);

	TIMER_InitCC_TypeDef timerCCInit = {
		.eventCtrl  = timerEventEveryEdge,
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
		.prsOutput  = timerPrsOutputDefault
	};
	TIMER_InitCC(TIMER1, 0, &timerCCInit);

	TIMER_Init_TypeDef timerInit = {
		.enable     = true,
		.debugRun   = false,
		.prescale   = TIMER1_PRESCALER,
		.clkSel     = timerClkSelHFPerClk,
		.count2x    = false,
		.ati        = false,
		.fallAction = timerInputActionNone,
		.riseAction = timerInputActionNone,
		.mode       = timerModeUp,
		.dmaClrAct  = false,
		.quadModeX4 = false,
		.oneShot    = false,
		.sync       = false
	};
	TIMER_Init(TIMER1, &timerInit);
}
