#include <stdio.h>
#include <mpi.h>
#include "calc.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int rank, size;
    double T=atof(argv[1]);
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int N=(L/h)+1;
    int vseg=get_vseg(N, rank, size);
    double beg=get_beg(N, rank, size);

    double t=MPI_Wtime();
    double **layers=init_calc(rank, size, vseg, beg);
    int j=1;
    //int steps=atoi(argv[1]);
    for (j=1; j<=T/tau; j++)
        perf_calc(layers, rank, size, vseg, j);
    if (rank==0)
        fprintf(stderr, "%d, %lf\n", size, MPI_Wtime()-t);    

    //if (rank==0)
        //check_sol("./test"); 
    //give_res(argv[2], layers, vseg, rank, size,  beg, --j);
    MPI_Finalize();
    return 0;
}
