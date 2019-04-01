from math import cos, pi, sin, sqrt


def generate(linear_led_count, linear_radius, circular_segment_led_count, circular_to_linear_angle):
    linear_leds = [
        -linear_radius + i * (2 * linear_radius / (linear_led_count - 1))
        for i in range(linear_led_count)
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

        arc_circle_center_shift = 1 - circular_segment_radius * cos(
            pi / 4 - circular_segment_angular_size / 2
        )

        circular_segment_leds.append(
            (arc_point_x + arc_circle_center_shift, arc_point_y + arc_circle_center_shift)
        )

    all_circular_segments_leds = [[(x, y), (-x, y), (-x, -y), (x, -y)] for x, y in circular_segment_leds]

    all_circular_segments_leds_flat = [i for sublist in zip(*all_circular_segments_leds) for i in sublist]

    all_linear_leds = [(x, 0) for x in linear_leds] + [(0, y) for y in linear_leds if abs(y) > 1e-9]

    all_leds = all_circular_segments_leds_flat + all_linear_leds

    return all_leds
