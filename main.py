import math
import numpy as np
from timebudget import timebudget
iterations_count = round(1e7)
def complex_operation(input_index):
   print("Complex operation. Input index: {:2d}".format(input_index))
   [math.exp(i) * math.sinh(i) for i in [1] * iterations_count]
  
@timebudget
def run_complex_operations(operation, input):
   for i in input:
      operation(i) 

input = range(10)
run_complex_operations(complex_operation, input) 