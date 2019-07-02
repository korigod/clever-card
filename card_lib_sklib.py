from skidl import Pin, Part, SchLib, SKIDL, TEMPLATE

SKIDL_lib_version = '0.0.1'

card_lib = SchLib(tool=SKIDL).add_parts(*[
        Part(name='Battery_Cell',dest=TEMPLATE,tool=SKIDL,keywords='battery cell',description='Single-cell battery',ref_prefix='BT',num_units=1,do_erc=True,footprint='footprints:BatteryHolder_Keystone_3026_1x2016',pins=[
            Pin(num='1',name='+',func=Pin.PASSIVE,do_erc=True),
            Pin(num='2',name='-',func=Pin.PASSIVE,do_erc=True)]),
        Part(name='EFM32LG232F64',dest=TEMPLATE,tool=SKIDL,keywords='Energy Micro energymicro EFM32 32bit ARM Cortex Flash Microcontroller MCU',iter_num=64,description='Family: Leopard Gecko, Package: QFP64, Package size: 10mm x 10mm',ref_prefix='U',num_units=4,fplist=['QFP64'],do_erc=True,footprint='Package_QFP:LQFP-64_10x10mm_P0.5mm',pins=[
            Pin(num='1',name='PA0/GPIO_EM4WU0/I2C0_SDA_#0/LEU0_RX_#4/PRS_CH0_#0/TIM0_CC0_#0-1-4',func=Pin.PASSIVE,do_erc=True),
            Pin(num='2',name='PA1/CMU_CLK1_#0/I2C0_SCL_#0/PRS_CH1_#0/TIM0_CC1_#0-1',func=Pin.PASSIVE,do_erc=True),
            Pin(num='3',name='PA2/CMU_CLK0_#0/ETM_TD0_#3/TIM0_CC2_#0-1',func=Pin.PASSIVE,do_erc=True),
            Pin(num='4',name='PA3/ETM_TD1_#3/LES_ALTEX2/TIM0_CDTI0_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='5',name='PA4/ETM_TD2_#3/LES_ALTEX3/TIM0_CDTI1_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='6',name='PA5/ETM_TD3_#3/LES_ALTEX4/LEU1_TX_#1/TIM0_CDTI2_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='17',name='PA8/TIM2_CC0_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='18',name='PA9/TIM2_CC1_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='19',name='PA10/TIM2_CC2_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='15',name='PB7/LFXTAL_P/TIM1_CC0_#3/US0_TX_#4/US1_CLK_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='16',name='PB8/LFXTAL_N/TIM1_CC1_#3/US0_RX_#4/US1_CS_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='21',name='PB11/DAC0_OUT0/OPAMP_OUT0/I2C1_SDA_#1/LETIM0_OUT0_#1/TIM1_CC2_#3',func=Pin.PASSIVE,do_erc=True),
            Pin(num='24',name='PB13/HFXTAL_P/LEU0_TX_#1/US0_CLK_#4-5',func=Pin.PASSIVE,do_erc=True),
            Pin(num='25',name='PB14/HFXTAL_N/LEU0_RX_#1/US0_CS_#4-5',func=Pin.PASSIVE,do_erc=True),
            Pin(num='9',name='PC0/ACMP0_CH0/DAC0_OUT0ALT/OPAMP_OUT0ALT_#0/I2C0_SDA_#4/LES_CH0/PCNT0_S0IN_#2/PRS_CH2_#0/TIM0_CC1_#4/US0_TX_#5/US1_TX_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='10',name='PC1/ACMP0_CH1/DAC0_OUT0ALT/OPAMP_OUT0ALT_#1/I2C0_SCL_#4/LES_CH1/PCNT0_S1IN_#2/PRS_CH3_#0/TIM0_CC2_#4/US0_RX_#5/US1_RX_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='11',name='PC2/ACMP0_CH2/DAC0_OUT0ALT/OPAMP_OUT0ALT_#2/LES_CH2/TIM0_CDTI0_#4/US2_TX',func=Pin.PASSIVE,do_erc=True),
            Pin(num='12',name='PC3/ACMP0_CH3/DAC0_OUT0ALT/OPAMP_OUT0ALT_#3/LES_CH3/TIM0_CDTI1_#4/US2_RX',func=Pin.PASSIVE,do_erc=True),
            Pin(num='13',name='PC4/ACMP0_CH4/DAC0_P0/OPAMP_P0/I2C1_SDA_#0/LES_CH4/LETIM0_OUT0_#3/PCNT1_S0IN/TIM0_CDTI2_#4/US2_CLK',func=Pin.PASSIVE,do_erc=True),
            Pin(num='14',name='PC5/ACMP0_CH5/DAC0_N0/OPAMP_N0/I2C1_SCL/LES_CH5/LETIM0_OUT1_#3/PCNT1_S1IN/US2_CS',func=Pin.PASSIVE,do_erc=True),
            Pin(num='37',name='PC6/ACMP0_CH6/ETM_TCLK_#2/I2C0_SDA_#2/LES_CH6/LEU1_TX_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='38',name='PC7/ACMP0_CH7/ETM_TD0_#2/I2C0_SCL_#2/LES_CH7/LEU1_RX',func=Pin.PASSIVE,do_erc=True),
            Pin(num='41',name='PC8/ACMP1_CH0/LES_CH8/TIM2_CC0_#2/US0_CS_#2',func=Pin.PASSIVE,do_erc=True),
            Pin(num='42',name='PC9/ACMP1_CH1/GPIO_EM4WU2/LES_CH9/TIM2_CC1_#2/US0_CLK_#2',func=Pin.PASSIVE,do_erc=True),
            Pin(num='43',name='PC10/ACMP1_CH2/LES_CH10/TIM2_CC2_#2/US0_RX_#2',func=Pin.PASSIVE,do_erc=True),
            Pin(num='44',name='PC11/ACMP1_CH3/LES_CH11/US0_TX_#2',func=Pin.PASSIVE,do_erc=True),
            Pin(num='45',name='PC12/ACMP1_CH4/CMU_CLK0_#1/DAC0_OUT1ALT/OPAMP_OUT1ALT_#0/LES_CH12',func=Pin.PASSIVE,do_erc=True),
            Pin(num='46',name='PC13/ACMP1_CH5/DAC0_OUT1ALT/OPAMP_OUT1ALT_#1/LES_CH13/PCNT0_S0IN_#0/TIM0_CDTI0_#1-3/TIM1_CC0_#0/TIM1_CC2_#4',func=Pin.PASSIVE,do_erc=True),
            Pin(num='47',name='PC14/ACMP1_CH6/DAC0_OUT1ALT/OPAMP_OUT1ALT_#2/LES_CH14/PCNT0_S1IN_#0/TIM0_CDTI1_#1-3/TIM1_CC1_#0/US0_CS_#3',func=Pin.PASSIVE,do_erc=True),
            Pin(num='48',name='PC15/ACMP1_CH7/DAC0_OUT1ALT/OPAMP_OUT1ALT_#3/DBG_SWO_#1/LES_CH15/TIM0_CDTI2_#1-3/TIM1_CC2_#0/US0_CLK_#3',func=Pin.PASSIVE,do_erc=True),
            Pin(num='28',name='PD0/ADC0_CH0/DAC0_OUT0ALT/OPAMP_OUT0ALT_#4/DAC0_OUT2/OPAMP_OUT2_#1/PCNT2_S0IN_#0/US1_TX_#1',func=Pin.PASSIVE,do_erc=True),
            Pin(num='29',name='PD1/ADC0_CH1/DAC0_OUT1ALT/OPAMP_OUT1ALT_#4/DBG_SWO_#2/PCNT2_S1IN_#0/TIM0_CC0_#3/US1_RX_#1',func=Pin.PASSIVE,do_erc=True),
            Pin(num='30',name='PD2/ADC0_CH2/DBG_SWO_#3/TIM0_CC1_#3/US1_CLK_#1',func=Pin.PASSIVE,do_erc=True),
            Pin(num='31',name='PD3/ADC0_CH3/DAC0_N2/OPAMP_N2/ETM_TD1_#0-2/TIM0_CC2_#3/US1_CS_#1',func=Pin.PASSIVE,do_erc=True),
            Pin(num='32',name='PD4/ADC0_CH4/DAC0_P2/OPAMP_P2/ETM_TD2_#0-2/LEU0_TX_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='33',name='PD5/ADC0_CH5/DAC0_OUT2/OPAMP_OUT2_#0/ETM_TD3_#0-2/LEU0_RX_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='34',name='PD6/ACMP0_O_#2/ADC0_CH6/DAC0_P1/OPAMP_P1/ETM_TD0_#0/I2C0_SDA_#1/LES_ALTEX0/LETIM0_OUT0_#0/PCNT0_S0IN_#3/TIM1_CC0_#4/US1_RX_#2',func=Pin.PASSIVE,do_erc=True),
            Pin(num='35',name='PD7/ACMP1_O_#2/ADC0_CH7/CMU_CLK0_#2/DAC0_N1/OPAMP_N1/ETM_TCLK_#0/I2C0_SCL_#1/LES_ALTEX1/LETIM0_OUT1_#0/PCNT0_S1IN_#3/TIM1_CC1_#4/US1_TX_#2',func=Pin.PASSIVE,do_erc=True),
            Pin(num='36',name='PD8/BU_VIN/CMU_CLK1_#1',func=Pin.PASSIVE,do_erc=True),
            Pin(num='57',name='PE8/PCNT2_S0IN_#1/PRS_CH3_#1',func=Pin.PASSIVE,do_erc=True),
            Pin(num='58',name='PE9/PCNT2_S1IN_#1',func=Pin.PASSIVE,do_erc=True),
            Pin(num='59',name='PE10/TIM1_CC0_#1/US0_TX_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='60',name='PE11/LES_ALTEX5/TIM1_CC1_#1/US0_RX_#0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='61',name='PE12/CMU_CLK1_#2/I2C0_SDA_#6/LES_ALTEX6/TIM1_CC2_#1/US0_CLK_#0/US0_RX_#3',func=Pin.PASSIVE,do_erc=True),
            Pin(num='62',name='PE13/ACMP0_O_#0/GPIO_EM4WU5/I2C0_SCL_#6/LES_ALTEX7/US0_CS_#0/US0_TX_#3',func=Pin.PASSIVE,do_erc=True),
            Pin(num='63',name='PE14/LEU0_TX_#2/TIM3_CC0',func=Pin.PASSIVE,do_erc=True),
            Pin(num='64',name='PE15/LEU0_RX_#2/TIM3_CC1',func=Pin.PASSIVE,do_erc=True),
            Pin(num='49',name='PF0/DBG_SWCLK_#0-1-2-3/I2C0_SDA_#5/LETIM0_OUT0_#2/LEU0_TX_#3/TIM0_CC0_#5/US1_CLK_#2',func=Pin.PASSIVE,do_erc=True),
            Pin(num='50',name='PF1/DBG_SWDIO_#0-1-2-3/GPIO_EM4WU3/I2C0_SCL_#5/LETIM0_OUT1_#2/LEU0_RX_#3/TIM0_CC1_#5/US1_CS_#2',func=Pin.PASSIVE,do_erc=True),
            Pin(num='51',name='PF2/ACMP1_O_#0/DBG_SWO_#0/GPIO_EM4WU4/LEU0_TX_#4/TIM0_CC2_#5',func=Pin.PASSIVE,do_erc=True),
            Pin(num='52',name='PF3/ETM_TD3_#1/PRS_CH0_#1/TIM0_CDTI0_#2-5',func=Pin.PASSIVE,do_erc=True),
            Pin(num='53',name='PF4/PRS_CH1_#1/TIM0_CDTI1_#2-5',func=Pin.PASSIVE,do_erc=True),
            Pin(num='54',name='PF5/PRS_CH2_#1/TIM0_CDTI2_#2-5',func=Pin.PASSIVE,do_erc=True),
            Pin(num='27',name='AVDD_0',func=Pin.PWRIN,do_erc=True),
            Pin(num='23',name='AVDD_1',func=Pin.PWRIN,do_erc=True),
            Pin(num='40',name='DECOUPLE',func=Pin.PWRIN,do_erc=True),
            Pin(num='7',name='IOVDD_0',func=Pin.PWRIN,do_erc=True),
            Pin(num='26',name='IOVDD_3',func=Pin.PWRIN,do_erc=True),
            Pin(num='55',name='IOVDD_5',func=Pin.PWRIN,do_erc=True),
            Pin(num='20',name='~RESET~',func=Pin.PASSIVE,do_erc=True),
            Pin(num='39',name='VDD_DREG',func=Pin.PWRIN,do_erc=True),
            Pin(num='8',name='VSS',func=Pin.PWRIN,do_erc=True),
            Pin(num='22',name='VSS',func=Pin.PWRIN,do_erc=True),
            Pin(num='56',name='VSS',func=Pin.PWRIN,do_erc=True)]),
        Part(name='Crystal',dest=TEMPLATE,tool=SKIDL,keywords='quartz ceramic resonator oscillator',description='Two pin crystal',ref_prefix='Y',num_units=1,fplist=['Crystal*'],do_erc=True,footprint='Crystal:Crystal_SMD_3215-2Pin_3.2x1.5mm',pins=[
            Pin(num='1',name='1',func=Pin.PASSIVE,do_erc=True),
            Pin(num='2',name='2',func=Pin.PASSIVE,do_erc=True)]),
        Part(name='LSM6DS3',dest=TEMPLATE,tool=SKIDL,keywords='Accelerometer Gyroscope MEMS',description='I2C/SPI, iNEMO inertial module: always-on 3D accelerometer and 3D gyroscope',ref_prefix='U',num_units=1,fplist=['LGA*3x2.5mm*P0.5mm*LayoutBorder3x4y*'],do_erc=True,footprint='footprints:LSM6DS3',pins=[
            Pin(num='1',name='SDO/SA0',func=Pin.BIDIR,do_erc=True),
            Pin(num='10',name='NC',func=Pin.NOCONNECT,do_erc=True),
            Pin(num='11',name='NC',func=Pin.NOCONNECT,do_erc=True),
            Pin(num='12',name='CS',do_erc=True),
            Pin(num='13',name='SCL',do_erc=True),
            Pin(num='14',name='SDA',func=Pin.BIDIR,do_erc=True),
            Pin(num='2',name='SDX',func=Pin.BIDIR,do_erc=True),
            Pin(num='3',name='SCX',do_erc=True),
            Pin(num='4',name='INT1',func=Pin.OUTPUT,do_erc=True),
            Pin(num='5',name='VDDIO',func=Pin.PWRIN,do_erc=True),
            Pin(num='6',name='GND',func=Pin.PWRIN,do_erc=True),
            Pin(num='7',name='GND',func=Pin.PWRIN,do_erc=True),
            Pin(num='8',name='VDD',func=Pin.PWRIN,do_erc=True),
            Pin(num='9',name='INT2',func=Pin.OUTPUT,do_erc=True)]),
        Part(name='TOUCH_PAD',dest=TEMPLATE,tool=SKIDL,keywords='capacitive sense touch pad',description='Single capacitive sense touch pad',ref_prefix='TS',do_erc=True,footprint='footprints:TouchPad_D8.0mm',pins=[
            Pin(num=1,name='PAD',func=Pin.PASSIVE,do_erc=True)]),
        Part(name='TOUCH_SLIDER_4PADS',dest=TEMPLATE,tool=SKIDL,keywords='capacitive sense touch slider',description='Capacitive sense touch slider with 4 pads',ref_prefix='TS',do_erc=True,footprint='footprints:TouchSlider-4_42x7mm',pins=[
            Pin(num=1,name='PAD_1',func=Pin.PASSIVE,do_erc=True),
            Pin(num=2,name='PAD_2',func=Pin.PASSIVE,do_erc=True),
            Pin(num=3,name='PAD_3',func=Pin.PASSIVE,do_erc=True),
            Pin(num=4,name='PAD_4',func=Pin.PASSIVE,do_erc=True)]),
        Part(name='POGO_10PADS',dest=TEMPLATE,tool=SKIDL,keywords='capacitive sense touch pad',description='Single capacitive sense touch pad',ref_prefix='J',do_erc=True,footprint='footprints:TestPoints_2x05_P2.54_D1.7',pins=[
            Pin(num=1,name='PAD1',func=Pin.PASSIVE,do_erc=True),
            Pin(num=2,name='PAD2',func=Pin.PASSIVE,do_erc=True),
            Pin(num=3,name='PAD3',func=Pin.PASSIVE,do_erc=True),
            Pin(num=4,name='PAD4',func=Pin.PASSIVE,do_erc=True),
            Pin(num=5,name='PAD5',func=Pin.PASSIVE,do_erc=True),
            Pin(num=6,name='PAD6',func=Pin.PASSIVE,do_erc=True),
            Pin(num=7,name='PAD7',func=Pin.PASSIVE,do_erc=True),
            Pin(num=8,name='PAD8',func=Pin.PASSIVE,do_erc=True),
            Pin(num=9,name='PAD9',func=Pin.PASSIVE,do_erc=True),
            Pin(num=10,name='PAD10',func=Pin.PASSIVE,do_erc=True)]),
        Part(name='C',dest=TEMPLATE,tool=SKIDL,keywords='cap capacitor',description='Unpolarized capacitor',ref_prefix='C',num_units=1,fplist=['C_*'],do_erc=True,footprint='Capacitor_SMD:C_0603_1608Metric',pins=[
            Pin(num='1',name='~',func=Pin.PASSIVE,do_erc=True),
            Pin(num='2',name='~',func=Pin.PASSIVE,do_erc=True)]),
        Part(name='Ferrite_Bead_Small',dest=TEMPLATE,tool=SKIDL,keywords='L ferrite bead inductor filter',description='Ferrite bead, small symbol',ref_prefix='FB',num_units=1,fplist=['Inductor_*', 'L_*', '*Ferrite*'],do_erc=True,footprint='Inductor_SMD:L_0603_1608Metric',pins=[
            Pin(num='1',name='~',func=Pin.PASSIVE,do_erc=True),
            Pin(num='2',name='~',func=Pin.PASSIVE,do_erc=True)]),
        Part(name='R',dest=TEMPLATE,tool=SKIDL,keywords='R res resistor',description='Resistor',ref_prefix='R',num_units=1,fplist=['R_*'],do_erc=True,footprint='Resistor_SMD:R_0603_1608Metric',pins=[
            Pin(num='1',name='~',func=Pin.PASSIVE,do_erc=True),
            Pin(num='2',name='~',func=Pin.PASSIVE,do_erc=True)]),
        Part(name='Q_PMOS_GSD',dest=TEMPLATE,tool=SKIDL,keywords='transistor PMOS P-MOS P-MOSFET',description='P-MOSFET transistor, gate/source/drain',ref_prefix='Q',num_units=1,do_erc=True,footprint='Package_TO_SOT_SMD:SOT-23',pins=[
            Pin(num='1',name='G',do_erc=True),
            Pin(num='2',name='S',func=Pin.PASSIVE,do_erc=True),
            Pin(num='3',name='D',func=Pin.PASSIVE,do_erc=True)]),
        Part(name='LED_Dual_AACC',dest=TEMPLATE,tool=SKIDL,keywords='LED diode bicolor dual',description='Dual LED, cathodes on pins 3 and 4',ref_prefix='D',num_units=1,fplist=['LED*', 'LED_SMD:*', 'LED_THT:*'],do_erc=True,footprint='footprints:LED_DUAL_0606',pins=[
            Pin(num='1',name='A1',do_erc=True),
            Pin(num='2',name='A2',do_erc=True),
            Pin(num='3',name='K1',do_erc=True),
            Pin(num='4',name='K2',do_erc=True)])])