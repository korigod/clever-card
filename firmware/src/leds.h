#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"

#define LED_FPS 50
#define LED_COUNT 2

struct Led {
	uint8_t cathode : 4;
	uint8_t anode : 4;
	uint8_t brightness;
};

struct LedIds {
	uint8_t red;
	uint8_t green;
};

extern struct Led leds[LED_COUNT * 2];
struct LedIds ledIds[LED_COUNT];

void initLeds();

#define UPDATE_LEDS_STACK_SIZE configMINIMAL_STACK_SIZE
void updateLeds(void * pvParameters);

void switchOffAnodes(void);
void resetCathodes(void);
void enableCathode(uint8_t cathode);
