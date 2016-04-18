#include <math.h>
#include <stdio.h>
#include "const.h"
#include "bound.h"

double **init_calc(int, int, int, double);
void perf_calc(double**, int, int, int, int);
int get_vseg(int, int, int);
double get_beg(int, int, int);
void give_res(const char*, double**, int, int, int, double, int);
void check_sol(const char*);
