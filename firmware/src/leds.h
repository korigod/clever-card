#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"

#define LED_FPS 50
#define LED_COUNT 4

enum LedColor {
	red,
	green
};

struct Led {
	uint8_t cathode : 4;
	uint8_t anode : 4;
	enum LedColor color : 2;
};

extern struct Led leds[LED_COUNT];

void initLeds();

#define UPDATE_LEDS_STACK_SIZE configMINIMAL_STACK_SIZE
void updateLeds(void * pvParameters);

void switchOffAnodes(void);
void resetCathodes(void);
void enableCathode(uint8_t cathode);
