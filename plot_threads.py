import matplotlib.pyplot as plt
import numpy as np


filename = "sum_results.csv"

data = np.loadtxt(filename, delimiter=',', dtype='int')
nthreads, count, time_ms = data.T

threads = [[], [], [], [], [], [], [], [], [], []]
counts = [[], [], [], [], [], [], [], [], [], []]

for i in range(0, len(nthreads)):
	th = nthreads[i] - 1
	threads[th].append(time_ms[i])
	counts[th].append(count[i])



plt.figure()

for i in range(0, 10):
	plt.plot(counts[i], threads[i])
	
plt.legend(["1 thread", "2 threads", "3 threads", "4 threads", "5 threads", "6 threads", "7 threads", "8 threads", "9 threads", "10 threads"])
plt.show()
