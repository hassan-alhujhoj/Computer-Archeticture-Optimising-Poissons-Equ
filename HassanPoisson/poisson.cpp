#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "poisson.hpp"
#include <thread>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>

// Modified by Tim & Hassan
using namespace std;

// storing res
#define STORE 	res -= ta->delta * ta->delta * ta->source[((z * ta->ysize) + y) * ta->xsize];\
				res /= 6;\
				ta->potential[((z * ta->ysize) + y) * ta->xsize + x] = res;
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
	unsigned int zStart;
	unsigned int zEnd;
	unsigned int blockSize;
	unsigned int remainder;
}arg;

// declares poissonThreads() function
void poissonThreads(arg* ta); 

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
    unsigned int zStart = 0;
    unsigned int zEnd = 0;
    unsigned int blockSize = 0;
    unsigned int remainder = 0;

	double *input = (double *)malloc(size);
	if (!input) {
		fprintf(stderr, "malloc failure\n");
		return;
	}
	memcpy(input, source, size);
	arg ta = {
		.source = source,
		.potential = potential,
		.Vbound = Vbound,
		.xsize = xsize,
		.ysize = ysize,
		.zsize = zsize,
		.delta = delta,
		.numiters = numiters,
		.numcores = numcores,
		.input = input,
		.size = size,
	};
	
	vector<thread> thread_vector(numcores);
	for (unsigned int i = 0; i < numcores; i++) {
		blockSize = floor(zsize / numcores); cout << "\nblocksize:" << blockSize << endl;
		remainder = zsize % numcores; cout << "\nremainder:" <<  remainder << endl;
		zStart = i * blockSize; cout << "\nzStart:" << zStart << endl;
		zEnd += blockSize; cout << "\nzEnd:" << zEnd << endl;
		thread_vector[i] = thread(poissonThreads, &ta);
		printf("\nThread %d has been created!\n", i);
	}

	for(unsigned int i = 0; i < numcores; i++){
		if(thread_vector[i].joinable()){
			thread_vector[i].join();
		} 
		printf("\nThread %d has been joined!\n", i);
	}
}

void poissonThreads(arg* ta){
	cout << "\nYou're in poissonThreads()\n";
	
	// This is a flag to make sure that the cases don't run a boundery condition when it doesn't contain a boundery z
	unsigned int fl_zmin;
	//unsigned int fl_zmax;
	if(ta->zStart == 0){
		fl_zmin = 1;
		ta->zStart++;
	} else if (ta->zStart == ta->zEnd - 1){
		//fl_zmax = 1;
		ta->zEnd--;
	}

	for (unsigned int iter = 0; iter < ta->numiters; iter++) {
		
		// Normal cases (non-boundary). Case: x y z
		for (unsigned int z = ta->zStart; z < ta->zEnd + 1; z++) {
			for (unsigned int y = 1; y < ta->ysize - 1; y++) {
				for (unsigned int x = 1; x < ta->xsize - 1; x++) {
					double res = 0;

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
		
		// Z on boundaries, X and Y not on boundaries. Case: x y z'
		if(fl_zmin){
			for (unsigned int y = 1; y < ta->ysize - 1; y++) {
				for (unsigned int x = 1; x < ta->xsize - 1; x++) {
					double res = 0;
					
					// z = 0
					unsigned int z = 0;
					xMid yMid zMin
					STORE
					
					// z = ysize - 1
					res = 0;
					z = ta->zsize - 1;
					xMid yMid zMax
					STORE
				}
			}
		}

		// Y on boundaries, X and Z not on boundaries. Case: x y' z
		for (unsigned int z = ta->zStart; z < ta->zEnd; z+=ta->blockSize) {
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
		
		// Y and Z on boundaries, X not on boundaries. Case: x y' z'
		if(fl_zmin){
			for (unsigned int x = 1; x < ta->xsize - 1; x++) {
				double res = 0;
		
				// y = 0, z = 0
				unsigned int y = 0;
				unsigned int z = 0;
				xMid yMin zMin
				STORE
				
				// y = 0, z = zsize - 1
				res = 0;
				y = 0;
				z = ta->zsize - 1;
				xMid yMin zMax
				STORE
				
				// y = xsize - 1, z = 0
				res = 0;
				y = ta->ysize - 1;
				z = 0;
				xMid yMax zMin
				STORE
				
				// y = xsize - 1
				// z = zsize - 1	
				res = 0;
				y = ta->ysize - 1;
				z = ta->zsize - 1;
				xMid yMax zMax
				STORE
			}
		}
		// X on boundaries, Y and Z not on boundaries. Case: x' y z
		for (unsigned int z = ta->zStart; z < ta->zEnd; z+=ta->blockSize) {
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
		
		// X and Z on boundaries, Y not on boundaries. Case: x' y z'
		if(fl_zmin){
			for (unsigned int y = 1; y < ta->ysize - 1; y++) {
				double res = 0;
			
				// x = 0, z = 0
				unsigned int x = 0;
				unsigned int z = 0;
				xMin yMid zMin
				STORE
				
				// x = 0, z = zsize - 1
				res = 0;
				x = 0;
				z = ta->zsize - 1;
				xMin yMid zMax
				STORE
				
				// x = xsize - 1, z = 0
				res = 0;
				x = ta->xsize - 1;
				z = 0;
				xMax yMid zMin
				STORE
				
				// x = xsize - 1
				// z = zsize - 1	
				res = 0;
				x = ta->xsize - 1;
				z = ta->zsize - 1;
				xMax yMid zMax
				STORE
			}
		}

		// X and Y on boundaries, Z not on boundaries. Case: x' y' z
		for (unsigned int z = ta->zStart; z < ta->zEnd; z+=ta->blockSize) {
			double res = 0;
		
			// x = 0, y = 0
			unsigned int x = 0;
			unsigned int y = 0;
			xMin yMin zMid
			STORE
			
			// x = 0, y = zsize - 1
			res = 0;
			x = 0;
			y = ta->ysize - 1;
			xMin yMax zMid
			STORE
			
			// x = xsize - 1, y = 0
			res = 0;
			x = ta->xsize - 1;
			y = 0;
			xMax yMin zMid
			STORE
			
			// x = xsize - 1
			// y = zsize - 1	
			res = 0;
			x = ta->xsize - 1;
			y = ta->ysize - 1;
			xMax yMax zMid
			STORE
		}

		// Corner cases. Case: x' y' z'
		if(fl_zmin){
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
		}
		memcpy(ta->input, ta->potential, ta->size);
	}
}

