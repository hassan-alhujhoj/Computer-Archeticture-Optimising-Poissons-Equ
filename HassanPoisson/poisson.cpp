#include <thread>
#include <iostream>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Modified by Tim & Hassan
using namespace std;

// storing res
#define STORE 	res -= ta.delta * ta.delta * ta.source[((z * ta.ysize) + y) * ta.xsize];\
				res /= 6;\
				ta.potential[((z * ta.ysize) + y) * ta.xsize] = res;
//defining x, y and z bounderies				
#define xMax res += ta.Vbound + ta.input[((z * ta.ysize) + y) * ta.xsize + (x - 1)];
#define xMid res += ta.input[((z * ta.ysize) + y) * ta.xsize + (x + 1)] + ta.input[((z * ta.ysize) + y) * ta.xsize + (x - 1)];
#define xMin res += ta.input[((z * ta.ysize) + y) * ta.xsize + (x + 1)] + ta.Vbound;

#define yMax res += ta.Vbound + ta.input[((z * ta.ysize) + (y - 1)) * ta.xsize + x];
#define yMid res+= ta.input[((z * ta.ysize) + (y + 1)) * ta.xsize + x] + ta.input[((z * ta.ysize) + (y - 1)) * ta.xsize + x]; 
#define yMin res+= ta.input[((z * ta.ysize) + (y + 1)) * ta.xsize + x] + ta.Vbound;

#define zMax res += ta.Vbound + ta.input[(((z - 1) * ta.ysize) + y) * ta.xsize + x];
#define zMid res += ta.input[(((z + 1) * ta.ysize) + y) * ta.xsize + x] + ta.input[(((z - 1) * ta.ysize) + y) * ta.xsize + x];
#define zMin res += ta.input[(((z + 1) * ta.ysize) + y) * ta.xsize + x] + ta.Vbound;

typedef struct thread_args {
	double * __restrict__ source;
	double * __restrict__ potential;
	double Vbound;
	unsigned int xsize; 
	unsigned int ysize; 
	unsigned int zsize; 
	double delta;
	unsigned int numiters; 
	unsigned int numcores;
	double *input;
	size_t size;
	FILE *results;
}arg;

/// Solve Poisson's equation for a rectangular box with Dirichlet
/// boundary conditions on each face.
/// \param source is a pointer to a flattened 3-D array for the source function
/// \param potential is a pointer to a flattened 3-D array for the calculated potential
/// \param Vbound is the potential on the boundary
/// \param xsize is the number of elements in the x-direction
/// \param ysize is the number of elements in the y-direction
/// \param zsize is the number of elements in the z-direction
/// \param delta is the voxel spacing in all directions
/// \param numiters is the number of iterations to perform
/// \param numcores is the number of CPU cores to use.  If 0, an optimal number is chosen
void poisson_dirichlet (double * __restrict__ source,
                        double * __restrict__ potential,
                        double Vbound,
                        unsigned int xsize, unsigned int ysize, unsigned int zsize, double delta,
                        unsigned int numiters, unsigned int numcores)
{
    // source[i, j, k] is accessed with source[((k * ysize) + j) * xsize + i]
    // potential[i, j, k] is accessed with potential[((k * ysize) + j) * xsize + i]    
    size_t size = (size_t)ysize * zsize * xsize * sizeof(double);
	double *input = (double *)malloc(size);
	if (!input) {
		fprintf(stderr, "malloc failure\n");
		return;
	}
	memcpy(input, source, size);
}

