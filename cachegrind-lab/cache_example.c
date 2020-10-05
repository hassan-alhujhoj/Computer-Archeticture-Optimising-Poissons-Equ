/**
 * cache_example
 * Shows both nicely behaved linear memory access, and poorly behaved
 * (non-linear) memory access.
 * Designed to be used with cachegrind to show read cache misses
 * @author Andre Renaud
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Sum a matrix of numbers with linear memory access
 */
int iterate_good(int *buffer, size_t x_max, size_t y_max)
{
	int sum = 0;
	printf("Running good iteration\n");
	for (size_t y = 0; y < y_max; y++) {
		for (size_t x = 0; x < x_max; x++) {
			sum = buffer[y * x_max + x];
		}
	}
	return sum;
}

/**
 * Sum a matrix of numbers with non-linear memory access
 */
int iterate_bad(int *buffer, size_t x_max, size_t y_max)
{
	int sum = 0;
	printf("Running bad iteration\n");
	for (size_t x = 0; x < x_max; x++) {
		for (size_t y = 0; y < y_max; y++) {
			sum = buffer[y * x_max + x];
		}
	}
	return sum;
}

int main(int argc, char **argv)
{
	size_t x_max = 5000;
	size_t y_max = 5000;
	int *buffer = (int *)calloc(x_max * y_max, sizeof(int));
	int good = 0;

	if (argc >= 2) {
		good = strcmp(argv[1], "good") == 0;
	}

	if (good)
		iterate_good(buffer, x_max, y_max);
	else
		iterate_bad(buffer, x_max, y_max);

	return 0;
}
