#include "calc.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void init(int thrnum)
{
    int totalpts=(b-a)/h + 1;
    int i;
    
    VALS=(double**)calloc(2, sizeof(double*)); 
    VALS[0]=(double*)calloc(totalpts, sizeof(double));
    VALS[1]=(double*)calloc(totalpts, sizeof(double));
    SEMS=(sem_t*)calloc(thrnum-1, sizeof(sem_t));
    
    for (i=0; i<thrnum-1; i++)
        sem_init(&(SEMS[i]), 0, 0);
         
    return;
}

double g(double x)
{
    if (x<0 || x>2)
        return 0;
    else
        return x*(2-x);
}

double **prep_args(int thrnum, double T)
{
    double **args=(double**)calloc(thrnum, sizeof(double*));
    
    int i;
    for (i=0; i<thrnum; i++){
        args[i]=(double*)calloc(3, sizeof(double));
        args[i][0]=(double)i;
        args[i][1]=(double)thrnum;
        args[i][2]=T;
    }

    return args;
}

void *start_routine(void *args)
{ 
    int rank=(int)(((double*)args)[0]);
    int thrnum=(int)(((double*)args)[1]);
    double T=((double*)args)[2];
    int pnum=get_pnum(rank, thrnum);
    int pbeg=get_pbeg(rank, thrnum);

    int j; 
    set_init_cond(rank, thrnum, pbeg, pnum);  
    for (j=1; j<=T/tau; j++)
        perf_calc(rank, thrnum, pbeg, pnum, j); 

    pthread_exit(NULL);
}

int get_pnum(int rank, int thrnum)
{
    int totalpts=(b-a)/h + 1;
    int res=totalpts/thrnum;
    if (rank < totalpts%thrnum)
        res++;

    return res;
}

int get_pbeg(int rank, int thrnum)
{
    int totalpts=(b-a)/h + 1;
    int res=(totalpts/thrnum)*rank;
    if (rank < totalpts%thrnum)
        res+=rank;
    else
        res+=totalpts%thrnum;

    return res;
}

void set_init_cond(int rank, int thrnum, int pbeg, int pnum)
{
    int i, total;
    int pend=pbeg+pnum;
    for (i=pbeg; i<pend; i++)
        VALS[0][i]=g(i*h);
    if (rank!=thrnum-1)
        sem_post(&(SEMS[rank]));

    return;
}

void perf_calc(int rank, int thrnum, int pbeg, int pnum, int j)
{
    int i, pend, lay;
    pend=pbeg+pnum; 
    lay=(j%2)==1;
    
    if (rank>0)
        sem_wait(&(SEMS[rank-1]));

    if (rank>0)
        VALS[lay][pbeg]=(1-(c*tau)/h)*VALS[!lay][pbeg] + ((c*tau)/h)*VALS[!lay][pbeg-1];
    else
        VALS[lay][pbeg]=0;
    
    for (i=pbeg+1; i<pend; i++)
        VALS[lay][i]=(1-(c*tau)/h)*VALS[!lay][i] + ((c*tau)/h)*VALS[!lay][i-1];

    if (rank!=thrnum-1)
        sem_post(&(SEMS[rank]));

    return;
}

void copy_res(int j)
{
    if (j%2==1){
        int i;
        int total=(b-a)/h + 1;
        for (i=0; i<total; i++)
            VALS[0][i]=VALS[1][i];
    }
    
    return;
}

void get_res()
{
    int i;
    int total=(b-a)/h + 1;
    
    for (i=0; i<total; i++)
        printf("%lf, %lf\n", i*h, VALS[0][i]);

    return;
}

void end(int thrnum, double **args)
{
    int i;
    for (i=0; i<thrnum-1; i++)
        sem_destroy(&(SEMS[i]));

    for (i=0; i<thrnum; i++)
        free(args[i]);

    free(args);
    free(SEMS);
    free(VALS[0]);
    free(VALS[1]);
    free(VALS);

    return;
}

