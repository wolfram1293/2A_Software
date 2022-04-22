#include <stdio.h>
#include <stdio.h>
int getmax(int[],int);
int numbers[] = {12, 34, 56, 78, 91};

int main()
{
    int N=sizeof numbers/sizeof numbers[0];
    printf("max=%d\n", getmax(numbers,N));
    return 0;
}

int getmax(int a[],int N){
    int max=0;
    for(int i=0;i<N;i++){
        if (a[i]>N){
            max=a[i];
        }
    }
    return max;
}
