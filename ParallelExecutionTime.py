import math
import time
import multiprocessing
from multiprocessing import Process
from multiprocessing import Pool
import matplotlib.pyplot as plt

def Sleeping():
    time.sleep(2)

if __name__ == '__main__':
  noOfCores = [1,2,3,4]
  timeOfCores = [0,0,0,0]
  pool = Pool()
  count = 0
  for n in noOfCores:
    noOfIterations = round(20/n)
    start = time.perf_counter()
    for i in range(noOfIterations):
      for j in range(n):
        result = pool.apply_async(Sleeping)
      for j in range(n):
        answer = result.get(timeout=10)
    finish = time.perf_counter()
    total = finish - start
    timeOfCores[count] = total
    print('the program has finished in {} seconds'.format(total))
    count = count+1


plt.plot(noOfCores, timeOfCores)
plt.title("Number of Cores vs the Execution Time")
plt.xlabel('Number of Cores')
plt.ylabel('Execution Time')
plt.savefig('times.png')