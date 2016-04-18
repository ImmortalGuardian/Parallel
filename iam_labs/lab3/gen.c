#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *inp=fopen(argv[1], "r");
    int i, u;
    double a[20];
    for (i=0; i<=19; i++)
        fscanf(inp, "%d, %lf", &u, &a[i]);

    for (i=0; i<=19; i++)
        printf("%d, %lf\n", i+1, (a[0]/a[i]));

    return 0;
}
