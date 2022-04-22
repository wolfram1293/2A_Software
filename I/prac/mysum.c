#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]) {
    double v1 = atof(argv[1]);
    double v2 = atof(argv[2]);
    printf("%f", v1 + v2);
    return 0;
}