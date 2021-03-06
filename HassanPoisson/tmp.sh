#!/bin/bash

# This cmd will output t_sum,nthreads, time, count, n_sum, time, count to a text file

make clean
make all
./poisson_test 101 10 1
mv gmon.out gmon.sum
./poisson_test 101 10 1
echo "1 101 complete"
./poisson_test 201 10 1
echo "1 201 complete"
./poisson_test 301 10 1
echo "1 301 complete"
./poisson_test 401 10 1
echo "1 401 complete"
./poisson_test 501 10 1
echo "1 501 complete"
./poisson_test 601 10 1
echo "1 601 complete"
./poisson_test 701 10 1
echo "1 701 complete"
./poisson_test 801 10 1
echo "1 801 complete"
./poisson_test 901 10 1
echo "1 901 complete"
./poisson_test 101 10 2
echo "2 101 complete"
./poisson_test 201 10 2
echo "2 201 complete"
./poisson_test 301 10 2
echo "2 301 complete"
./poisson_test 401 10 2
echo "2 401 complete"
./poisson_test 501 10 2
echo "2 501 complete"
./poisson_test 601 10 2
echo "2 601 complete"
./poisson_test 701 10 2
echo "2 701 complete"
./poisson_test 801 10 2
echo "2 801 complete"
./poisson_test 901 10 2
echo "2 901 complete"
./poisson_test 101 10 4
echo "4 101 complete"
./poisson_test 201 10 4
echo "4 201 complete"
./poisson_test 301 10 4
echo "4 301 complete"
./poisson_test 401 10 4
echo "4 401 complete"
./poisson_test 501 10 4
echo "4 501 complete"
./poisson_test 601 10 4
echo "4 601 complete"
./poisson_test 701 10 4
echo "4 701 complete"
./poisson_test 801 10 4
echo "4 801 complete"
./poisson_test 901 10 4
echo "4 901 complete"
./poisson_test 101 10 8
echo "8 101 complete"
./poisson_test 201 10 8
echo "8 201 complete"
./poisson_test 301 10 8
echo "8 301 complete"
./poisson_test 401 10 8
echo "8 401 complete"
./poisson_test 501 10 8
echo "8 501 complete"
./poisson_test 601 10 8
echo "8 601 complete"
./poisson_test 701 10 8
echo "8 701 complete"
./poisson_test 801 10 8
echo "8 801 complete"
./poisson_test 901 10 8
echo "8 901 complete"
./poisson_test 101 10 16
echo "16 101 complete"
./poisson_test 201 10 16
echo "16 201 complete"
./poisson_test 301 10 16
echo "16 301 complete"
./poisson_test 401 10 16
echo "16 401 complete"
./poisson_test 501 10 16
echo "16 501 complete"
./poisson_test 601 10 16
echo "16 601 complete"
./poisson_test 701 10 16
echo "16 701 complete"
./poisson_test 801 10 16
echo "16 801 complete"
./poisson_test 901 10 16
echo "16 901 complete"
./poisson_test 101 10 32
echo "32 101 complete"
./poisson_test 201 10 32
echo "32 201 complete"
./poisson_test 301 10 32
echo "32 301 complete"
./poisson_test 401 10 32
echo "32 401 complete"
./poisson_test 501 10 32
echo "32 501 complete"
./poisson_test 601 10 32
echo "32 601 complete"
./poisson_test 701 10 32
echo "32 701 complete"
./poisson_test 801 10 32
echo "32 801 complete"
./poisson_test 901 10 32
echo "32 901 complete"
gprof main gmon.sum > output.txt
echo "Finished 😊"