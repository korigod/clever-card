#include "em_gpio.h"
#include "task.h"

#define IMU_IRQ_GPIO_PORT gpioPortB
#define IMU_IRQ_GPIO_PIN  11

static TaskHandle_t _task_to_notify;

void enableLSM6DS3Interrupt(TaskHandle_t taskToNotify);