void *poissonThreads(void* argument){
	arg ta; // an instant of struct arg
	for (unsigned int iter = 0; iter < ta.numiters; iter++) {
		// X on boundaries, Y and Z not on boundaries
		for (unsigned int z = 1; z < ta.zsize - 1; z++) {
			for (unsigned int y = 1; y < ta.ysize - 1; y++) {
				double res = 0;
			
				// x = 0
				unsigned int x = 0;
				xMin yMid zMid
				STORE
						
				// x = xsize - 1
				x = ta.xsize - 1;
				xMax yMid zMid
				STORE
			}
		}
		// Y on boundaries, X and Z not on boundaries
		for (unsigned int z = 1; z < ta.zsize - 1; z++) {
			for (unsigned int x = 1; x < ta.xsize - 1; x++) {
				double res = 0;
				
				// y = 0
				unsigned int y = 0;
				xMid yMin zMid
				STORE
				
				// y = ysize - 1
				y = ta.ysize - 1;
				xMid yMax zMid
				STORE
			}
		}
		// Z on boundaries, X and Y not on boundaries
		for (unsigned int y = 1; y < ta.ysize - 1; y++) {
			for (unsigned int x = 1; x < ta.xsize - 1; x++) {
				double res = 0;
				
				// z = 0
				unsigned int z = 0;
				xMid xMid zMin
				STORE
				
				// z = zsize - 1
				z = ta.zsize - 1;
				xMid xMid zMax
				STORE
			}
		}
		
		// Corner cases
		// x = y = z = 0
		unsigned int x = 0, y = 0, z = 0;
		double res = 0;
		xMin yMin zMin
		STORE
									
		// x = xsize-1, y = 0, z = 0
		x = ta.xsize - 1; y = 0; z = 0;
		res = 0;
		xMax yMin zMin
		STORE
											
		// x = xsize-1, y = ysize-1, z = 0
		x = ta.xsize - 1; y = ta.ysize - 1; z = 0;
		res = 0;
		xMax yMax zMin
		STORE
					
		// x = xsize-1, y = 0, z = zsize-1
		x = ta.xsize - 1; y = 0; z = ta.zsize - 1;		
		res = 0;
		xMax yMin zMax
		STORE

		// x = xsize-1, y = ysize-1, z = zsize-1
		x = ta.xsize - 1; y = ta.ysize - 1; z = ta.zsize - 1;
		res = 0;
		xMax yMax zMax
		STORE

		// x = 0, y = ysize - 1, z = 0
		x = 0; y = ta.ysize - 1; z = 0;
		res = 0;
		xMin yMax zMin
		STORE
					
		// x = 0, y = ysize - 1, z = zsize - 1
		x = 0; y = ta.ysize - 1; z = ta.zsize - 1;
		res = 0;
		xMin yMax zMax
		STORE
									
		// x = 0, y = 0, z = zsize - 1
		x = 0; y = 0; z = ta.zsize - 1;
		res = 0;
		xMin yMin zMax
		STORE
		
		// Normal cases (non-boundary)
		for (z = 1; z < ta.zsize - 1; z++) {
			for (y = 1; y < ta.ysize - 1; y++) {
				for (x = 1; x < ta.xsize - 1; x++) {
					res = 0;

					res += ta.input[((z * ta.ysize) + y) * ta.xsize + (x + 1)];
					res += ta.input[((z * ta.ysize) + y) * ta.xsize + (x - 1)]; 

					res += ta.input[((z * ta.ysize) + (y + 1)) * ta.xsize + x];
					res += ta.input[((z * ta.ysize) + (y - 1)) * ta.xsize + x];

					res += ta.input[(((z + 1) * ta.ysize) + y) * ta.xsize + x];
					res += ta.input[(((z - 1) * ta.ysize) + y) * ta.xsize + x];
					STORE
				}
			}
		}
		memcpy(ta.input, ta.potential, ta.size);
	}

	// ta.results = fopen("/results.txt","w");
	// if(ta.results == NULL){
	// 	printf("Error!");   
	// 	exit(1);             
	// }
	// for(unsigned int z = 0; z < ta.zsize - 1; z++){
	// 		for(unsigned int y = 0; y < ta.ysize - 1; y++){
	// 			for (unsigned int x = 0; x < ta.xsize - 1; x++){
		// 			unsigned int xOut = ta.potential[x];
		// 			unsigned int yOut = ta.potential[y];
		// 			unsigned int zOut = ta.potential[z];
		// 			fprintf(ta.results,"%d, %d, %d\n",xOut, yOut, zOut);
	// 		}
	// 	}
	// }fclose(ta.results);
	free(ta.input);
	return 0;
}

