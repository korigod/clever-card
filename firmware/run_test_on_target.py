import sys
import os
import time
import argparse
import subprocess
import pylink

OBJCOPY_PATH = os.path.expanduser('~/Downloads/SimplicityStudio_v4/developer/toolchains/gnu_arm/7.2_2017q4/bin/arm-none-eabi-objcopy')
OUTPUT_BINARY_PATH = 'build/test/out/test.bin'

parser = argparse.ArgumentParser()
parser.add_argument('-d', '--device', default='EFM32TG232F32', metavar='device', help='target device identifier as expected by JLink')
parser.add_argument('-s', '--swo-speed', default=875000, type=int, metavar='baudrate', help='SWO baud rate')
parser.add_argument('-f', '--file', required=True, metavar='executable.out', help='.out file to upload to target device')
args = parser.parse_args()

subprocess.call([OBJCOPY_PATH, "-O", "binary", args.file, OUTPUT_BINARY_PATH])

jlink = pylink.JLink()
jlink.open()
jlink.set_tif(pylink.enums.JLinkInterfaces.SWD)
jlink.connect(args.device, verbose=True)
jlink.set_reset_strategy(pylink.enums.JLinkResetStrategyCortexM3.RESETPIN)
jlink.reset()
jlink.halt()
jlink.flash_file(OUTPUT_BINARY_PATH, addr=0x0)

jlink.swo_start(args.swo_speed)
jlink.reset()
jlink.swo_flush()
jlink.restart()

data_str = ''
while 'OK' not in data_str and 'FAIL' not in data_str:
    num_bytes = jlink.swo_num_bytes()
    if num_bytes == 0:
        time.sleep(0.1)
        continue

    data = jlink.swo_read(0, num_bytes, True)
    data_str = ''.join(map(chr, data))

    # Unfortunately, swo_read_stimulus does not work properly. For some reason,
    # the output is read twice and the first byte is lost. Raw SWO data received
    # with swo_read contains bytes which encode the stimulus id and the package
    # size (1, 2 or 4 bytes). 0x01 means "stimulus 0, single byte package".
    data_str = data_str.replace('\x01', '')

    sys.stdout.write(data_str)
    sys.stdout.flush()

jlink.swo_stop()
