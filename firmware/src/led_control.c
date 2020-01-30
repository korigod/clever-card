#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#include "leds.h"


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


void ledControlTask(void * pvParameters) {
	TickType_t previousWakeTime = xTaskGetTickCount();
	const uint32_t timeToKeepLedOn = 100;
	while (1) {
		traverseLedsInArray(topLeftArcLeds, sizeof(topLeftArcLeds), false, &previousWakeTime, timeToKeepLedOn);
		traverseLedsInArray(verticalLeds, sizeof(verticalLeds), true, &previousWakeTime, timeToKeepLedOn);
		traverseLedsInArray(bottomLeftArcLeds, sizeof(bottomLeftArcLeds), false, &previousWakeTime, timeToKeepLedOn);
		traverseLedsInArray(horizontalLeds, sizeof(horizontalLeds), false, &previousWakeTime, timeToKeepLedOn);
		traverseLedsInArray(bottomRightArcLeds, sizeof(bottomRightArcLeds), false, &previousWakeTime, timeToKeepLedOn);
		traverseLedsInArray(verticalLeds, sizeof(verticalLeds), false, &previousWakeTime, timeToKeepLedOn);
		traverseLedsInArray(topRightArcLeds, sizeof(topRightArcLeds), false, &previousWakeTime, timeToKeepLedOn);
		traverseLedsInArray(horizontalLeds, sizeof(horizontalLeds), true, &previousWakeTime, timeToKeepLedOn);
	}
}
