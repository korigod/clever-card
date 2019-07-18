#include "em_gpio.h"
#include "task.h"

#define IMU_IRQ_GPIO_PORT gpioPortB
#define IMU_IRQ_GPIO_PIN  11

void enableLSM6DS3Interrupt(TaskHandle_t taskToNotify);
