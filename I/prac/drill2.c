#include <stdio.h>
int gcd(int,int);
int array[3] = {34,1496,374};
int main() {
    int temp=gcd(array[0],array[1]);
    int ans=gcd(temp,array[2]);
    printf("gcd = %d\n",ans);
}

int gcd(int a,int b){
    int r = a % b;
    while(r!=0){
        a = b;
        b = r;
        r = a % b;
    }
    return b;
}