#include <thread>
#include <iostream>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "poisson.hpp"

// Modified by Tim & Hassan
using namespace std;
// storing res
#define STORE 	res -= ta->delta * ta->delta * ta->source[((z * ta->ysize) + y) * ta->xsize];\
				res /= 6;\
				ta->potential[((z * ta->ysize) + y) * ta->xsize] = res;
//defining x, y and z bounderies				
#define xMax res += ta->Vbound + ta->input[((z * ta->ysize) + y) * ta->xsize + (x - 1)];
#define xMid res += ta->input[((z * ta->ysize) + y) * ta->xsize + (x + 1)] + ta->input[((z * ta->ysize) + y) * ta->xsize + (x - 1)];
#define xMin res += ta->input[((z * ta->ysize) + y) * ta->xsize + (x + 1)] + ta->Vbound;

#define yMax res += ta->Vbound + ta->input[((z * ta->ysize) + (y - 1)) * ta->xsize + x];
#define yMid res+= ta->input[((z * ta->ysize) + (y + 1)) * ta->xsize + x] + ta->input[((z * ta->ysize) + (y - 1)) * ta->xsize + x]; 
#define yMin res+= ta->input[((z * ta->ysize) + (y + 1)) * ta->xsize + x] + ta->Vbound;

#define zMax res += ta->Vbound + ta->input[(((z - 1) * ta->ysize) + y) * ta->xsize + x];
#define zMid res += ta->input[(((z + 1) * ta->ysize) + y) * ta->xsize + x] + ta->input[(((z - 1) * ta->ysize) + y) * ta->xsize + x];
#define zMin res += ta->input[(((z + 1) * ta->ysize) + y) * ta->xsize + x] + ta->Vbound;

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
	arg ta = (arg){.delta = delta, .xsize = xsize, .ysize = ysize, .zsize = zsize, .input = input, .potential = potential, .source = source, .numiters = numiters, .Vbound = 1};
	thread thread_x;
	thread(poissonThreads, &ta);
	thread_x.join();

	FILE *results;
	results = fopen("results.txt","w");
	if(results == NULL){
		printf("Error!");   
		exit(1);             
	}
	for(unsigned int z = 0; z < zsize - 1; z++){
			for(unsigned int y = 0; y < ysize - 1; y++){
				for (unsigned int x = 0; x < xsize - 1; x++){
					double result = potential[((z * ysize) + y) * xsize + x]; //access x, y or z
					fprintf(results,"%.10lf\n",result);
			}
		} 
	}fclose(results);
}

