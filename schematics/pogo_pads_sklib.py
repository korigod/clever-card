from skidl import Pin, Part, SchLib, SKIDL, TEMPLATE

SKIDL_lib_version = '0.0.1'

pogo_pads_lib = SchLib(tool=SKIDL).add_parts(*[
    Part(
        name='POGO_10PADS',
        dest=TEMPLATE,
        tool=SKIDL,
        description='Single capacitive sense touch pad',
        keywords='capacitive sense touch pad',
        ref_prefix='J',
        do_erc=True,
        pins=[
            Pin(num=i, name=f'PAD{i}', func=Pin.PASSIVE, do_erc=True)
            for i in range(1, 11)
        ]
    )
])
