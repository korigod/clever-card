import skidl as sk
import os
import sys
from skidl import generate_netlist, Net, Part, subcircuit, TEMPLATE


sk.lib_search_paths[sk.KICAD].extend([
    '/usr/share/kicad/library',
    'components'
])

coin_battery = Part('Device', 'Battery_Cell', footprint='Battery_Holders:3028')
mcu = Part('EFM32LG232F64', 'EFM32LG232F64', footprint='Package_QFP:LQFP-64_10x10mm_P0.5mm')
imu = Part('Sensor_Motion', 'LSM6DS3', footprint='LSM6D3:LSM6D3')
# led_dc_converter = Part('charge_pumps', 'CAT3661', footprint='Package_DFN_QFN:QFN-16-1EP_3x3mm_P0.5mm_EP1.8x1.8mm')

leds = 49 * Part('led_rgba', 'LED_RGBA', TEMPLATE)


@subcircuit
def high_side_switch(control_in, load_power_out, vcc, gnd):
    driving_mosfet = Part('Device', 'Q_NMOS_DSG')
    driving_mosfet_pulldown = Part('Device', 'R', value='220K')

    power_mosfet = Part('Device', 'Q_PMOS_DSG')
    power_mosfet_pullup = Part('Device', 'R', value='22K')

    driving_mosfet['G'] += control_in
    driving_mosfet['G'] & driving_mosfet_pulldown & gnd
    driving_mosfet['S'] += gnd

    (
        (vcc | power_mosfet['S']) &
        power_mosfet_pullup &
        (driving_mosfet['D'] | power_mosfet['G'])
    )
    power_mosfet['D'] += load_power_out


gnd = Net('GND')

sw = high_side_switch(Net(), Net(), Net(), gnd)

coin_battery['-'] += gnd

generate_netlist()
