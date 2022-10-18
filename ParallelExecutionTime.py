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

plt.figure(1)
plt.plot(noOfCores, timeOfCores)
plt.title("Execution Time against the Number of Cores Used")
plt.xlabel('Number of Cores Used')
plt.ylabel('Execution Time (s)')
plt.xticks([1,2,3,4])
plt.savefig('times.png')

speedup = [0,0,0,0]
for i in range(4):
  speedup[i] = timeOfCores[0]/timeOfCores[i]
plt.figure(2)
plt.plot(noOfCores, speedup)
plt.title("Speedup against the Number of Cores Used")
plt.xlabel('Number of Cores Used')
plt.ylabel('Speedup')
plt.xticks([1,2,3,4])
plt.savefig('speedup.png')
