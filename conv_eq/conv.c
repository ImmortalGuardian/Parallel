#include "calc.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main(int argc, char *argv[])
{   
    int thrnum=atoi(argv[1]);
    pthread_t *threads=(pthread_t*)calloc(thrnum, sizeof(pthread_t)); 
    init(thrnum); 
    
    int i;
    double T=atof(argv[2]);
    double **args=prep_args(thrnum, T);
    for (i=0; i<thrnum; i++)
        pthread_create(&(threads[i]), NULL, start_routine, (void*)args[i]); 
    
    for (i=0; i<thrnum; i++)
        pthread_join(threads[i], NULL);
    copy_res(T/tau);
    get_res();
    
    end(thrnum, args);
    free(threads);

    return 0;
}
