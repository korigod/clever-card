#include <stdio.h>
#include "em_gpio.h"
#include "unity.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "test_common.h"
#include "board.h"
#include "lsm6ds3.h"
#include "lsm6ds3_registers.h"
#include "gpio_irq.h"

TEST_FILE("tasks.c")
TEST_FILE("timer.c")
TEST_FILE("port.c")
TEST_FILE("list.c")
TEST_FILE("lists.c")
TEST_FILE("debug_hooks.c")


#define ACCEL_MAX_ERROR_G 0.025
#define GYRO_MAX_ERROR_DPS 5.0


void additionalSuiteSetUp(void) {
	// Disable FreeRTOS scheduler in SysTick handler
	vTaskSuspendAll();
}


void setUp(void) {
	initLSM6DS3(LSM6DS3_SPI_BAUDRATE);
}


int suiteTearDown(int num_failures) {
	writeRegister(LSM6DS3_CTRL1_XL, LSM6DS3_ODR_XL_POWER_DOWN);
	writeRegister(LSM6DS3_CTRL2_G, LSM6DS3_ODR_G_POWER_DOWN);

	GPIO_DriveModeSet(gpioPortA, gpioDriveModeLowest);
	GPIO_PinModeSet(gpioPortA, 10, gpioModePushPullDrive, 1);

	if (num_failures == 0) {
		GPIO_PinModeSet(gpioPortE, 8, gpioModeWiredAnd, 0);  // Green LED
	} else {
		GPIO_PinModeSet(gpioPortE, 9, gpioModeWiredAnd, 0);  // Red LED
	}
}


void test_whoAmIRegisterReadsCorrectly(void) {
	TEST_ASSERT_EQUAL_HEX8(LSM6DS3_WHO_AM_I_VALUE, readRegister(LSM6DS3_WHO_AM_I_REG));
}


void test_whoAmIRegisterReadsAreStable(void) {
	int incorrectReadsCount = 0;
	for (int i = 0; i < 1000; i++) {
		if (readRegister(LSM6DS3_WHO_AM_I_REG) != LSM6DS3_WHO_AM_I_VALUE) {
			incorrectReadsCount++;
		}
	}
	TEST_ASSERT_EQUAL_INT(0, incorrectReadsCount);
}


void test_registerWritesThenReadsCorrectly(void) {
	const uint8_t testRegisterAddress = LSM6DS3_INT2_CTRL;
	int incorrectReadsCount = 0;
	for (uint8_t i = 0; i < 255; i++) {
		writeRegister(testRegisterAddress, i);
		if (readRegister(testRegisterAddress) != i) {
			incorrectReadsCount++;
		}
	}
	TEST_ASSERT_EQUAL_INT(0, incorrectReadsCount);
}


TEST_VALUE(1000)
TEST_VALUE(10000)
TEST_VALUE(100000)
TEST_VALUE(350000)
TEST_VALUE(1000000)
TEST_VALUE(1750000)
TEST_VALUE(3500000)
void test_differentBaudratesWorkCorrectly(int baudrate) {
	initLSM6DS3(baudrate);
	TEST_ASSERT_EQUAL_HEX8(LSM6DS3_WHO_AM_I_VALUE, readRegister(LSM6DS3_WHO_AM_I_REG));
}


void test_accelXValueIsSane(void) {
	writeRegister(LSM6DS3_CTRL1_XL, LSM6DS3_ODR_XL_208Hz);

	while((readRegister(LSM6DS3_STATUS_REG) & LSM6DS3_XLDA_DATA_AVAIL) == 0) {
		// Wait until accelerometer data is ready
	}

	int16_t accelX = readRegister(LSM6DS3_OUTX_L_XL) | readRegister(LSM6DS3_OUTX_H_XL) << 8;

	// Default acceleration limits are -2g/+2g, so 1g is max_int16 / 2
	float xFloat = accelX / 16384.0f;

	TEST_ASSERT_FLOAT_WITHIN(ACCEL_MAX_ERROR_G, 0, xFloat);
}


void test_accelYValueIsSane(void) {
	writeRegister(LSM6DS3_CTRL1_XL, LSM6DS3_ODR_XL_208Hz);

	while((readRegister(LSM6DS3_STATUS_REG) & LSM6DS3_XLDA_DATA_AVAIL) == 0) {
		// Wait until accelerometer data is ready
	}

	int16_t accelY = readRegister(LSM6DS3_OUTY_L_XL) | readRegister(LSM6DS3_OUTY_H_XL) << 8;

	// Default acceleration limits are -2g/+2g, so 1g is max_int16 / 2
	float yFloat = accelY / 16384.0f;

	TEST_ASSERT_FLOAT_WITHIN(ACCEL_MAX_ERROR_G, 0, yFloat);
}


