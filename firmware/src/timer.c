#include "em_cmu.h"
#include "em_timer.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "timer.h"
#include "leds.h"


static uint32_t overflowsTillBoot;
static uint32_t timerTicksPeriod;
static uint32_t usecondsBeforeLastTimerOverflow;

static uint8_t currentCathode = 0xFF;  // No current cathode
static uint8_t currentLed = 0xFF;


void handleTimerOverflow(void) {
	TIMER_IntClear(TIMER1, TIMER_IF_OF);
	overflowsTillBoot += 1;

	usecondsBeforeLastTimerOverflow =
		((uint64_t)overflowsTillBoot * timerTicksPeriod * 16 * 1000000) / sysClockFreq;
}


void TIMER1_IRQHandler(void) {
	if (TIMER_IntGet(TIMER1) & TIMER_IF_CC0) {
		switchOffAnodes();

		TIMER_IntClear(TIMER1, TIMER_IF_CC0);

		// We check it here to prevent jitter of the LED on time if
		// both flags are being processed in the same ISR call
		if (TIMER_IntGet(TIMER1) & TIMER_IF_OF) {
			handleTimerOverflow();
		}

		currentLed++;
		if (currentLed >= LED_COUNT) {
			currentLed = 0xFF;
			resetCathodes();
			TIMER_IntDisable(TIMER1, TIMER_IF_CC0);
			return;
		}
		if (leds[currentLed].cathode != currentCathode) {
			resetCathodes();
			enableCathode(leds[currentLed].cathode);
			currentCathode = leds[currentLed].cathode;
		}
		uint8_t ticksToWait = 10;
		uint16_t switchOffTime = (uint16_t)TIMER_CounterGet(TIMER1) + ticksToWait;
		TIMER_CompareSet(TIMER1, 0, switchOffTime);
		GPIO->P[gpioPortA].DOUTSET = 1 << leds[currentLed].anode;

	} else if (TIMER_IntGet(TIMER1) & TIMER_IF_OF) {
		handleTimerOverflow();
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


void triggerLedsUpdateCycle(void) {
	// If we set the flag after we enable the interrupt, then we must be
	// certain that the flag is not set at the moment of enabling, otherwise
	// the ISR will be executed twice: immediately after it's enabled and
	// when the flag is set. So it's easier just to set the flag beforehand.
	TIMER_IntSet(TIMER1, TIMER_IF_CC0);
	TIMER_IntEnable(TIMER1, TIMER_IEN_CC0);
}
