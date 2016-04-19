#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
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

double f(double x);
void *start_routine(void*);
long getpnum(long*);

int main(int argc, char *argv[])
{
    long num=atol(argv[1]);
    long N=atol(argv[2]);
    pthread_t *thread=calloc(num, sizeof(pthread_t));
    if (thread==NULL)
        handle_error("Cannot allocate memory");
    
    int i, en;
    long arg[3]={num, N};
    SUM=0;
    if (sem_init(&sem, 0, 1))
        handle_error("Cannot create semaphore");

    struct timespec beg, end;
    double t;
    clock_gettime(CLOCK_REALTIME, &beg);
    for (i=0; i<num; i++){
        arg[2]=i;
        en=pthread_create(&(thread[i]), NULL, start_routine, (void*)arg);
        if (en)
            handle_error_en(en, "Cannot create thread");
    }
    for (i=0; i<num; i++){
        en=pthread_join(thread[i], NULL);
        if (en)
            handle_error_en(en, "Cannot join thread");
    }
    clock_gettime(CLOCK_REALTIME, &end);
    t=end.tv_sec-beg.tv_sec;
    t+=(end.tv_nsec-beg.tv_nsec)*1E-9;

    sem_destroy(&sem);
    free(thread);
    
    #ifdef ACC
        printf("%ld, %lf\n", num, t);
    #else
        printf("%lf\n", (b-a)*SUM/N);
    #endif

    return 0;
}  

void *start_routine(void *arg)
{
    long rank=((long*)arg)[2];
    unsigned int seed=time(NULL)*rank; 
    long pnum=getpnum((long*)arg);
    long i;
    double S=0, val=0;
    for (i=1; i<=pnum; i++){
        val=a+(b-a)*((double)rand_r(&seed)/RAND_MAX);
        S+=f(val);
    }
    
    if (sem_wait(&sem))
        handle_error("Cannot lock semaphore");
    SUM+=S;
    if (sem_post(&sem))
        handle_error("Cannot unlock semaphore");

    pthread_exit(NULL);
}
    
double f(double x)
{
    return sin(x);
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
    
