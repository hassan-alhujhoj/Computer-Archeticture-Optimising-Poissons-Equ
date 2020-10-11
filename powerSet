#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Oct 12 02:14:00 2020

@author: hassan99
"""


print("make clean")
print("make")

threads = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
powers = range(1, 9)
print("./sum 1 1 > sum.txt")
for thread in threads:
    for power in powers:
        input_sum = 10 ** power
        print("./sum {} {} >> sum.txt".format(input_sum, thread))
        print('echo "Running sum {} {}..."'.format(input_sum, thread))