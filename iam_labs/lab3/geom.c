#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define M 10

int min(int, int);
long long *gen(int, int, int, long long);
int get_pow(int);

int main(int argc, char *argv[])
{
    long long N=atoll(argv[1]);
    int size, myrank;
    MPI_Status status;
     
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    int num=N/size;
    if (myrank<N%size)
        num++;
    long long *a=gen(num, myrank, size,  N);
    long long i;

    double t, T=0;
    long long res=0;
    int st;
    long long buf;
    long long d;
    int j;
    for (j=1; j<=M; j++){
        if (myrank==0)
            t=MPI_Wtime();

        res=0;
        for (i=0; i<num; i++)
            res+=a[i];
    
        st=2;
        d=get_pow(size);
        for (i=1; i<=d; i++){
            if (myrank%st==st/2){
                MPI_Send((void*)&res, 1, MPI_UNSIGNED_LONG_LONG, myrank-st/2, myrank-st/2, MPI_COMM_WORLD);
                st*=2;
                continue;
            }    
            else if (myrank%st==0){
                if ((myrank+st/2)<size)
                MPI_Recv((void*)&buf, 1, MPI_UNSIGNED_LONG_LONG, myrank+st/2, myrank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                res+=buf;
                st*=2;
                continue;
            }
        }
        T+=MPI_Wtime()-t;
    }
    
    free(a);
    if (myrank==0)
        printf("%d, %lf\n", size, T/M);

    MPI_Finalize();
    return 0;
}

int min(int a, int b)
{
    return (a<=b)? a:b;
}

long long *gen(int num, int myrank, int size, long long N)
{
    long long skip=(N/size)*myrank;
    skip+=min(myrank, N%size);
    
    long long *a=(long long*)calloc(num, sizeof(long long));
    long int i;
    for (i=0; i<num; i++)
        a[i]=i+skip+1;

    return a;
}

int get_pow(int size)
{
    int pow=0;
    int cmp=1;
    while (size>cmp){
        cmp<<=1;
        pow++;
    }

    return pow;
}

