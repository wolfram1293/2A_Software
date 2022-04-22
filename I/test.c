#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    for(int i=1;i<10;i++){
        printf("%d\n",i);
        if(i==5){
            i=i+1;
        }
    }
return 0;
}