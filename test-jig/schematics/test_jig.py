import skidl as sk
import os
import sys
from itertools import zip_longest
from skidl import Bus, ERC, generate_netlist, Net, Part, POWER, subcircuit, TEMPLATE


sk.lib_search_paths[sk.KICAD].extend([
    '/usr/share/kicad/library'
])


pcb_connector = Part('Connector_Generic', 'Conn_02x05_Odd_Even', footprint='footprints:PinHeader_2x05_P2.54mm_D0.7mm_Vertical')
debugger_connector = Part('Connector_Generic', 'Conn_02x10_Odd_Even', footprint='Connector_IDC:IDC-Header_2x10_P2.54mm_Vertical')
auxillary_connector = Part('Connector_Generic', 'Conn_02x05_Odd_Even', footprint='Connector_PinHeader_2.54mm:PinHeader_2x05_P2.54mm_Vertical')

vdd = Net('VDD')
gnd = Net('GND')
reset = Net('RESET')
usart_rx = Net('USART_RX')
usart_tx = Net('USART_TX')
dbg_swo = Net('DBG_SWO')
dbg_swdio = Net('DBG_SWDIO')
dbg_swclk = Net('DBG_SWCLK')
etm_tclk = Net('ETM_TCLK')
etm_td0 = Net('ETM_TD0')

pcb_connector[1] += vdd
pcb_connector[2] += gnd
pcb_connector[3] += reset
pcb_connector[4] += usart_rx
pcb_connector[5] += usart_tx
pcb_connector[6] += dbg_swo
pcb_connector[7] += dbg_swdio
pcb_connector[8] += dbg_swclk
pcb_connector[9] += etm_tclk
pcb_connector[10] += etm_td0

auxillary_connector[:] += pcb_connector[:]

debugger_connector[1] += vdd
debugger_connector[7] += dbg_swdio
debugger_connector[9] += dbg_swclk
debugger_connector[13] += dbg_swo
debugger_connector[15] += reset
debugger_connector[4, 6, 8, 10, 12, 14, 16, 18, 20] += gnd  # Pin 18 is Cable detect, should be pulled low

# Not connected
debugger_connector[2].do_erc = False
debugger_connector[3].do_erc = False
debugger_connector[5].do_erc = False
debugger_connector[11].do_erc = False
debugger_connector[17].do_erc = False
debugger_connector[19].do_erc = False

ERC()
generate_netlist()
