#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"

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
static StaticTask_t updateLedsTaskControlBlock;
static StackType_t updateLedsTaskStack[UPDATE_LEDS_STACK_SIZE];
void updateLeds(void * pvParameters);
