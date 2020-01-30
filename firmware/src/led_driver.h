#ifndef LED_DRIVER_H_INC
#define LED_DRIVER_H_INC

#include "leds.h"

void initializeLedDriver(void);
void disableLeds(void);

// It's necessary to prepareAnode() before switching it on.
// It allows switchOnAnode() to take less time.
void prepareAnode(struct LedAnode anode);
void switchOnAnode(struct LedAnode anode);
void switchOffAnodes(void);

void switchOnCathode(struct LedCathode cathode);
void switchOffCathodes(void);

#endif /* LED_DRIVER_H_INC */
