#include <math.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#include "leds.h"

#define LED_FPS 25


void traverseLedsInArray(
	RgLedId_t arrayToTraverse[],
	size_t arraySize,
	bool reverseTraverseDirection,
	TickType_t * previousWakeTimePtr,
	uint32_t millisecondsToKeepEachLedOn
) {
	for (int i = 0; i < arraySize; i++) {
		int index = i;
		if (reverseTraverseDirection) {
			index = arraySize - i - 1;
		}
		vTaskDelayUntil(previousWakeTimePtr, pdMS_TO_TICKS(millisecondsToKeepEachLedOn));
		clearLeds();
		ledOutputs[rgLeds[arrayToTraverse[index]].red] = 10;
		ledOutputs[rgLeds[arrayToTraverse[index]].green] = 20;
		latchLedOutputs();
	}
}


void smoothlyTraverseLedsInArray(
	RgLedId_t arrayToTraverse[],
	size_t arraySize,
	bool reverseTraverseDirection,
	TickType_t * previousWakeTimePtr,
	uint32_t millisecondsToKeepEachLedOn
) {
	for (uint32_t frame = 0; frame <= (arraySize - 1) * millisecondsToKeepEachLedOn * LED_FPS / 1000; frame++) {
		vTaskDelayUntil(previousWakeTimePtr, pdMS_TO_TICKS(1000 / LED_FPS));
		clearLeds();
		float currentLedId = frame * 1000.0f / (float)LED_FPS / (float)millisecondsToKeepEachLedOn;
		if (reverseTraverseDirection) {
			currentLedId = (arraySize - 1) - currentLedId;
		}
		for (int i = 0; i < arraySize; i++) {
			ledOutputs[rgLeds[arrayToTraverse[i]].red] = 5 * fmaxf(2 - fabsf(i - currentLedId), 0);
		}

		latchLedOutputs();
	}
}


void ledControlTask(void * pvParameters) {
	TickType_t previousWakeTime = xTaskGetTickCount();
	const uint32_t timeToKeepLedOn = 100;
	while (1) {
		smoothlyTraverseLedsInArray(topLeftArcLeds, sizeof(topLeftArcLeds), false, &previousWakeTime, timeToKeepLedOn);
		smoothlyTraverseLedsInArray(verticalLeds, sizeof(verticalLeds), true, &previousWakeTime, timeToKeepLedOn);
		smoothlyTraverseLedsInArray(bottomLeftArcLeds, sizeof(bottomLeftArcLeds), false, &previousWakeTime, timeToKeepLedOn);
		smoothlyTraverseLedsInArray(horizontalLeds, sizeof(horizontalLeds), false, &previousWakeTime, timeToKeepLedOn);
		smoothlyTraverseLedsInArray(bottomRightArcLeds, sizeof(bottomRightArcLeds), false, &previousWakeTime, timeToKeepLedOn);
		smoothlyTraverseLedsInArray(verticalLeds, sizeof(verticalLeds), false, &previousWakeTime, timeToKeepLedOn);
		smoothlyTraverseLedsInArray(topRightArcLeds, sizeof(topRightArcLeds), false, &previousWakeTime, timeToKeepLedOn);
		smoothlyTraverseLedsInArray(horizontalLeds, sizeof(horizontalLeds), true, &previousWakeTime, timeToKeepLedOn);
	}
}
