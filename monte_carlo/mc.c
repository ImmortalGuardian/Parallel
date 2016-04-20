#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>

#define handle_error_en(en, msg) \
    do {errno = en; perror(msg); exit(EXIT_FAILURE);} while (0)

#define handle_error(msg) \
    do {perror(msg); exit(EXIT_FAILURE);} while (0)

#define a 0
#define b M_PI

sem_t sem;
double SUM;

double F(double, double);
double f(double);
void *start_routine(void*);
long getpnum(long*);

int main(int argc, char *argv[])
{
    long num=atol(argv[1]);
    long N=atol(argv[2]);
    pthread_t *thread=calloc(num, sizeof(pthread_t));
    long **arg=calloc(num, sizeof(long*));
    if (thread==NULL || arg==NULL)
        handle_error("Cannot allocate memory");
    
    long i, en;
    for (i=0; i<num; i++){
        arg[i]=calloc(3, sizeof(long));
        if (arg[i]==NULL)
            handle_error("Cannot allocate memory");
        arg[i][0]=num; arg[i][1]=N;
    }
    SUM=0;
    if (sem_init(&sem, 0, 1))
        handle_error("Cannot create semaphore");

    struct timespec beg, end;
    double t;
    clock_gettime(CLOCK_REALTIME, &beg);
    for (i=0; i<num; i++){
        arg[i][2]=i;
        en=pthread_create(&(thread[i]), NULL, start_routine, (void*)arg[i]);
        if (en)
            handle_error_en(en, "Cannot create thread");
    }

    #ifdef RET
        double *retval; 
        double S=0;
        for (i=0; i<num; i++){
            en=pthread_join(thread[i], (void**)&retval);
            if (en)
                handle_error_en(en, "Cannot join thread");
            S+=*retval;
            free(retval);
        }
        SUM=S;
    #else 
    for (i=0; i<num; i++){
        en=pthread_join(thread[i], NULL);
        if (en)
            handle_error_en(en, "Cannot join thread");
    }
    #endif

    clock_gettime(CLOCK_REALTIME, &end);
    t=end.tv_sec-beg.tv_sec;
    t+=(end.tv_nsec-beg.tv_nsec)*1E-9;

    sem_destroy(&sem);
    free(thread);
    for (i=0; i<num; i++)
        free(arg[i]);
    free(arg);
    
    #ifdef ACC
        printf("%ld, %lf\n", num, t);
    #else
        printf("%lf\n", (M_PI*M_PI*SUM)/N);
    #endif

    return 0;
}  

void *start_routine(void *arg)
{
    long rank=((long*)arg)[2];
    static int r=0;
    r=rank;
    /*Just want to initialize seeds with such freaky values*/
    unsigned int seed_x=time(NULL)*rank, seed_y=(unsigned int)rand_r(&seed_x);
    long pnum=getpnum((long*)arg);
    long i;
    double x, y;
    double *S=calloc(1, sizeof(double));
    if (S==NULL)
        handle_error("Cannot allocate memory");
    *S=0;
    for (i=1; i<=pnum; i++){
        x=(double)rand_r(&seed_x)/RAND_MAX;
        y=(double)rand_r(&seed_y)/RAND_MAX;
        if ((y<f(x)) || (fabs(y-f(x))<DBL_EPSILON))
            *S+=F(x, y);
    }
    
    if (sem_wait(&sem))
        handle_error("Cannot lock semaphore");
    SUM+=*S;
    if (sem_post(&sem))
        handle_error("Cannot unlock semaphore");

    pthread_exit(S);
}

double f(double x)
{
    return sin((b-a)*x);
}
 
double F(double x, double y)
{
    return y*x;
}

long getpnum(long *arg)
{
    long num=((long*)arg)[0];
    long N=((long*)arg)[1];
    long rank=((long*)arg)[2];

    long s=N/num;
    if (rank<(N%num))
        s++;
    return s;
} 
    
