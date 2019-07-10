#include "em_cmu.h"
#include "em_timer.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "timer.h"


void TIMER1_IRQHandler(void) {
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
