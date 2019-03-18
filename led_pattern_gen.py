import math
from matplotlib import pyplot as plt

from tkinter import Tk, Scale, Button, mainloop, HORIZONTAL


def draw(LINEAR_LED_COUNT, LINEAR_RADIUS, CIRCULAR_SEGMENT_LED_COUNT, CIRCLULAR_TO_LINEAR_ANGLE):
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

    print(len(all_leds))

    plt.plot([x for x, y in all_leds], [y for x, y in all_leds], 'go')
    plt.axis('equal')
    plt.show()


def show_values():
    draw(w1.get(), w2.get(), w3.get(), w4.get() / 180 * math.pi)


master = Tk()
w1 = Scale(master, from_=1, to=21, orient=HORIZONTAL)
w1.set(9)
w1.pack()
w2 = Scale(master, from_=1, to=2, orient=HORIZONTAL)
w2.set(1)
w2.pack()
w3 = Scale(master, from_=1, to=21, orient=HORIZONTAL)
w3.set(8)
w3.pack()
w4 = Scale(master, from_=0, to=130, orient=HORIZONTAL)
w4.set(30)
w4.pack()
Button(master, text='Show', command=show_values).pack()

mainloop()
