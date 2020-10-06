#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Modified by Tim

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
	
	unsigned int x, y, z;
	double res = 0;
	
	memcpy(input, source, size);
	for (unsigned int iter = 0; iter < numiters; iter++) {
		// X on boundaries, Y and Z not on boundaries
		for (z = 1; z < ysize - 1; z++) {
			for (y = 1; y < ysize - 1; y++) {
				res = 0;
				
				// x = 0
				res = input[((z * ysize) + y) * xsize + 1] + 
						input[((z * ysize) + (y - 1)) * xsize] +
						input[((z * ysize) + (y + 1)) * xsize] + 
						input[(((z - 1) * ysize) + y) * xsize] +
						input[(((z + 1) * ysize) + y) * xsize] + 
						Vbound;
						
				res -= delta * delta * source[((z * ysize) + y) * xsize];
				res /= 6;
				potential[((z * ysize) + y) * xsize] = res;
						
				// x = xsize - 1
				x = xsize - 1;
				res = input[((z * ysize) + y) * xsize + (x - 1)] + 
						input[((z * ysize) + (y - 1)) * xsize + x] +
						input[((z * ysize) + (y + 1)) * xsize + x] + 
						input[(((z - 1) * ysize) + y) * xsize + x] +
						input[(((z + 1) * ysize) + y) * xsize + x] + 
						Vbound;
				
				res -= delta * delta * source[((z * ysize) + y) * xsize + x];
				res /= 6;
				potential[((z * ysize) + y) * xsize + x] = res;
			}
		}
		// Y on boundaries, X and Z not on boundaries
		for (z = 1; z < zsize - 1; z++) {
			for (x = 1; x < xsize - 1; x++) {
				res = 0;
				
				// y = 0
				res = input[((z * ysize) + 1) * xsize + x] + 
						input[(z * ysize) * xsize + (x - 1)] + 
						input[(z * ysize) * xsize + (x - 1)] + 
						input[((z - 1) * ysize) * xsize + x] + 
						input[((z + 1) * ysize) * xsize + x] + 
						Vbound;

				res -= delta * delta * source[z * ysize * xsize + x];
				res /= 6;
				potential[z * ysize * xsize + x] = res;
				
				// y = ysize - 1
				y = ysize - 1;
				res = input[((z * ysize) + (y - 1)) * xsize + x] + 
						input[((z * ysize) + y) * xsize + (x - 1)] +
						input[((z * ysize) + y) * xsize + (x - 1)] + 
						input[(((z - 1) * ysize) + y) * xsize + x] +
						input[(((z + 1) * ysize) + y) * xsize + x] + 
						Vbound;
						
				res -= delta * delta * source[((z * ysize) + y) * xsize + x];
				res /= 6;
				potential[((z * ysize) + y) * xsize + x] = res;
			}
		}
		// Z on boundaries, X and Y not on boundaries
		for (y = 1; y < ysize - 1; y++) {
			for (x = 1; x < xsize - 1; x++) {
				res = 0;
				
				// z = 0
				res = input[(ysize + y) * xsize + x] +
						input[y * xsize + (x - 1)] + 
						input[y * xsize + (x + 1)] + 
						input[(y - 1) * xsize + x] + 
						input[(y + 1) * xsize + x] + 
						Vbound;
				
				res -= delta * delta * source[y * xsize + x];
				res /= 6;
				potential[y * xsize + x] = res;
				
				// z = zsize - 1
				z = zsize - 1;
				res = input[(((z - 1) * ysize) + y) * xsize + x] + 
						input[((z * ysize) + (y - 1)) * xsize + x] +
						input[((z * ysize) + (y + 1)) * xsize + x] + 
						input[((z * ysize) + y) * xsize + (x - 1)] +
						input[((z * ysize) + y) * xsize + (x + 1)] + 
						Vbound;
				
				res -= delta * delta * source[((z * ysize) + y) * xsize + x];
				res /= 6;
				potential[((z * ysize) + y) * xsize + x] = res;
			}
		}
		
		// x = y = z = 0
		x = 0; y = 0; z = 0;
		res = 
				3*Vbound +
				input[((z * ysize) + y) * xsize + (x + 1)] +
				input[((z * ysize) + (y + 1)) * xsize + x] +
				input[(((z + 1) * ysize) + y) * xsize + x];
				
		res -= delta * delta * source[((z * ysize) + y) * xsize + x];
		potential[((z * ysize) + y) * xsize + x] = res/6;
									
		// x = xsize-1, y = 0, z = 0
		x = xsize - 1; y = 0; z = 0;
		res = 
				3*Vbound +
				input[((z * ysize) + y) * xsize + (x - 1)] +
				input[((z * ysize) + (y + 1)) * xsize + x] +
				input[(((z + 1) * ysize) + y) * xsize + x];
		
		res -= delta * delta * source[((z * ysize) + y) * xsize + x];
		potential[((z * ysize) + y) * xsize + x] = res/6;
											
		// x = xsize-1, y = ysize-1, z = 0
		x = xsize - 1; y = ysize - 1; z = 0;
		res = 
				3*Vbound +
				input[((z * ysize) + y) * xsize + (x - 1)] +
				input[((z * ysize) + (y - 1)) * xsize + x] +
				input[(((z + 1) * ysize) + y) * xsize + x];
				
		res -= delta * delta * source[((z * ysize) + y) * xsize + x];
		potential[((z * ysize) + y) * xsize + x] = res/6;
					
		// x = xsize-1, y = 0, z = zsize-1
		x = xsize - 1; y = 0; z = zsize - 1;		
		res = 
				3*Vbound + 
				input[((z * ysize) + y) * xsize + (x - 1)] +
				input[((z * ysize) + (y + 1)) * xsize + x] +
				input[(((z - 1) * ysize) + y) * xsize + x];

		res -= delta * delta * source[((z * ysize) + y) * xsize + x];
		potential[((z * ysize) + y) * xsize + x] = res/6;
		
		// x = xsize-1, y = ysize-1, z = zsize-1
		x = xsize - 1; y = ysize - 1; z = zsize - 1;
		res = 
				3*Vbound +
				input[((z * ysize) + y) * xsize + (x - 1)] +
				input[((z * ysize) + (y - 1)) * xsize + x] +
				input[(((z - 1) * ysize) + y) * xsize + x];

		res -= delta * delta * source[((z * ysize) + y) * xsize + x];
		potential[((z * ysize) + y) * xsize + x] = res/6;
		// x = 0, y = ysize - 1, z = 0
		x = 0; y = ysize - 1; z = 0;
		res = 
				3*Vbound + 		
				input[((z * ysize) + y) * xsize + (x + 1)] +
				input[((z * ysize) + (y  - 1)) * xsize + x] +
				input[(((z + 1) * ysize) + y) * xsize + x];

		res -= delta * delta * source[((z * ysize) + y) * xsize + x];
		potential[((z * ysize) + y) * xsize + x] = res/6;
					
		// x = 0, y = ysize - 1, z = zsize - 1
		x = 0; y = ysize - 1; z = zsize - 1;
		res = 
				3*Vbound +
				input[((z * ysize) + y) * xsize + (x + 1)] +
				input[((z * ysize) + (y - 1)) * xsize + x] +
				input[(((z - 1) * ysize) + y) * xsize + x];	
				
		res -= delta * delta * source[((z * ysize) + y) * xsize + x];
		potential[((z * ysize) + y) * xsize + x] = res/6;
									
		// x = 0, y = 0, z = zsize - 1
		x = 0; y = 0; z = zsize - 1;
		res = 
				3*Vbound +
				input[((z * ysize) + y) * xsize + (x + 1)] +
				input[((z * ysize) + (y + 1)) * xsize + x] +
				input[(((z - 1) * ysize) + y) * xsize + x];		
		
		res -= delta * delta * source[((z * ysize) + y) * xsize + x];
		potential[((z * ysize) + y) * xsize + x] = res/6;
		
		// Normal cases (non-boundary)
		for (z = 1; z < zsize - 1; z++) {
			for (y = 1; y < ysize - 1; y++) {
				for (x = 1; x < zsize - 1; x++) {
					res = 0;

					res += input[((z * ysize) + y) * xsize + (x + 1)];
					res += input[((z * ysize) + y) * xsize + (x - 1)];

					res += input[((z * ysize) + (y + 1)) * xsize + x];
					res += input[((z * ysize) + (y - 1)) * xsize + x];

					res += input[(((z + 1) * ysize) + y) * xsize + x];
					res += input[(((z - 1) * ysize) + y) * xsize + x];

					res -= delta * delta * source[((z * ysize) + y) * xsize + x];
					res /= 6;

					potential[((z * ysize) + y) * xsize + x] = res;
				}
			}
		}
		memcpy(input, potential, size);
	}
	free(input);
}
