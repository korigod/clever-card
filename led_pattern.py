from __future__ import division

from math import cos, pi, sin, sqrt
from itertools import chain


def generate(linear_led_count, linear_radius, circular_segment_led_count, circular_to_linear_angle):
    assert(linear_led_count % 2 == 1)
    linear_led_segment = [
        i * (2 * linear_radius / (linear_led_count - 1))
        for i in range(1, linear_led_count // 2 + 1)
    ]

    circular_segment_angular_size = pi * 3 / 2 - 2 * circular_to_linear_angle
    circular_segment_radius = sqrt(2) / 2 * linear_radius / sin(circular_segment_angular_size / 2)

    circular_segment_leds = []
    for i in range(1, circular_segment_led_count + 1):
        arc_point_x = circular_segment_radius * cos(
            pi / 4 - circular_segment_angular_size / 2 + i * (
                circular_segment_angular_size / (circular_segment_led_count + 1)
            )
        )

        arc_point_y = circular_segment_radius * sin(
            pi / 4 - circular_segment_angular_size / 2 + i * (
                circular_segment_angular_size / (circular_segment_led_count + 1)
            )
        )

        arc_circle_center_shift = linear_radius - circular_segment_radius * cos(
            pi / 4 - circular_segment_angular_size / 2
        )

        circular_segment_leds.append(
            (arc_point_x + arc_circle_center_shift, arc_point_y + arc_circle_center_shift)
        )

    all_circular_segments_leds = [[(x, y), (-y, x), (-x, -y), (y, -x)] for x, y in circular_segment_leds]
    all_circular_segments_leds_flat = list(chain.from_iterable(all_circular_segments_leds))

    # Linear LEDs are arranged in circular manner to simplify PCB layout
    linear_leds_arranged = [[(a, 0), (0, a), (-a, 0), (0, -a)] for a in linear_led_segment]
    linear_leds_flat = list(chain.from_iterable(linear_leds_arranged))

    central_led = (0, 0)

    all_leds = linear_leds_flat + all_circular_segments_leds_flat + [central_led]

    return all_leds
