/// \brief Test program for solving Poisson's equation using Jacobi relaxation
/// \author M. P. Hayes UCECE

#include <stdlib.h>
#include <stdio.h>
#include "poisson.hpp"

int main (int argc, char *argv[])
{
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
    
    poisson_dirichlet(source, potential, 1, xsize, ysize, zsize, delta,
                      numiters, numcores);

    return 0;
}