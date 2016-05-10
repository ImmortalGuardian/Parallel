#include <semaphore.h>
#include "const.h"

sem_t *SEMS;
double **VALS;

double g(double);
void init(int);
void *start_routine(void*);
int get_pnum(int, int);
int get_pbeg(int, int);
void set_init_cond(int, int, int, int);
void perf_calc(int, int, int, int, int);
void copy_res(int);
void get_res();
void end(int, double**);
double **prep_args(int, double);
