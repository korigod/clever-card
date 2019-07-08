/***************************************************************************/ /**
 * @file
 * @brief FreeRTOS Blink Demo for Energy Micro EFM32LG_STK3600 Starter Kit
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#define STACK_SIZE_FOR_TASK (configMINIMAL_STACK_SIZE + 10)
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1)

typedef struct
{
	/* LED blink interval in milliseconds */
	portTickType delay;
	/* GPIO # */
	int ledNo;
} TaskParams_t;

static void LedBlink(void *pParameters)
{
	TaskParams_t *pData = (TaskParams_t *)pParameters;
	const portTickType delay = pData->delay;

	for (;;)
	{
	GPIO_PinOutToggle(gpioPortE, pData->ledNo);
	vTaskDelay(delay);
	}
}

int main(void)
{
	CHIP_Init();

	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortE, 3, gpioModePushPull, 0);

	static TaskParams_t parametersToTask1 = {pdMS_TO_TICKS(1000), 2};
	static TaskParams_t parametersToTask2 = {pdMS_TO_TICKS(500), 3};

	xTaskCreate(LedBlink, (const char *)"LedBlink1", STACK_SIZE_FOR_TASK, &parametersToTask1, TASK_PRIORITY, NULL);
	xTaskCreate(LedBlink, (const char *)"LedBlink2", STACK_SIZE_FOR_TASK, &parametersToTask2, TASK_PRIORITY, NULL);

	vTaskStartScheduler();

	while (1) {
	}
}
