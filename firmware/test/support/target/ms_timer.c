#include "em_cmu.h"
#include "ms_timer.h"
#include "unity.h"


uint32_t startTime;
uint32_t stopTime;


void SysTick_Handler(void)
{
	msTicks++;
}


void unityExecTimeStart(void) {
	startTime = msTicks;
}

void unityExecTimeStop(void) {
	stopTime = msTicks;
}

void unityPrintExecTime(void) {
	UnityPrint(" (");
	UNITY_COUNTER_TYPE execTimeMs = (stopTime - startTime);
	UnityPrintNumberUnsigned(execTimeMs);
	UnityPrint(" ms)");
}
