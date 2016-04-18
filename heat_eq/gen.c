#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *in=fopen("acc.out", "r");
    FILE *out=fopen("acc", "w");
    int n=atoi(argv[1]);

    int i, p;
    double t, t1;
    fscanf(in, "%d, %lf\n", &p, &t1);
    fprintf(out, "%d, 1.0\n", p);
    for (i=2; i<=n; i++){
        fscanf(in, "%d, %lf\n", &p, &t);
        fprintf(out, "%d, %lf\n", p, t1/t);
    }

    return 0;
}

