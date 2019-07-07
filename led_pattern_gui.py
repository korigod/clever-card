import math
from tkinter import Tk, Scale, Button, mainloop, HORIZONTAL
from matplotlib import pyplot as plt

from led_pattern import generate


def show_values():
    all_leds = generate(
        linear_led_count.get(),
        linear_radius.get(),
        circular_segment_led_count.get(),
        circular_to_linear_angle.get() / 180 * math.pi
    )
    plt.plot([x for x, y in all_leds], [y for x, y in all_leds], 'go')
    plt.axis('equal')
    plt.show()


if __name__ == '__main__':
    master = Tk()
    linear_led_count = Scale(master, from_=1, to=21, orient=HORIZONTAL)
    linear_led_count.set(9)
    linear_led_count.pack()
    linear_radius = Scale(master, from_=1, to=2, orient=HORIZONTAL)
    linear_radius.set(1)
    linear_radius.pack()
    circular_segment_led_count = Scale(master, from_=1, to=21, orient=HORIZONTAL)
    circular_segment_led_count.set(8)
    circular_segment_led_count.pack()
    circular_to_linear_angle = Scale(master, from_=0, to=130, orient=HORIZONTAL)
    circular_to_linear_angle.set(30)
    circular_to_linear_angle.pack()
    Button(master, text='Show', command=show_values).pack()

    mainloop()
