#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "calc.h"

double **init_calc(int rank, int size, int vseg, double beg)
{
    double **a=(double**)calloc(2, sizeof(double*));
    a[0]=(double*)calloc(vseg, sizeof(double));
    a[1]=(double*)calloc(vseg, sizeof(double));
   
    int m=!(rank==0);
 
    if (rank>0)
        a[0][0]=U0(beg-h);
    else
        a[0][0]=U1(0);
    if (rank<size-1)
        a[0][vseg-1]=U0(beg+(vseg-1-m)*h);
    else
        a[0][vseg-1]=U2(0);

    int i;  
    for (i=1; i<vseg-1; i++)
        a[0][i]=U0(beg+(i-1+!m)*h);
    
    return a;
}

void perf_calc(double *a[], int rank, int size, int vseg, int j)
{
    int lay=(j%2==1);
    
    /*if (rank==0 || rank==size-1){
        printf("%d %d\n", rank, j);
        fflush(stdout);
    }*/
    
    MPI_Request req[4];
 
    a[lay][1]=(1-2*tau/(h*h))*a[!lay][1] + (tau/(h*h))*(a[!lay][2]+a[!lay][0]);
    if (rank>0){
        MPI_Isend((void*)&a[lay][1], 1, MPI_DOUBLE, rank-1, rank, MPI_COMM_WORLD, &req[0]);
        MPI_Irecv((void*)&a[lay][0], 1, MPI_DOUBLE, rank-1, rank-1, MPI_COMM_WORLD, &req[1]);
    }
    else
        a[lay][0]=U1(j*tau);

    a[lay][vseg-2]=(1-2*tau/(h*h))*a[!lay][vseg-2] + (tau/(h*h))*(a[!lay][vseg-1]+a[!lay][vseg-3]);
    if (rank<size-1){
        MPI_Isend((void*)&a[lay][vseg-2], 1, MPI_DOUBLE, rank+1, rank, MPI_COMM_WORLD, &req[2]);
        MPI_Irecv((void*)&a[lay][vseg-1], 1, MPI_DOUBLE, rank+1, rank+1, MPI_COMM_WORLD, &req[3]);
    } 
    else
        a[lay][vseg-1]=U2(j*tau);    

    int i;
    for (i=2; i<=vseg-3; i++)
        a[lay][i]=(1-2*tau/(h*h))*a[!lay][i] + (tau/(h*h))*(a[!lay][i+1]+a[!lay][i-1]);
    
    if (rank>0 && rank<size-1) 
        MPI_Waitall(4, req, MPI_STATUSES_IGNORE);
    else if (rank==size-1 && size>1)
        MPI_Waitall(2, req, MPI_STATUSES_IGNORE);
    else if (size>1)
        MPI_Waitall(2, &(req[2]), MPI_STATUSES_IGNORE);

    /*if (rank==0 || rank==size-1){
        if (rank==0) 
            printf("Proc %d, step %d:, %lf %lf %lf\n", rank, j, a[lay][0], a[lay][1], a[lay][2]);
        else
            printf("Proc %d, step %d:, %lf %lf %lf\n", rank, j, a[lay][vseg-3], a[lay][vseg-2], a[lay][vseg-1]);
        fflush(stdout);
    }*/
    return;
}

int get_vseg(int N, int rank, int size)
{
    int s=N/size;
    if (rank<N%size)
        s++;
    if (rank>0 && rank<size-1)
        return s+2;
    else
        return s+1;
} 

double get_beg(int N, int rank, int size)
{
    int m=(N/size)*rank;
    if (rank>N%size)
        m+=N%size;
    else
        m+=rank;
        
    return m*h;
}

void give_res(const char *fname, double *a[], int vseg, int rank, int size, double beg, int j)
{
    int lay=(j%2==1);
    int i;
    int buf;

    int m=(rank==0 || rank==size-1);
    int f=(rank==0);
    FILE *fout=fopen(fname, "a");
    if (rank>0)
        MPI_Recv((void*)&buf, 1, MPI_INT, rank-1, rank-1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (i=1; i<=vseg-2+m; i++){
        fprintf(fout, "%lf, %lf\n", beg+(i-1)*h, a[lay][i-f]);
        fflush(fout);
    }
    fflush(fout);
    fclose(fout);
    //printf("%d\n",rank);
    if (rank<size-1)
        MPI_Send((void*)&buf, 1, MPI_INT, rank+1, rank, MPI_COMM_WORLD);
    
    return;
}

void check_sol(const char *fname)
{
    int n, i, m;
    double S_m, U_0, t;
    FILE *out=fopen(fname, "w");

    U_0=U0(0);
    m=10;
    for (n=0; n<=L/h; n++){
        S_m=0;
        for (i=0; i<=m; i++){
            //t=-(2*i+1)*(2*i+1)*M_PI*M_PI*T/(L*L);
            S_m+=(4*U_0/M_PI)*((exp(t)*sin(M_PI*(2*i+1)*n*h))/(2*i+1));
        }
        fprintf(out, "%lf, %lf\n", n*h, S_m);
    }

    fclose(out); 
    return;
}
