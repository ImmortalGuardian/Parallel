#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int i;
    int n=strlen(argv[1]);
    printf("%s\n", argv[1]); 
    n--;
    for (i==0; i<n/2; i++)
        if (argv[1][i]!=argv[1][n-i]){
            printf("NO\n");
            return 0;
        }
    
    printf("YES\n");

    return 0;
}
