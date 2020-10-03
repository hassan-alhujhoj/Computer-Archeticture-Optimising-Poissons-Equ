#!/bin/bash

# This cmd will output t_sum,nthreads, time, count, n_sum, time, count to a text file

echo "Running sum 10 5..."
./sum 10 5 > sum_results.txt
echo "Running sum 100 5..."
./sum 1000 5 >> sum_results.txt
echo "Running sum 1000 5..."
./sum 10000 5 >> sum_results.txt
echo "Running sum 10000 5..."
./sum 100000 5 >> sum_results.txt
echo "Running sum 100000 5..."
./sum 1000000 5 >> sum_results.txt
echo "Running sum 1000000 5..."
./sum 10000000 5 >> sum_results.txt
echo "Running sum 10000000 5..."
./sum 100000000 5 >> sum_results.txt
echo "Running sum 100000000 5..."
./sum 1000000000 5 >> sum_results.txt
echo "Finished 😊"
