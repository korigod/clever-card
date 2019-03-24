from skidl import Pin, Part, SchLib, SKIDL, TEMPLATE

SKIDL_lib_version = '0.0.1'

touch_sense_lib = SchLib(tool=SKIDL).add_parts(*[
    Part(
        name='TOUCH_PAD',
        dest=TEMPLATE,
        tool=SKIDL,
        description='Single capacitive sense touch pad',
        keywords='capacitive sense touch pad',
        ref_prefix='TS',
        do_erc=True,
        pins=[
            Pin(num=1, name='PAD', func=Pin.PASSIVE, do_erc=True)
        ]
    ),
    Part(
        name='TOUCH_SLIDER_4PADS',
        dest=TEMPLATE,
        tool=SKIDL,
        description='Capacitive sense touch slider with 4 pads',
        keywords='capacitive sense touch slider',
        ref_prefix='TS',
        do_erc=True,
        pins=[
            Pin(num=1, name='PAD_1', func=Pin.PASSIVE, do_erc=True),
            Pin(num=2, name='PAD_2', func=Pin.PASSIVE, do_erc=True),
            Pin(num=3, name='PAD_3', func=Pin.PASSIVE, do_erc=True),
            Pin(num=4, name='PAD_4', func=Pin.PASSIVE, do_erc=True),
        ]
    ),
])
