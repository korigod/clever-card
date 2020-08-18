#include <string.h>

#include "unity.h"
#include "fff.h"
#include "test_common.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "imu.h"
#include "lsm6ds3.h"
#include "lsm6ds3_registers.h"


FAKE_VALUE_FUNC4(BaseType_t, xTaskNotifyWait, uint32_t, uint32_t, uint32_t *, TickType_t)
FAKE_VALUE_FUNC4(BaseType_t, xQueueGenericSend, QueueHandle_t, const void * const, TickType_t, const BaseType_t)
FAKE_VALUE_FUNC0(TickType_t, xTaskGetTickCount)


void additionalSuiteSetUp(void) {
	initLSM6DS3(LSM6DS3_SPI_BAUDRATE);
	writeRegister(LSM6DS3_CTRL1_XL, LSM6DS3_ODR_XL_208Hz);
}


void additionalSuiteTearDown(int num_failures) {
	writeRegister(LSM6DS3_CTRL1_XL, LSM6DS3_ODR_XL_POWER_DOWN);
}


TEST_VALUE(0)
TEST_VALUE(1)
TEST_VALUE(2)
void test_imuTaskQueriesIMUAndResultIsSane(int axis) {
	int expectedRawAcceleration;
	if (axis == 2) {
		expectedRawAcceleration = -16384;  // z axis
	} else {
		expectedRawAcceleration = 0;  // x, y axes
	}

	struct ImuRaw result;
	BaseType_t xQueueGenericSend_customFake(
		QueueHandle_t queueHandle,
		const void * const resultPtr,
		TickType_t xTicksToWait,
		const BaseType_t xCopyPosition
	) {
		// Mock of the queue send function will copy
		// the IMU data from the function under test
		result = *(struct ImuRaw *)resultPtr;
		return pdTRUE;
	}
	xQueueGenericSend_fake.custom_fake = xQueueGenericSend_customFake;

	queryIMU(NULL);

	TEST_ASSERT_GREATER_OR_EQUAL(1, xQueueGenericSend_fake.call_count);

	TEST_ASSERT_INT16_WITHIN(400, expectedRawAcceleration, result.acceleration[axis]);
}