void test_accelZValueIsSane(void) {
	writeRegister(LSM6DS3_CTRL1_XL, LSM6DS3_ODR_XL_208Hz);

	while((readRegister(LSM6DS3_STATUS_REG) & LSM6DS3_XLDA_DATA_AVAIL) == 0) {
		// Wait until accelerometer data is ready
	}

	int16_t accelZ = readRegister(LSM6DS3_OUTZ_L_XL) | readRegister(LSM6DS3_OUTZ_H_XL) << 8;

	// Default acceleration limits are -2g/+2g, so 1g is max_int16 / 2
	float zFloat = accelZ / 16384.0f;

	TEST_ASSERT_FLOAT_WITHIN(ACCEL_MAX_ERROR_G, -1, zFloat);
}


void test_gyroXValueIsSane(void) {
	writeRegister(LSM6DS3_CTRL2_G, LSM6DS3_ODR_G_208Hz);

	for (int i = 0; i < 10; i++) {
		while((readRegister(LSM6DS3_STATUS_REG) & LSM6DS3_GDA_DATA_AVAIL) == 0) {
			// Wait until gyro data is ready
		}
		readRegister(LSM6DS3_OUTX_H_G);  // Reset Data Ready flag and wait for next measurement
	}

	int16_t angRateX = readRegister(LSM6DS3_OUTX_L_G) | readRegister(LSM6DS3_OUTX_H_G) << 8;

	// Default acceleration limits are -2g/+2g, so 1g is max_int16 / 2
	float xRateDegreesPerSecond = angRateX / 131.072f;

	TEST_ASSERT_FLOAT_WITHIN(GYRO_MAX_ERROR_DPS, 0, xRateDegreesPerSecond);
}


void test_gyroYValueIsSane(void) {
	writeRegister(LSM6DS3_CTRL2_G, LSM6DS3_ODR_G_208Hz);

	while((readRegister(LSM6DS3_STATUS_REG) & LSM6DS3_GDA_DATA_AVAIL) == 0) {
		// Wait until accelerometer data is ready
	}

	int16_t angRateY = readRegister(LSM6DS3_OUTY_L_G) | readRegister(LSM6DS3_OUTY_H_G) << 8;

	// Default acceleration limits are -2g/+2g, so 1g is max_int16 / 2
	float yRateDegreesPerSecond = angRateY / 131.072f;

	TEST_ASSERT_FLOAT_WITHIN(GYRO_MAX_ERROR_DPS, 0, yRateDegreesPerSecond);
}


void test_gyroZValueIsSane(void) {
	writeRegister(LSM6DS3_CTRL2_G, LSM6DS3_ODR_G_208Hz);

	while((readRegister(LSM6DS3_STATUS_REG) & LSM6DS3_GDA_DATA_AVAIL) == 0) {
		// Wait until accelerometer data is ready
	}

	int16_t angRateZ = readRegister(LSM6DS3_OUTZ_L_G) | readRegister(LSM6DS3_OUTZ_H_G) << 8;

	// Default acceleration limits are -2g/+2g, so 1g is max_int16 / 2
	float zRateDegreesPerSecond = angRateZ / 131.072f;

	TEST_ASSERT_FLOAT_WITHIN(GYRO_MAX_ERROR_DPS, 0, zRateDegreesPerSecond);
}


void test_accelerometerSelfTestPasses(void) {
	writeRegister(LSM6DS3_CTRL1_XL, LSM6DS3_ODR_XL_52Hz);

	for (int i = 0; i < 10; i++) {
		while((readRegister(LSM6DS3_STATUS_REG) & LSM6DS3_XLDA_DATA_AVAIL) == 0) {
			// Wait until accelerometer data is ready
		}
		readRegister(LSM6DS3_OUTX_H_XL);  // Reset Data Ready flag and wait for next measurement
	}

	int16_t accelX = readRegister(LSM6DS3_OUTX_L_XL) | readRegister(LSM6DS3_OUTX_H_XL) << 8;
	int16_t accelY = readRegister(LSM6DS3_OUTY_L_XL) | readRegister(LSM6DS3_OUTY_H_XL) << 8;
	int16_t accelZ = readRegister(LSM6DS3_OUTZ_L_XL) | readRegister(LSM6DS3_OUTZ_H_XL) << 8;

	writeRegister(LSM6DS3_CTRL5_C, LSM6DS3_ST_XL_POS_SIGN_TEST);

	for (int i = 0; i < 10; i++) {
		while((readRegister(LSM6DS3_STATUS_REG) & LSM6DS3_XLDA_DATA_AVAIL) == 0) {
			// Wait until accelerometer data is ready
		}
		readRegister(LSM6DS3_OUTX_H_XL);  // Reset Data Ready flag and wait for next measurement
	}

	int16_t accelXselfTest = readRegister(LSM6DS3_OUTX_L_XL) | readRegister(LSM6DS3_OUTX_H_XL) << 8;
	int16_t accelYselfTest = readRegister(LSM6DS3_OUTY_L_XL) | readRegister(LSM6DS3_OUTY_H_XL) << 8;
	int16_t accelZselfTest = readRegister(LSM6DS3_OUTZ_L_XL) | readRegister(LSM6DS3_OUTZ_H_XL) << 8;

	writeRegister(LSM6DS3_CTRL5_C, LSM6DS3_ST_XL_NORMAL_MODE);

	TEST_ASSERT_INT16_WITHIN(13000, 14500, accelXselfTest - accelX);
	TEST_ASSERT_INT16_WITHIN(13000, 14500, accelYselfTest - accelY);
	TEST_ASSERT_INT16_WITHIN(13000, 14500, accelZselfTest - accelZ);
}


