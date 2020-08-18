#include <stdio.h>
#include "em_system.h"
#include "unity.h"

#include "test_common.h"


enum supportedMCUs {
	MCU_NOT_DEFINED,
	MCU_EFM32TG232F32,
	MCU_EFM32LG232F256
};

#ifdef EFM32LG232F256
	enum supportedMCUs mcu = MCU_EFM32LG232F256;
#elif EFM32TG232F32
	enum supportedMCUs mcu = MCU_EFM32TG232F32;
#else
	enum supportedMCUs mcu = MCU_NOT_DEFINED;
#endif


void test_mcuIsSupported(void) {
	TEST_ASSERT_NOT_EQUAL(MCU_NOT_DEFINED, mcu);
}


void test_mcuFamilyMatches(void) {
	SYSTEM_PartFamily_TypeDef expectedPartFamily = 0;
	if (mcu == MCU_EFM32LG232F256) {
		expectedPartFamily = systemPartFamilyEfm32Leopard;
	} else if (mcu == MCU_EFM32TG232F32) {
		expectedPartFamily = systemPartFamilyEfm32Tiny;
	}
	TEST_ASSERT_EQUAL_INT(expectedPartFamily, SYSTEM_GetFamily());
}


void test_mcuPartNumberMatches(void) {
	uint16_t expectedPartNumber = 0;
	if (mcu == MCU_EFM32LG232F256 || mcu == MCU_EFM32TG232F32) {
		expectedPartNumber = 232;
	}
	TEST_ASSERT_EQUAL_UINT16(expectedPartNumber, SYSTEM_GetPartNumber());
}


void test_mcuRamSizeMatches(void) {
	uint16_t expectedRamSize = 0;
	if (mcu == MCU_EFM32LG232F256) {
		expectedRamSize = 32;
	} else if (mcu == MCU_EFM32TG232F32) {
		expectedRamSize = 4;
	}
	TEST_ASSERT_EQUAL_UINT16(expectedRamSize, SYSTEM_GetSRAMSize());
}


void test_mcuFlashSizeMatches(void) {
	uint16_t expectedFlashSize = 0;
	if (mcu == MCU_EFM32LG232F256) {
		expectedFlashSize = 256;
	} else if (mcu == MCU_EFM32TG232F32) {
		expectedFlashSize = 32;
	}
	TEST_ASSERT_EQUAL_UINT16(expectedFlashSize, SYSTEM_GetFlashSize());
}
