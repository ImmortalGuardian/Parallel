#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define M 10

int min(int, int);
short *gen(int);

int main(int argc, char *argv[])
{
    long long N=atoll(argv[1]);
    int size, myrank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    if (myrank==0){
        int i, j;
        int buf;
        long long res=0;
        double t, T;
       
        for (j=1; j<=M; j++){ 
            t=MPI_Wtime();
            res=0;
            for (i=1; i<=size-1; i++){
                buf=0;
                MPI_Recv((void*)&buf, 1, MPI_LONG_LONG, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                res+=buf;
            }
            T+=MPI_Wtime()-t;
        }
        printf("%d, %lf\n", size, T/M);
        
    }

    else {
        int num=N/(size-1);
        long long res;
        //long long skip=num*(myrank-1);
        if ((myrank-1)<N%(size-1))
            num++;
        //skip+=min(myrank-1, N%(size-1)); 

        //FILE *inp=fopen(argv[2], "r");
        short *a=gen(num);
        long long i;

        /*for (i=1; i<=skip; i++)
            fscanf(inp, "%d ", &(a[0]));
        for (i=0; i<num; i++)
            fscanf(inp, "%hu ", &(a[i]));
        printf("%d: %llu %d\n", myrank, skip, num);
        fflush(stdout);*/
        
        int j;
        for (j=0; j<=M; j++){
            res=0;
            for (i=0; i<num; i++)
                res+=a[i];  
            MPI_Send((void*)&res, 1, MPI_LONG_LONG, 0, myrank, MPI_COMM_WORLD);
        }
    }
    
    MPI_Finalize();
    return 0;
}

int min(int a, int b)
{
    return (a<=b)? a:b;
}

short *gen(int num){
    int i;
    short *a=calloc(num, sizeof(short));
    
    for (i=0; i<num; i++)
        a[i]=1;

    return a;
}
        