void *poissonThreads(void* argument){
	printf("You're in poissonThreads()\n");
	arg *ta = (arg*) argument;
	for (unsigned int iter = 0; iter < ta->numiters; iter++) {
		// X on boundaries, Y and Z not on boundaries. Case: x' y z
		for (unsigned int z = 1; z < ta->zsize - 1; z++) {
			for (unsigned int y = 1; y < ta->ysize - 1; y++) {
				double res = 0;
			
				// x = 0
				unsigned int x = 0;
				xMin yMid zMid
				STORE
						
				res = 0;
				// x = xsize - 1
				x = ta->xsize - 1;
				xMax yMid zMid
				STORE
			}
		}
		// Y on boundaries, X and Z not on boundaries. Case: x y' z
		for (unsigned int z = 1; z < ta->zsize - 1; z++) {
			for (unsigned int x = 1; x < ta->xsize - 1; x++) {
				double res = 0;
				
				// y = 0
				unsigned int y = 0;
				xMid yMin zMid
				STORE
				
				res = 0;
				// y = ysize - 1
				y = ta->ysize - 1;
				xMid yMax zMid
				STORE
			}
		}
		// Z on boundaries, X and Y not on boundaries. Case: x y z'
		for (unsigned int y = 1; y < ta->ysize - 1; y++) {
			for (unsigned int x = 1; x < ta->xsize - 1; x++) {
				double res = 0;
				
				// z = 0
				unsigned int z = 0;
				xMid yMid zMin
				STORE
				
				res = 0;
				// z = zsize - 1
				z = ta->zsize - 1;
				xMid yMid zMax
				STORE
			}
		}
		// Y and Z on boundaries, X not on boundaries. Case: x y' z'
		for (unsigned int x = 1; x < ta->xsize - 1; x++) {
			double res = 0;
		
			// y = 0, z = 0
			unsigned int y = 0;
			unsigned int z = 0;
			xMid yMin zMin
			STORE
					
			res = 0;
			// y = xsize - 1
			// z = zsize - 1
			y = ta->ysize - 1;
			z = ta->zsize - 1;
			xMid yMax zMax
			STORE
		}
		// X and Z on boundaries, Y not on boundaries. Case: x' y z'
		for (unsigned int y = 1; y < ta->ysize - 1; y++) {
			double res = 0;
		
			// x = 0, z = 0
			unsigned int x = 0;
			unsigned int z = 0;
			xMin yMid zMin
			STORE
					
			res = 0;
			// x = xsize - 1
			// z = zsize - 1
			x = ta->xsize - 1;
			z = ta->zsize - 1;
			xMax yMid zMax
			STORE
		}
		// X and Y on boundaries, Z not on boundaries. Case: x' y' z
		for (unsigned int z = 1; z < ta->zsize - 1; z++) {
			double res = 0;
		
			// x = 0, y = 0
			unsigned int x = 0;
			unsigned int y = 0;
			xMin yMin zMid
			STORE
					
			res = 0;
			// x = xsize - 1
			// y = zsize - 1
			x = ta->xsize - 1;
			y = ta->ysize - 1;
			xMax yMax zMid
			STORE
		}

		// Corner cases. Case: x' y' z'
		// x = y = z = 0
		unsigned int x = 0, y = 0, z = 0;
		double res = 0;
		xMin yMin zMin
		STORE
		
		res = 0;							
		// x = xsize-1, y = 0, z = 0
		x = ta->xsize - 1; y = 0; z = 0;
		xMax yMin zMin
		STORE
		
		res = 0;									
		// x = xsize-1, y = ysize-1, z = 0
		x = ta->xsize - 1; y = ta->ysize - 1; z = 0;
		xMax yMax zMin
		STORE
		
		res = 0;			
		// x = xsize-1, y = 0, z = zsize-1
		x = ta->xsize - 1; y = 0; z = ta->zsize - 1;
		xMax yMin zMax
		STORE
		
		res = 0;
		// x = xsize-1, y = ysize-1, z = zsize-1
		x = ta->xsize - 1; y = ta->ysize - 1; z = ta->zsize - 1;
		xMax yMax zMax
		STORE
		
		res = 0;
		// x = 0, y = ysize - 1, z = 0
		x = 0; y = ta->ysize - 1; z = 0;
		xMin yMax zMin
		STORE
		
		res = 0;			
		// x = 0, y = ysize - 1, z = zsize - 1
		x = 0; y = ta->ysize - 1; z = ta->zsize - 1;
		xMin yMax zMax
		STORE
		
		res = 0;							
		// x = 0, y = 0, z = zsize - 1
		x = 0; y = 0; z = ta->zsize - 1;
		xMin yMin zMax
		STORE
		
		// Normal cases (non-boundary). Case: x y z
		for (z = 1; z < ta->zsize - 1; z++) {
			for (y = 1; y < ta->ysize - 1; y++) {
				for (x = 1; x < ta->xsize - 1; x++) {
					res = 0;

					res += ta->input[((z * ta->ysize) + y) * ta->xsize + (x + 1)];
					res += ta->input[((z * ta->ysize) + y) * ta->xsize + (x - 1)]; 

					res += ta->input[((z * ta->ysize) + (y + 1)) * ta->xsize + x];
					res += ta->input[((z * ta->ysize) + (y - 1)) * ta->xsize + x];

					res += ta->input[(((z + 1) * ta->ysize) + y) * ta->xsize + x];
					res += ta->input[(((z - 1) * ta->ysize) + y) * ta->xsize + x];
					STORE
				}
			}
		}
		memcpy(ta->input, ta->potential, ta->size);
	}
	return 0;
}

