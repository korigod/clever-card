import math
from constraint import Problem

from led_pattern import generate

led_coords = generate(9, 1, 8, math.pi / 6)
p = Problem()
a = [10, 20, 30]
p.addVariables(['A_{}'.format(i) for i in range(1, 9)], range(49))
p.addVariables(['K1_{}'.format(i) for i in range(1, 12)], range(49))
p.addVariables(['K2_{}'.format(i) for i in range(1, 12)], range(49))
# p.addConstraint()
p.getSolutions()
