/// \brief Test program for solving Poisson's equation using Jacobi relaxation
/// \author M. P. Hayes UCECE

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include "poisson.hpp"
#include <inttypes.h>

// Modified by Tim & Hassan
using namespace std;

int main (int argc, char *argv[])
{
    struct timespec start, end;
	uint64_t nanoseconds;

    double *source;
    double *potential;
    unsigned int N;
    unsigned int numiters;
    unsigned int numcores;
    unsigned int xsize;
    unsigned int ysize;
    unsigned int zsize;    
    double delta = 0.1;

    if (argc < 3)
    {
        fprintf (stderr, "Usage: %s size numiters\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    xsize = N;
    ysize = N;
    zsize = N;

    numiters = atoi(argv[2]);

    if (argc > 3)
        numcores = atoi(argv[3]);
    else
        numcores = 0;

    source = (double *)calloc(xsize * ysize * zsize, sizeof(*source));
    potential = (double *)calloc(xsize * ysize * zsize, sizeof(*potential));

    source[((zsize / 2 * ysize) + ysize / 2) * xsize + xsize / 2] = 1.0;    
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    poisson_dirichlet(source, potential, 1, xsize, ysize, zsize, delta,
                      numiters, numcores);
	clock_gettime(CLOCK_MONOTONIC, &end);
	nanoseconds = (end.tv_sec - start.tv_sec) * 1000000000ULL +
		(end.tv_nsec - start.tv_nsec);
	
	printf("Took %" PRIu64 " ms\n",
		nanoseconds / 1000000);
    
    // clear data in output.txt
    ofstream ofs;
    ofs.open("output.txt", ofstream::out | ofstream::trunc);
    ofs.close();
    // write data in output.txt
    FILE *output = fopen("output.txt", "w+");
	if(!output) {
        perror("\nFile opening failed!\n");
        cout << EXIT_FAILURE;
    }else{
		for(unsigned int z = 0; z < zsize; z++){
				for(unsigned int y = 0; y < ysize; y++){
					for (unsigned int x = 0; x < xsize; x++){
						double result = potential[((z * ysize) + y) * xsize + x]; //access x, y or z
						fprintf(output,"%d, %d, %d, %.10lf\n"
                                , x, y, z, result);
				}
			} 
		}
	}
	if (ferror(output))
		puts("\nI/O error when reading\n");
	else if (feof(output))
		puts("\nEnd of file reached successfully!\n");
	fclose(output);
    return 0;
}
