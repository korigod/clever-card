#include "imu.h"
#include "gpio_irq.h"


void GPIO_ODD_IRQHandler(void) 
{
	if (GPIO_IntGet() & (1 << IMU_IRQ_GPIO_PIN)) {
		GPIO_IntClear(1 << IMU_IRQ_GPIO_PIN);
		xTaskNotifyFromISR(imuTaskToNotify, 0, eNoAction, NULL);
	}
}


void enableLSM6DS3Interrupt(TaskHandle_t taskToNotify) {
	imuTaskToNotify = taskToNotify;

	GPIO_PinModeSet(IMU_IRQ_GPIO_PORT,
	                IMU_IRQ_GPIO_PIN,
	                gpioModeInput,
	                1); // 1 for pull-up

	// Priority 6 of 7 (medium priority of three RTOS-enabled, which are 5–7)
	NVIC_SetPriority(GPIO_ODD_IRQn, 6);

	NVIC_EnableIRQ(GPIO_ODD_IRQn);

	GPIO_ExtIntConfig(IMU_IRQ_GPIO_PORT,
	                  0,                // unused arg
	                  IMU_IRQ_GPIO_PIN, // interrupt line (= pin)
	                  true,             // rising edge interrupt
	                  false,            // falling edge interrupt
	                  true);            // enable
}
