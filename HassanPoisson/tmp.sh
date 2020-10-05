#!/bin/bash

# This cmd will output t_sum,nthreads, time, count, n_sum, time, count to a text file

echo "Running sum 10 5..."
poisson_test 100 100 > results.txt
echo "Finished 😊"