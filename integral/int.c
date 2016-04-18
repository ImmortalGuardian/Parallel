#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double f(double x);
double calc(double beg, int nseg, double dx);

int main(int argc, char *argv[])
{
    int N=atoi(argv[1]);
    int myrank, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
        
    if (myrank==0){
        double dx=1.0/N;          
        int nseg=N/size;
        double wnd=nseg*dx;
        double buf[3]={-wnd, (double)nseg, dx};
        
        int i;
        for (i=1; i<=size-1; i++){
            buf[0]+=wnd;
            MPI_Send((void*)buf, 3, MPI_DOUBLE, i, i, MPI_COMM_WORLD);
        }

        double sum=calc(wnd*(size-1), N-nseg*(size-1), dx);
        double prt; 
        for (i=1; i<=size-1; i++){
            MPI_Recv((void*)&prt, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD, &status);
            sum+=prt;
        }

        printf("Parallel computation led to result I=%10.8lf\n", sum);
        
        sum=calc(0, N, dx);
        printf("Successive computation led to result I=%10.8lf\n", sum);
    }
    else{
        double buf[3];
        MPI_Recv((void*)buf, 3, MPI_DOUBLE, 0, myrank, MPI_COMM_WORLD, &status);
        double prt=calc(buf[0], (int)buf[1], buf[2]);
        MPI_Send((void*)&prt, 1, MPI_DOUBLE, 0, myrank, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
} 


double f(double x)
{
    return 4/(1+x*x);
} 

double calc(double beg, int nseg, double dx)
{
    int i;
    double a=beg-dx, b=beg;
    double prt=0;
    for (i=1; i<=nseg; i++){
        a+=dx;
        b+=dx;
        prt+=(dx/2)*(f(a)+f(b));
    }
    
    return prt;
}
