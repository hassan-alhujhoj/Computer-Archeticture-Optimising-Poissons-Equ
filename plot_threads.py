import matplotlib.pyplot as plt
import numpy as np


filename = "Lab_1/sum.txt"

data = np.loadtxt(filename, delimiter=',', dtype='float')
t_sum, nthreads, time1, count1, n_sum, time2, count2 = data.T

threads = [[], [], [], [], [], [], [], [], [], []]
counts = [[], [], [], [], [], [], [], [], [], []]

for i in range(0, len(nthreads)):
	th = int(nthreads[i] - 1)
	threads[th].append(time1[i])
	counts[th].append(count1[i])



plt.figure()

for i in range(0, 10):
	plt.plot(counts[i], threads[i])
	
plt.legend(["1 thread", "2 threads", "3 threads", "4 threads", "5 threads", "6 threads", "7 threads", "8 threads", "9 threads", "10 threads"])
plt.show()
