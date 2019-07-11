#include "em_gpio.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "leds.h"
#include "timer.h"
#include "gpio_irq.h"

struct Led leds[LED_COUNT] = { { 3, 10, red }, { 3, 11, green },
                               { 4, 10, red }, { 4, 11, green } };


void initLeds(void) {
	// Set mode of anode lines (0-5, 8-10) to push-pull
	GPIO->P[gpioPortA].MODEL = 0x00444444;
	GPIO->P[gpioPortA].MODEH = 0x00000444;
}


// Set mode of all the LED cathode lines to DISABLED
void resetCathodes() {
	// The whole high half of port E (pins 8—15) are cathode lines
	GPIO->P[gpioPortE].MODEH = 0x0;

	// Only pins 2—5 of port F are cathodes, so we preserve the mode of the others
	GPIO->P[gpioPortF].MODEL &= 0xFF0000FF;
}


// Cathode lines are portE[8-15] and portF[2-5]
void enableCathode(uint8_t cathode) {
	if (cathode < 8) {
		GPIO_PinModeSet(gpioPortF, cathode, gpioModeWiredAnd, 0);
	} else {
		GPIO_PinModeSet(gpioPortE, cathode, gpioModeWiredAnd, 0);
	}
}


void switchOffAnodes() {
	GPIO->P[gpioPortA].DOUTCLR = 0xFFFF;
}


void updateLeds(void * pvParameters) {
	TickType_t previousWakeTime = xTaskGetTickCount();
	for ( ;; ) {
		vTaskDelayUntil(&previousWakeTime, pdMS_TO_TICKS(1000 / LED_FPS));
		triggerLedsUpdateCycle();
	}
}
