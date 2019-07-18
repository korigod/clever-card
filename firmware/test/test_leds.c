#include "unity.h"
#include "fff.h"
#include "leds.h"

#include "em_gpio.h"


FAKE_VALUE_FUNC(TickType_t, xTaskGetTickCount);
FAKE_VOID_FUNC(vTaskDelayUntil, TickType_t * const, const TickType_t);
FAKE_VOID_FUNC(triggerLedsUpdateCycle);

GPIO_TypeDef gpio;
GPIO_TypeDef * gpioBase = &gpio;

void setUp(void) {
}

void tearDown(void) {
}

void test_enableCathode_portF(void) {
	enableCathode(5);
	TEST_ASSERT_EQUAL_HEX32(0x8 << 5 * 4, gpio.P[gpioPortF].MODEL);
}

void test_enableCathode_portE(void) {
	enableCathode(8);
	TEST_ASSERT_EQUAL_HEX32(0x8 << (8 - 8) * 4, gpio.P[gpioPortE].MODEH);
}
