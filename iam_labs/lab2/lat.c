#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int N=atoi(argv[1]);
    int i, np, mp, k;
    double t;
    FILE *out;
    void *a;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &mp);

    out=fopen("./outlat", "w");
    for (k=0; k<=22; k++){
        a=malloc(1<<k);

        if (mp==0){
            t=MPI_Wtime();
            for (i=0; i<N; i++){
                MPI_Send(a, 0, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(a, 0, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &status);
            }
            t=MPI_Wtime()-t;
            fprintf(out, "%4d, %3.0lf\n", k, (t/(2*N))*1000000000);
        }

        if (mp==1)
             for (i=0; i<N; i++){
                MPI_Recv(a, 0, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Send(a, 0, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
            }
        
        free(a); 
    }
    
    fclose(out); 
    MPI_Finalize();
    return 0;
}
