import math
from tkinter import Tk, Scale, Button, mainloop, HORIZONTAL
from matplotlib import pyplot as plt

from led_pattern import generate


def show_values():
    all_leds = generate(w1.get(), w2.get(), w3.get(), w4.get() / 180 * math.pi)
    plt.plot([x for x, y in all_leds], [y for x, y in all_leds], 'go')
    plt.axis('equal')
    plt.show()


if __name__ == '__main__':
    master = Tk()
    w1 = Scale(master, from_=1, to=21, orient=HORIZONTAL)
    w1.set(9)
    w1.pack()
    w2 = Scale(master, from_=0.5, to=10, orient=HORIZONTAL)
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
