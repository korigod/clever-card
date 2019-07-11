#include "em_gpio.h"
#include "task.h"

#define IMU_IRQ_GPIO_PORT gpioPortB
#define IMU_IRQ_GPIO_PIN  11

// This interrupt is assigned to unexisting pin because it is
// triggered by software to wake up LED updating task.
#define LED_IRQ_GPIO_PORT gpioPortB
#define LED_IRQ_GPIO_PIN  15

static TaskHandle_t imuTaskToNotify;

static TaskHandle_t ledTaskToNotify;

void enableLSM6DS3Interrupt(TaskHandle_t taskToNotify);

void enableSoftwareLedInterrupt(TaskHandle_t taskToNotify);
