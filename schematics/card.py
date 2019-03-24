import skidl as sk
import os
import sys
from skidl import Bus, generate_netlist, Net, Part, subcircuit, TEMPLATE


sk.lib_search_paths[sk.KICAD].extend([
    '/usr/share/kicad/library',
    'components'
])


class Cap(Part):
    def __init__(self, capacitance: str = '0.1uF', footprint: str = 'C_0603', *args, **kwargs):
        footprint_name = footprint if ':' in footprint else f'Capacitor_SMD:{footprint}'
        super().__init__(
            'Device', 'C', value=capacitance, footprint=footprint_name, *args, **kwargs
        )


class Res(Part):
    def __init__(self, resistance: str, footprint: str = 'R_0603_1608Metric', *args, **kwargs):
        footprint_name = footprint if ':' in footprint else f'Resistor_SMD:{footprint}'
        super().__init__(
            'Device', 'R', value=resistance, footprint=footprint_name, *args, **kwargs
        )


def efm_power(efm, vdd, gnd):
    efm['VSS'] += gnd

    efm['DECOUPLE'] & Cap('1uF', 'C_0603') & gnd

    for pin in efm['IOVDD_[0-9]+']:
        pin += vdd
        vdd & Cap('0.1uF', 'C_0603', description=f'{pin} decoupling cap') & gnd
    vdd & Cap('10uF', 'C_0603', description=f'IOVDD decoupling cap') & gnd

    avdd = Net('MCU_AVDD')
    for pin in efm['AVDD_[0-9]+']:
        pin += avdd
        avdd & Cap('10nF', 'C_0603', description=f'{pin} decoupling cap') & gnd
    avdd & Cap('10uF', 'C_0603', description=f'AVDD decoupling cap') & gnd
    (
        vdd &
        Part('Device', 'Ferrite_Bead_Small', footprint='Inductor_SMD:L_0603_1608Metric') &
        Res('1') &
        avdd
    )

    efm['VDD_DREG'] += vdd
    (
        vdd & (
            Cap('0.1uF', description='VDD decoupling cap') |
            Cap('3.3uF', description='VDD decoupling cap') |
            Cap('100uF', description='VDD decoupling cap')
        ) & gnd
    )


def bicolor_led_matrix(led_template, anodes, cathodes, led_count=None):
    total_leds = 0
    for anode_string in anodes:
        cathodes_iterator = iter(cathodes)

        for first_cathode_string, second_cathode_string in zip(
            cathodes_iterator, cathodes_iterator
        ):
            led = led_template()
            led['A1'] += anode_string
            led['A2'] += anode_string
            led['K1'] += first_cathode_string
            led['K2'] += second_cathode_string

            total_leds += 1
            if total_leds == led_count:
                return

    if led_count is not None and total_leds < led_count:
        raise Exception(
            'Number of anode and cathode strings is not enough for specified LED count.'
        )


coin_battery = Part('Device', 'Battery_Cell', footprint='Battery_Holders:3028')
mcu = Part('EFM32LG232F64', 'EFM32LG232F64', footprint='Package_QFP:LQFP-64_10x10mm_P0.5mm')
imu = Part('Sensor_Motion', 'LSM6DS3', footprint='LSM6D3:LSM6D3')
led_template = Part('Device', 'LED_DUAL_AACC', TEMPLATE, footprint='LED_DUAL_0606')

vdd = Net('VDD')
gnd = Net('GND')
coin_battery['+'] += vdd
coin_battery['-'] += gnd

efm_power(mcu, vdd, gnd)

led_anode_strings = Bus('LED_A', 10)
led_cathode_strings = Bus('LED_K', 10)
bicolor_led_matrix(led_template, led_anode_strings, led_cathode_strings, led_count=49)

generate_netlist()
