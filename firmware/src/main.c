#include "em_chip.h"

#include "timer.h"
#include "leds.h"

int main(void)
{
	CHIP_Init();

	initializeLeds();
	enableAndTriggerTimerInterrupt(switchToNextLed);

	while (1) {
	}
}
