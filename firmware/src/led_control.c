#include <math.h>
#include <stdlib.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "common.h"
#include "board.h"
#include "leds.h"

#define LED_FPS 50


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


struct RgLedArray {
	RgLedId_t * arrayPtr;
	size_t arraySize;
	bool reverse;
};

void smoothlyTraverseLedsInArrayOfArrays(
	struct RgLedArray arrayOfArraysToTraverse[],
	size_t arrayCount,
	bool reverseTraverseDirection,
	bool traverseArraysInSeries,
	bool smoothStartAndEnd,
	float traverseZoneRadius,
	TickType_t * previousWakeTimePtr,
	uint32_t millisecondsToKeepEachLedOn
) {
	size_t totalLedCount = 0;
	for (int i = 0; i < arrayCount; i++) {
		if (traverseArraysInSeries) {
			totalLedCount += arrayOfArraysToTraverse[i].arraySize;
		} else {
			totalLedCount = max(totalLedCount, arrayOfArraysToTraverse[i].arraySize);
		}
	}

	if (smoothStartAndEnd) {
		totalLedCount += 2 * traverseZoneRadius;
	}

	for (uint32_t frame = 0; frame <= (totalLedCount - 1) * millisecondsToKeepEachLedOn * LED_FPS / 1000; frame++) {
		vTaskDelayUntil(previousWakeTimePtr, pdMS_TO_TICKS(1000 / LED_FPS));

		clearLeds();

		float currentLedId = frame * 1000.0f / (float)LED_FPS / (float)millisecondsToKeepEachLedOn;
		if (smoothStartAndEnd) {
			currentLedId -= traverseZoneRadius;
		}

		int ledsBehind = 0;
		for (int arrayIndex = 0; arrayIndex < arrayCount; arrayIndex++) {
			struct RgLedArray ledArray = arrayOfArraysToTraverse[arrayIndex];

			for (int i = 0; i < ledArray.arraySize; i++) {
				int index = i;
				if (ledArray.reverse) {
					index = ledArray.arraySize - i - 1;
				}

				int indexInWholeSequence;
				if (traverseArraysInSeries) {
					indexInWholeSequence = index + ledsBehind;
				} else {
					indexInWholeSequence = index;
				}

				float score = traverseZoneRadius - fabsf(indexInWholeSequence - currentLedId);
				float brightness = fmaxf(score, 0);
				float middleIndex = (float)(ledArray.arraySize - 1) / 2.0f;
				float redBrightness = brightness * fabsf((float)i - middleIndex) / middleIndex;
				float greenBrightness = brightness - redBrightness;
				RgLedId_t rgLedIndex = ledArray.arrayPtr[i];
				ledOutputs[rgLeds[rgLedIndex].red] += (uint8_t)(redBrightness * 10.0f);
				ledOutputs[rgLeds[rgLedIndex].green] += (uint8_t)(greenBrightness * 10.0f);
			}
			ledsBehind += ledArray.arraySize;
		}

		latchLedOutputs();
	}
}


void ledControlTask(void * pvParameters) {
	TickType_t previousWakeTime = xTaskGetTickCount();
	const uint32_t timeToKeepLedOn = 200;
	while (1) {
		struct RgLedArray allLedArrays[] = {
			{topLeftArcLeds, sizeof(topLeftArcLeds) / sizeof(RgLedId_t), false},
			{bottomLeftArcLeds, sizeof(bottomLeftArcLeds) / sizeof(RgLedId_t), false},
			{horizontalLeds, sizeof(horizontalLeds) / sizeof(RgLedId_t), false},
			{bottomRightArcLeds, sizeof(bottomRightArcLeds) / sizeof(RgLedId_t), false},
			{verticalLeds, sizeof(verticalLeds) / sizeof(RgLedId_t), false},
			{topRightArcLeds, sizeof(topRightArcLeds) / sizeof(RgLedId_t), false},
		};
		smoothlyTraverseLedsInArrayOfArrays(
			allLedArrays,
			sizeof(allLedArrays) / sizeof(struct RgLedArray),
			false,
			false,
			true,
			3,
			&previousWakeTime,
			timeToKeepLedOn
		);
	}
}
