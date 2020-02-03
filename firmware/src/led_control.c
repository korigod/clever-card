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
	const RgLedId_t arrayToTraverse[],
	const size_t arraySize,
	const bool reverseTraverseDirection,
	TickType_t *const previousWakeTimePtr,
	const uint32_t millisecondsToKeepEachLedOn
) {
	for (size_t i = 0; i < arraySize; i++) {
		size_t index = i;
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
	const RgLedId_t arrayToTraverse[],
	const size_t arraySize,
	const bool reverseTraverseDirection,
	TickType_t *const previousWakeTimePtr,
	const uint32_t millisecondsToKeepEachLedOn
) {
	for (uint32_t frame = 0; frame <= (arraySize - 1) * millisecondsToKeepEachLedOn * LED_FPS / 1000; frame++) {
		vTaskDelayUntil(previousWakeTimePtr, pdMS_TO_TICKS(1000 / LED_FPS));

		clearLeds();

		float currentLedId = frame * 1000.0f / (float)LED_FPS / (float)millisecondsToKeepEachLedOn;

		if (reverseTraverseDirection) {
			currentLedId = (arraySize - 1) - currentLedId;
		}

		for (size_t i = 0; i < arraySize; i++) {
			ledOutputs[rgLeds[arrayToTraverse[i]].red] = 5 * fmaxf(2 - fabsf(i - currentLedId), 0);
		}

		latchLedOutputs();
	}
}


struct RgLedArray {
	const RgLedId_t *const arrayPtr;
	const size_t arraySize;
	const bool reverse;
};

void smoothlyTraverseLedsInArrayOfArrays(
	const struct RgLedArray arrayOfArraysToTraverse[],
	const size_t arrayCount,
	const bool reverseTraverseDirection,
	const bool traverseArraysInSeries,
	const bool smoothStartAndEnd,
	const float traverseZoneRadius,
	TickType_t *const previousWakeTimePtr,
	const uint32_t millisecondsToKeepEachLedOn
) {
	size_t totalLedCount = 0;
	for (size_t i = 0; i < arrayCount; i++) {
		if (traverseArraysInSeries) {
			totalLedCount += arrayOfArraysToTraverse[i].arraySize;
		} else {
			totalLedCount = max(totalLedCount, arrayOfArraysToTraverse[i].arraySize);
		}
	}

	if (smoothStartAndEnd) {
		totalLedCount += 2 * traverseZoneRadius;
	}

	#pragma GCC diagnostic ignored "-Wunused-variable"
	const TickType_t ticksBetweenLedUpdates = pdMS_TO_TICKS(1000 / LED_FPS);
	#pragma GCC diagnostic pop

	for (uint32_t frame = 0; frame <= (totalLedCount - 1) * millisecondsToKeepEachLedOn * LED_FPS / 1000; frame++) {
		// Assert that led updates take no more than 1/4 of time, necessary to keep up with defined FPS
		ASSERT_DEBUG(xTaskGetTickCount() - *previousWakeTimePtr < ticksBetweenLedUpdates / 2);
		ASSERT_RELEASE(xTaskGetTickCount() - *previousWakeTimePtr < ticksBetweenLedUpdates / 4);

		vTaskDelayUntil(previousWakeTimePtr, ticksBetweenLedUpdates);

		clearLeds();

		float currentLedId = frame * 1000.0f / (float)LED_FPS / (float)millisecondsToKeepEachLedOn;
		if (smoothStartAndEnd) {
			currentLedId -= traverseZoneRadius;
		}

		int ledsBehind = 0;
		for (size_t arrayIndex = 0; arrayIndex < arrayCount; arrayIndex++) {
			const struct RgLedArray ledArray = arrayOfArraysToTraverse[arrayIndex];

			for (size_t i = 0; i < ledArray.arraySize; i++) {
				size_t index = i;
				if (ledArray.reverse) {
					index = ledArray.arraySize - i - 1;
				}

				size_t indexInWholeSequence;
				if (traverseArraysInSeries) {
					indexInWholeSequence = index + ledsBehind;
				} else {
					indexInWholeSequence = index;
				}

				const float score = traverseZoneRadius - fabsf(indexInWholeSequence - currentLedId);
				const float brightness = fmaxf(score, 0);
				const float middleIndex = (float)(ledArray.arraySize - 1) / 2.0f;
				const float redBrightness = brightness * fabsf((float)i - middleIndex) / middleIndex;
				const float greenBrightness = brightness - redBrightness;
				const RgLedId_t rgLedIndex = ledArray.arrayPtr[i];
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
		const struct RgLedArray allLedArrays[] = {
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
