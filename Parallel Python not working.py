import math
import time
import multiprocessing
from multiprocessing import Process
from multiprocessing import Pool

def CPU_extensive(cores):
    """
    Doing mathematical calculations
    """
    for i in range(round(10000000/cores)):
      1+1
    print('Done')

start1 = time.perf_counter()
CPU_extensive(1)
finish1 = time.perf_counter()
print('the program has finished in {} seconds'.format(finish1 - start1))
if __name__ == '__main__': 
  start = time.perf_counter()

  processes = []
  pool = Pool()
  result1 = pool.apply_async(CPU_extensive, [2])
  result2 = pool.apply_async(CPU_extensive, [2])    
  answer1 = result1.get(timeout=10)
  answer2 = result2.get(timeout=10)
      
  finish = time.perf_counter()
  print('the program has finished in {} seconds'.format(finish - start))