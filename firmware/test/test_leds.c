#include "unity.h"
#include "fff.h"
#include "leds.h"

#include "em_gpio.h"
// #include "mock_em_gpio.h"
// #include "mock_timer.h"
// #include "mock_FreeRTOSConfig.h"
// #include "mock_FreeRTOS.h"
// #include "mock_task.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

FAKE_VALUE_FUNC(TickType_t, xTaskGetTickCount);
FAKE_VOID_FUNC(vTaskDelayUntil, TickType_t * const, const TickType_t);
FAKE_VOID_FUNC(triggerLedsUpdateCycle);
FAKE_VOID_FUNC(GPIO_PinModeSet, GPIO_Port_TypeDef, unsigned int, GPIO_Mode_TypeDef, unsigned int);

void setUp(void) {
}

void tearDown(void) {
}

void test_enableCathode_portF(void) {
	enableCathode(5);

	TEST_ASSERT_EQUAL(1, GPIO_PinModeSet_fake.call_count);

	TEST_ASSERT_EQUAL(gpioPortF,        GPIO_PinModeSet_fake.arg0_val);
	TEST_ASSERT_EQUAL(5,                GPIO_PinModeSet_fake.arg1_val);
	TEST_ASSERT_EQUAL(gpioModeWiredAnd, GPIO_PinModeSet_fake.arg2_val);
	TEST_ASSERT_EQUAL(0,                GPIO_PinModeSet_fake.arg3_val);
}

void test_enableCathode_portE(void) {
	enableCathode(8);

	TEST_ASSERT_EQUAL(1, GPIO_PinModeSet_fake.call_count);

	TEST_ASSERT_EQUAL(gpioPortE,        GPIO_PinModeSet_fake.arg0_val);
	TEST_ASSERT_EQUAL(8,                GPIO_PinModeSet_fake.arg1_val);
	TEST_ASSERT_EQUAL(gpioModeWiredAnd, GPIO_PinModeSet_fake.arg2_val);
	TEST_ASSERT_EQUAL(0,                GPIO_PinModeSet_fake.arg3_val);
}
