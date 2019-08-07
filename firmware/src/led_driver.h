#ifndef LED_DRIVER_H_INC
#define LED_DRIVER_H_INC

#include "em_gpio.h"

struct LedAnode {
	GPIO_Port_TypeDef port : 4;
	uint8_t id : 4;
};

struct LedCathode {
	GPIO_Port_TypeDef port : 4;
	uint8_t id : 4;
};

void initLeds(void);
void disableLeds(void);

void switchOnAnode(struct LedAnode anode);
void switchOffAnodes(void);

void switchOnCathode(struct LedCathode cathode);
void switchOffCathodes(void);

#endif /* LED_DRIVER_H_INC */
