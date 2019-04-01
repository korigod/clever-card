import math


def generate(LINEAR_LED_COUNT, LINEAR_RADIUS, CIRCULAR_SEGMENT_LED_COUNT, CIRCLULAR_TO_LINEAR_ANGLE):
    # LINEAR_LED_COUNT = 9  # 7
    # LINEAR_RADIUS = 1
    # CIRCLULAR_TO_LINEAR_ANGLE = math.pi / 6  # 25 deg
    # CIRCULAR_SEGMENT_LED_COUNT = 8  # 7

    linear_leds = [
        -LINEAR_RADIUS + i * (2 * LINEAR_RADIUS / (LINEAR_LED_COUNT - 1))
        for i in range(LINEAR_LED_COUNT)
    ]

    circular_segment_angular_size = math.pi * 3 / 2 - 2 * CIRCLULAR_TO_LINEAR_ANGLE

    circular_segment_radius = math.sqrt(2) / 2 * LINEAR_RADIUS / math.sin(circular_segment_angular_size / 2)

    circular_segment_leds = [
        (
            circular_segment_radius * math.cos(
                math.pi / 4 - circular_segment_angular_size / 2 + i * (circular_segment_angular_size / (CIRCULAR_SEGMENT_LED_COUNT + 1))
            ) + 1 - circular_segment_radius * math.cos(math.pi / 4 - circular_segment_angular_size / 2),
            circular_segment_radius * math.sin(
                math.pi / 4 - circular_segment_angular_size / 2 + i * (circular_segment_angular_size / (CIRCULAR_SEGMENT_LED_COUNT + 1))
            ) + 1 - circular_segment_radius * math.cos(math.pi / 4 - circular_segment_angular_size / 2)
        ) for i in range(1, CIRCULAR_SEGMENT_LED_COUNT + 1)
    ]

    all_circular_segments_leds = [[(x, y), (-x, y), (-x, -y), (x, -y)] for x, y in circular_segment_leds]

    all_circular_segments_leds_flat = [i for sublist in zip(*all_circular_segments_leds) for i in sublist]

    all_linear_leds = [(x, 0) for x in linear_leds] + [(0, y) for y in linear_leds if abs(y) > 1e-9]

    all_leds = all_circular_segments_leds_flat + all_linear_leds

    return all_leds
