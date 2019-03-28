import skidl as sk
import os
import sys
from skidl import Bus, ERC, generate_netlist, Net, Part, POWER, subcircuit, TEMPLATE

from pogo_pads_sklib import pogo_pads_lib
from touch_sense_sklib import touch_sense_lib


sk.lib_search_paths[sk.KICAD].extend([
    '/usr/share/kicad/library',
    'components'
])


class Cap(Part):
    def __init__(self, capacitance: str = '0.1uF', footprint: str = 'C_0603_1608Metric', *args, **kwargs):
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

    efm['DECOUPLE'] & Cap('1uF', 'C_0603_1608Metric') & gnd
    efm['DECOUPLE'].drive = POWER

    for pin in efm['IOVDD_[0-9]+']:
        pin += vdd
        vdd & Cap('0.1uF', 'C_0603_1608Metric', description=f'{pin} decoupling cap') & gnd
    vdd & Cap('10uF', 'C_0603_1608Metric', description=f'IOVDD decoupling cap') & gnd

    avdd = Net('MCU_AVDD')
    avdd.drive = POWER
    for pin in efm['AVDD_[0-9]+']:
        pin += avdd
        avdd & Cap('10nF', 'C_0603_1608Metric', description=f'{pin} decoupling cap') & gnd
    avdd & Cap('10uF', 'C_0603_1608Metric', description=f'AVDD decoupling cap') & gnd
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


def efm_debugging_interface(efm32xx232, connector_10_pins, vdd, gnd):
    # Pinout is compatible with EFM Mini Simplicity Connector
    connector_10_pins[1] += vdd
    connector_10_pins[2] += gnd
    connector_10_pins[3] += mcu['RESET']
    connector_10_pins[4] += mcu['US0_RX_#5']
    connector_10_pins[5] += mcu['US0_TX_#5']
    connector_10_pins[6] += mcu['DBG_SWO_#1']
    connector_10_pins[7] += mcu['DBG_SWDIO']
    connector_10_pins[8] += mcu['DBG_SWCLK']
    connector_10_pins[9] += mcu['ETM_TCLK_#0']
    connector_10_pins[10] += mcu['ETM_TD0_#0']


def efm_lfxo(efm32, crystal):
    efm32['LFXTAL_P'] & crystal & efm32['LFXTAL_N']


def imu_power(lsm6ds3, vdd, gnd):
    lsm6ds3['GND'] += gnd

    lsm6ds3['VDD'] += vdd
    lsm6ds3['VDD'] & Cap('0.1uF', description='LSM6DS3 VDD decoupling cap') & gnd

    lsm6ds3['VDDIO'] += vdd
    lsm6ds3['VDDIO'] & Cap('0.1uF', description='LSM6DS3 VDDIO decoupling cap') & gnd


def mcu_imu_spi(efm32xx232, lsm6ds3):
    lsm6ds3['SDO'] += efm32xx232['US1_RX_#1']
    lsm6ds3['SDA'] += efm32xx232['US1_TX_#1']
    lsm6ds3['SCL'] += efm32xx232['US1_CLK_#1']
    lsm6ds3['CS'] += efm32xx232['US1_CS_#1']

    lsm6ds3['INT1'] += efm32xx232['PB13']
    lsm6ds3['INT2'] += efm32xx232['PB14']

    # Unused I2C master for additional sensors
    lsm6ds3['SDX'] += gnd
    lsm6ds3['SCX'] += gnd


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


coin_battery = Part('Device', 'Battery_Cell', footprint='Battery:BatteryHolder_Keystone_3034_1x20mm')
mcu = Part('EFM32LG232F64', 'EFM32LG232F64', footprint='Package_QFP:LQFP-64_10x10mm_P0.5mm')
lfxo = Part('Device', 'Crystal', value='32768Hz', footprint='Crystal:Crystal_SMD_3215-2Pin_3.2x1.5mm')
imu = Part('Sensor_Motion', 'LSM6DS3', footprint='footprints:LSM6DS3')
led_template = Part('Device', 'LED_DUAL_AACC', TEMPLATE, footprint='footprints:LED_DUAL_0606')
touch_pads = 4 * Part(touch_sense_lib, 'TOUCH_PAD', TEMPLATE, footprint='TouchSense_Pad_D8.0mm')
touch_slider = Part(touch_sense_lib, 'TOUCH_SLIDER_4PADS', footprint='footprints:TouchSlider-4_50x8mm')
pogo_pads = Part(pogo_pads_lib, 'POGO_10PADS', footprint='')

vdd = Net('VDD')
gnd = Net('GND')
coin_battery['+'] += vdd
coin_battery['-'] += gnd
coin_battery['+'].drive = POWER
coin_battery['-'].drive = POWER

efm_power(mcu, vdd, gnd)
efm_debugging_interface(mcu, pogo_pads, vdd, gnd)
efm_lfxo(mcu, lfxo)
imu_power(imu, vdd, gnd)
mcu_imu_spi(mcu, imu)

led_anode_strings = Bus('LED_A', 9)
led_anode_strings += mcu['PA[0-5]/, PA[8:10]']
led_cathode_strings = Bus('LED_K', 12)
led_cathode_strings += mcu['PF[2-5]/, PE[8:15]']
bicolor_led_matrix(led_template, led_anode_strings, led_cathode_strings, led_count=49)

touch_pad_lines = Bus('TOUCH_PAD', [pad[1] for pad in touch_pads])
touch_pad_lines += mcu['PC(6|8|9|10)']
touch_slider[:] += mcu['PC[11:14]']

ERC()
generate_netlist()
