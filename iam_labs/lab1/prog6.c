#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int k=strlen(argv[1]);
    int i;

	while (k>=1){
	    if ((strlen(argv[1])%k)==0){
	        static int l;
	        l=strlen(argv[1])/k;
	        for (i=1; i<k; i++){
	            if (strncmp(argv[1]+l*(i-1), argv[1]+l*i, l)!=0)
	                break;
            }   
	        if (i==k){
	            printf("%d\n", k);
	            return 0;
            }    
            
        }
        k--;
    }
        
    printf("1\n");
    return 0;
}










