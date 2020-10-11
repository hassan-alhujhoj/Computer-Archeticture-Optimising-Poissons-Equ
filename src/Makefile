all: poisson_test

poisson_test: poisson_test.cpp poisson.cpp
	g++ -O3 -std=c++2a -Wall -g3 -o $@ $^ -lpthread

clean:
	rm -f poisson_test