void test_gyroSelfTestPasses(void) {
	writeRegister(LSM6DS3_CTRL2_G, LSM6DS3_ODR_G_52Hz | LSM6DS3_FS_G_2000dps);

	for (int i = 0; i < 10; i++) {
		while((readRegister(LSM6DS3_STATUS_REG) & LSM6DS3_GDA_DATA_AVAIL) == 0) {
			// Wait until gyro data is ready
		}
		readRegister(LSM6DS3_OUTX_H_G);  // Reset Data Ready flag and wait for next measurement
	}

	int16_t gyroX = readRegister(LSM6DS3_OUTX_L_G) | readRegister(LSM6DS3_OUTX_H_G) << 8;
	int16_t gyroY = readRegister(LSM6DS3_OUTY_L_G) | readRegister(LSM6DS3_OUTY_H_G) << 8;
	int16_t gyroZ = readRegister(LSM6DS3_OUTZ_L_G) | readRegister(LSM6DS3_OUTZ_H_G) << 8;

	writeRegister(LSM6DS3_CTRL5_C, LSM6DS3_ST_G_POS_SIGN_TEST);

	for (int i = 0; i < 10; i++) {
		while((readRegister(LSM6DS3_STATUS_REG) & LSM6DS3_GDA_DATA_AVAIL) == 0) {
			// Wait until gyro data is ready
		}
		readRegister(LSM6DS3_OUTX_H_G);  // Reset Data Ready flag and wait for next measurement
	}

	int16_t gyroXselfTest = readRegister(LSM6DS3_OUTX_L_G) | readRegister(LSM6DS3_OUTX_H_G) << 8;
	int16_t gyroYselfTest = readRegister(LSM6DS3_OUTY_L_G) | readRegister(LSM6DS3_OUTY_H_G) << 8;
	int16_t gyroZselfTest = readRegister(LSM6DS3_OUTZ_L_G) | readRegister(LSM6DS3_OUTZ_H_G) << 8;

	writeRegister(LSM6DS3_CTRL5_C, LSM6DS3_ST_G_NORMAL_MODE);

	TEST_ASSERT_INT16_WITHIN(3929, 6071, gyroXselfTest - gyroX);
	TEST_ASSERT_INT16_WITHIN(3929, 6071, gyroYselfTest - gyroY);
	TEST_ASSERT_INT16_WITHIN(3929, 6071, gyroZselfTest - gyroZ);
}


void test_accelDataReadyInterruptWorks(void) {
	GPIO_PinModeSet(
		IMU_IRQ_GPIO_PORT,
		IMU_IRQ_GPIO_PIN,
		gpioModeInputPull,
		0
	);

	writeRegister(LSM6DS3_CTRL1_XL, LSM6DS3_ODR_XL_208Hz);
	writeRegister(LSM6DS3_INT1_CTRL, LSM6DS3_INT1_DRDY_XL_ENABLED);

	int errorsCount = 0;
	for (int i = 0; i < 10; i++) {
		while((readRegister(LSM6DS3_STATUS_REG) & LSM6DS3_XLDA_DATA_AVAIL) == 0) {
			// Wait until accelerometer data is ready
		}
		if (GPIO_PinInGet(IMU_IRQ_GPIO_PORT, IMU_IRQ_GPIO_PIN) != 1) {
			errorsCount++;
		}
		readRegister(LSM6DS3_OUTX_H_XL);  // Reset Data Ready flag
		if (GPIO_PinInGet(IMU_IRQ_GPIO_PORT, IMU_IRQ_GPIO_PIN) != 0) {
			errorsCount++;
		}
	}

	GPIO_PinModeSet(
		IMU_IRQ_GPIO_PORT,
		IMU_IRQ_GPIO_PIN,
		gpioModeDisabled,
		0
	);

	TEST_ASSERT_EQUAL_INT(0, errorsCount);
}
