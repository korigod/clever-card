#include "Include/em_device.h"
#include "em_gpio.h"

#undef GPIO_BASE
#define GPIO_BASE gpioBase
GPIO_TypeDef * gpioBase;

#undef _GPIO_P_DOUTCLR_MASK
#undef _GPIO_P_DOUTSET_MASK
