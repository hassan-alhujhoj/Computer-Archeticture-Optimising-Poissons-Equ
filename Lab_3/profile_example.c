#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void do_work_1(double *array, size_t length)
{
	for (size_t i = 0; i < length; i++) {
		array[i] = sqrt(array[i]);	
	}
}

void do_work_2(double *array, size_t length)
{
	for (size_t i = 0; i < length; i++) {
		array[i] = pow(array[i], 2);
	}
}

int main(int argc, char **argv)
{
	size_t count = 10000000;
	double *array = (double *)malloc(count * sizeof(double));

	if (!array)
		exit(1);

	for (size_t i = 0; i < count; i++) {
		array[i] = i;
	}

	do_work_1(array, count);
	do_work_2(array, count);

	return 0;	
}