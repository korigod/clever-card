import skidl as sk
import os
from skidl import Net, Part, TEMPLATE


sk.lib_search_paths[sk.KICAD].extend([
    '/usr/share/kicad/library',
    'components'
])

coin_battery = Part('Device', 'Battery_Cell', footprint='Battery_Holders:3028')
# mcu = Part('efm32', '')
imu = Part('Sensor_Motion', 'LSM6DS3')
# imu = Part('dk_Motion-Sensors-IMUs-Inertial-Measurement-Units', 'LSM6DS3TR')
# led_dc_converter = Part('charge_pumps', 'CAT3661')
shift_registers = 3 * Part(
    # Nexperia 74HC595PW,118 / Diodes 74HC595T16-13
    '74xx', '74HC595', TEMPLATE, footprint='Package_SO:TSSOP-16_4.4x5mm_P0.65mm'
)
leds = 49 * Part('led_rgba', 'LED_RGBA', TEMPLATE)

gnd = Net('GND')

coin_battery['-'] += gnd
