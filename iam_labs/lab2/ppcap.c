#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

double calclat(int, int, void*, int);

int main(int argc, char *argv[])
{
    int i, k, np, mp, N, L;
    double s, t;
    FILE *out;
    void *a;
    MPI_Status status;

    N=atoi(argv[1]);
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &mp);
    

    out=fopen("./outppcap", "w");
    for (k=0; k<=22; k++){
        L=1<<k;
        a=malloc(L);
        s=calclat(mp, N, a, k);
        
        if (mp==0){
            t=MPI_Wtime();
            for (i=0; i<N; i++){
                MPI_Send(a, L, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(a, L, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &status);
            }
            t=MPI_Wtime()-t;
            fprintf(out, "%4d, %4.2lf\n", k, ((2*N*L)/(t-2*N*s))/(1<<20));
        }

        if (mp==1)
            for (i=0; i<N; i++){
                 MPI_Recv(a, L, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);  
                MPI_Send(a, L, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
            }

        free(a);
    }
    
    fclose(out);
    MPI_Finalize();
    return 0;
}

double calclat(int mp, int N, void *a, int k)
{
    int i;
    double t;
    MPI_Status status;

    if (mp==0){
        t=MPI_Wtime(); 
        for (i=0; i<N; i++){
            MPI_Send(a, 0, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(a, 0, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &status);
        }
        t=MPI_Wtime()-t;
        return t/(2*N);
    }

    if (mp==1){
        for (i=0; i<N; i++){
            MPI_Recv(a, 0, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(a, 0, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
        }
        return;
    }
}





