import skidl as sk
import os
import sys
from itertools import zip_longest
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


def efm_lfxo(efm32, crystal, gnd):
    efm32['LFXTAL_P'] += Net('LFXTAL_P')
    efm32['LFXTAL_N'] += Net('LFXTAL_N')
    efm32['LFXTAL_P'] & crystal & efm32['LFXTAL_N']
    efm32['LFXTAL_P'] & Cap('22pF', description='32768 Hz crystal load capacitor') & gnd
    efm32['LFXTAL_N'] & Cap('22pF', description='32768 Hz crystal load capacitor') & gnd


def efm_handle_unused_pins(efm32xx232, vdd, gnd):
    for spare_pin in efm32xx232['PC[4-5]/']:
        spare_pin & Res('0') & gnd

    unused_pins = [pin for pin in efm32xx232 if not pin.nets]
    for pin in unused_pins:
        pin += vdd

    return unused_pins


def imu_power(lsm6ds3, power_mosfet_gate, vdd, gnd):
    lsm6ds3['GND'] += gnd

    imu_vdd = Net('IMU_VDD')

    imu_power_mosfet = Part('Device', 'Q_PMOS_GSD', footprint='Package_TO_SOT_SMD:SOT-23')
    imu_power_mosfet['G'] += power_mosfet_gate
    imu_power_mosfet['S'] += vdd
    imu_power_mosfet['D'] += imu_vdd
    imu_vdd.drive = POWER

    lsm6ds3['VDD'] += imu_vdd
    lsm6ds3['VDD'] & Cap('0.1uF', description='LSM6DS3 VDD decoupling cap') & gnd

    lsm6ds3['VDDIO'] += imu_vdd
    lsm6ds3['VDDIO'] & Cap('0.1uF', description='LSM6DS3 VDDIO decoupling cap') & gnd

    lsm6ds3['VDD'] & Cap('10uF', description='LSM6DS3 VDD decoupling cap') & gnd


def mcu_imu_spi(efm32xx232, lsm6ds3, gnd):
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
    unused_anode_strings = []
    for cathode_index, cathode_string in enumerate(cathodes):
        anodes_iterator = iter(anodes)

        for first_anode_string, second_anode_string in zip(
            anodes_iterator, anodes_iterator
        ):
            led = led_template()
            led['K'] += cathode_string
            if cathode_index < len(cathodes) / 2:
                led['A1'] += first_anode_string
                led['A2'] += second_anode_string
            else:
                led['A1'] += second_anode_string
                led['A2'] += first_anode_string

            total_leds += 1
            if total_leds == led_count:
                return

    if len(anodes) % 2 == 1:
        unused_anode_string = anodes[-1]
        cathodes_iterator = iter(cathodes)
        for first_cathode_string, second_cathode_string in zip(
            cathodes_iterator, cathodes_iterator
        ):
            led = led_template()
            led['A'] += unused_anode_string
            led['K1'] += first_cathode_string
            led['K2'] += second_cathode_string

            total_leds += 1
            if total_leds == led_count:
                return

    if led_count is not None and total_leds < led_count:
        raise Exception(
            'Number of anode and cathode strings is not enough for specified LED count.'
        )


coin_battery = Part('Device', 'Battery_Cell', footprint='footprints:BatteryHolder_Keystone_3026_1x2016')
mcu = Part('EFM32LG232F64', 'EFM32LG232F64', footprint='Package_QFP:LQFP-64_10x10mm_P0.5mm')
lfxo = Part('Device', 'Crystal', value='32768Hz', footprint='Crystal:Crystal_SMD_3215-2Pin_3.2x1.5mm')
imu = Part('Sensor_Motion', 'LSM6DS3', footprint='footprints:LSM6DS3')
led_template = Part('Device', 'LED_DUAL_AACC', TEMPLATE, footprint='footprints:LED_DUAL_0606')
touch_pads = 4 * Part(touch_sense_lib, 'TOUCH_PAD', TEMPLATE, footprint='footprints:TouchPad_D8.0mm')
touch_slider = Part(touch_sense_lib, 'TOUCH_SLIDER_4PADS', footprint='footprints:TouchSlider-4_42x7mm')
pogo_pads = Part(pogo_pads_lib, 'POGO_10PADS', footprint='footprints:TestPoints_2x05_P2.54_D1.7')

vdd = Net('VDD')
gnd = Net('GND')
coin_battery['+'] += vdd
coin_battery['-'] += gnd
coin_battery['+'].drive = POWER
coin_battery['-'].drive = POWER

efm_power(mcu, vdd, gnd)
efm_debugging_interface(mcu, pogo_pads, vdd, gnd)

imu_power(imu, mcu['PD4'], vdd, gnd)
mcu_imu_spi(mcu, imu, gnd)

efm_lfxo(mcu, lfxo, gnd)

led_anode_strings = Bus('LED_A', 9)
led_anode_strings += mcu['PA[0-5]/, PA[8:10]']
led_cathode_strings = Bus('LED_K', 12)
led_cathode_strings += mcu['PE[8:15], PF[2-5]/']
bicolor_led_matrix(led_template, led_anode_strings, led_cathode_strings, led_count=49)

touch_pad_lines = Bus('TOUCH_PAD', [pad[1] for pad in touch_pads])
touch_pad_lines += mcu['PC14, PC12, PC9, PC8']
touch_slider[:] += mcu['PC13, PC11, PC10, PC6']

unused_pins = efm_handle_unused_pins(mcu, vdd, gnd)
print('These {} unused pins were connected to VDD:\n{}'.format(
    len(unused_pins), '\n'.join([str(pin) for pin in unused_pins])
))

ERC()
generate_netlist()
