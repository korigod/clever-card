import math
import pcbnew as pn

from led_pattern import generate


def place_leds():
    led_coords = generate(9, 12.7, 8, math.pi / 6)
    print(led_coords)

    # pcb = pn.LoadBoard('kicad/kicad.kicad_pcb')
    pcb = pn.GetBoard()

    leds = []
    for module in pcb.GetModules():
        ref = module.GetReference().encode('utf-8')
        if (ref.startswith('D')):
            leds.append(module)

    for led, coords in zip(leds, led_coords):
        led.SetPosition(
            pn.wxPoint(pn.FromMM(coords[0]), pn.FromMM(coords[1]))
        )

    # pn.Save('kicad/kicad.kicad_pcb')
    pn.Refresh()
