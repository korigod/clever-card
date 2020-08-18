#include "em_gpio.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#define IMU_IRQ_GPIO_PORT gpioPortB
#define IMU_IRQ_GPIO_PIN 13

void enableLSM6DS3Interrupt(TaskHandle_t taskToNotify);
