#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int i,j;
    int n=atoi(argv[1]);
    int *a=calloc(n, sizeof(int));
    
    for (i=0; i<n; i++)
        scanf("%d", &a[i]);
        
        
    for (i=n-1; i>=0; i--)
        printf("%d ", a[i]*a[i]);
        
    return 0;
} 
