#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int compar(const void *arg1, const void *arg2)
{
    int a=(int)*((int*)arg1);
    int b=(int)*((int*)arg2);

    if (a<b)
        return 1;
    if (b<a)
        return -1;
    else
        return 0;
}

int main(int argc, char *argv[])
{
    int i;
    int n=atoi(argv[1]);
    int *a=calloc(n, sizeof(int));

    srand(time(NULL)); 
    for (i=0; i<n; i++)
        a[i]=(short)rand();

    qsort(a, n, sizeof(int), compar);

    for (i=0; i<n; i++)
        printf("%d ", a[i]);
    printf("\n");

    return 0;
}
