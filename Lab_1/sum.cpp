#include <thread>	// std::thread
#include <iostream> // std::cout this function is similar to printf put output a steam
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

/* 	This would allow us to use std::cout or std::thread without typing that every time. 
*	So, we can type in cout or thread straight away. In fact, we can use anything withing 
*	std library that's related to cpp; e.g. you can use 'using ctype std' and that would 
*	allow you to use all the functions withing the ctype class within the std library.
*/
using namespace std;

// no_thread_sum returns the sum of the values in 'vals', of length 'count'
static double no_thread_sum(double *vals, size_t count)
{
	double sum = 0;
	for (size_t i = 0; i < count; i++) {
		sum += vals[i];
	}
	return sum;
}
 
// structure we're going to use for arguments to our pthread functions
struct thread_args {
	double *vals;
	size_t count;
	double ret;
	thread thread; //Changed this from pthread to cpp thread using std::thread thread.
};

// return the sum of a single slice of values
 void thread_sum_func(struct thread_args * ta)
{
	// struct thread_args *ta = (struct thread_args*)args;
	double sum = 0;

	for (size_t i = 0; i < ta->count; i++) {
		sum += ta->vals[i];
	}
	ta->ret = sum;
}

// Break up 'vals' into a series of slices, and spawn a thread to sum each
// slice, then return the sum of these intermediate sums.
static double thread_sum(double *vals, size_t count, int nthreads)
{
	// How many threads should we create?
	struct thread_args ta[nthreads];
	double sum = 0;
	
	// For simplicity, we're requiring them to all be equal size
	if (count % nthreads) {
		fprintf(stderr, "count %zu must be divisible by %d\n", count, nthreads);
		return 0;
	}

	// Split up the incoming data, and spawn the threads
	for (int i = 0; i < nthreads; i++) {
		ta[i].count = count / nthreads;
		ta[i].vals = &vals[i * count / nthreads];
		ta[i].ret = 0;
		// if (thread_create(&ta[i].thread, NULL, thread_sum_func, &ta[i]) < 0)
		// 	fprintf(stderr, "Could not create thread %d\n", i);
		ta[i].thread = thread(thread_sum_func, &ta[i]);
	}

	// Wait for each thread to finish, and add in its partial sum
	for (int i = 0; i < nthreads; i++) {
		// pthread_join(ta[i].thread, NULL);
		ta[i].thread.join();
		sum += ta[i].ret;
		// delete ta[i].thread; // freeing the heap memory assoiciated with threading
	}
	return sum;
}


int main(int argc, char **argv)
{
	struct timespec start, end;
	size_t count = 1000000000;
	double *vals;
	double n_sum;
	double t_sum;
	uint64_t nanoseconds;
	int nthreads = 4;

	if (argc > 1) {
		count = atoll(argv[1]);
	}
	if (argc > 2) {
		nthreads = atoi(argv[2]);
	}

	/* Allocate some space for our values, and initialise them with some
	 * random data
	 */
	vals = (double*) malloc(sizeof(double) * count);
	if (!vals)
		exit(1);

	for (size_t i = 0; i < count; i++) {
		vals[i] = rand() % 100;
	}

	// Determine the sum using a threaded summing routine
	clock_gettime(CLOCK_MONOTONIC, &start);
	t_sum = thread_sum(vals, count, nthreads);
	clock_gettime(CLOCK_MONOTONIC, &end);
	nanoseconds = (end.tv_sec - start.tv_sec) * 1000000000ULL +
		(end.tv_nsec - start.tv_nsec);
	cout << t_sum << ", " << nthreads << ", ";
	cout << nanoseconds / 1000000 << ", " << count << ", ";

	// Determine the sum using a non-threaded summing routine
	clock_gettime(CLOCK_MONOTONIC, &start);
	n_sum = no_thread_sum(vals, count);
	clock_gettime(CLOCK_MONOTONIC, &end);
	nanoseconds = (end.tv_sec - start.tv_sec) * 1000000000ULL +
		(end.tv_nsec - start.tv_nsec);
	cout << n_sum << ", ";
	cout << nanoseconds / 1000000 << ", " << count << endl;
	free(vals);

	return 0;
}