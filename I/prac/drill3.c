#include <stdio.h>
#include <stdlib.h>
void order(int[]);
int main(){
    srand(10);
    int array[3];
    for(int i=0;i<10000;i++){
        array[0]=rand()%99+1;
        array[1]=rand()%99+1;
        array[2]=rand()%99+1;
        order(array);
        while(array[0]>=array[1]+array[2]){
            array[0]=rand()%99+1;
            array[1]=rand()%99+1;
            array[2]=rand()%99+1;
            order(array);
        }
        printf("%d,%d,%d\n",array[0],array[1],array[2]);  
    }
}
void order(int array[3]){
    int a=array[0];
    int b=array[1];
    int c=array[2];
    int max,min;
    if(a>=b){
        max=a;
        min=b;
    }else{
        max=b;
        min=a;
    }
    if(max>=c){
        array[0]=max;
        if(c>=min){
            array[1]=c;
            array[2]=min;
        }else{
            array[1]=min;
            array[2]=c;
        }
    }else{
        array[0]=c;
        array[1]=max;
        array[2]=min;
    }
} 