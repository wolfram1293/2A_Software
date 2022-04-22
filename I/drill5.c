#include <stdio.h>
int main (){
	int prime[1000];
	for (int i = 0; i < 1000; i++)
		prime[i] = 0;
		prime[0] = 2;
		for (int j = 3; j; j++){
			for (i = 0; i < 100; i++){
				if (prime[i] == 0){
					prime[i] = j;
					break;
				}
				if (j%prime[i] == 0){
                    break;
                }
			}
			if (prime[99]){
                break;
            }
		}
		
		printf ("  1st prime : %3d\n", prime[0]);
		printf ("  2nd prime : %3d\n", prime[1]);
		printf ("  3rd prime : %3d\n", prime[2]);
		for (i = 3; i < 100; i++)
			printf ("%3dth prime : %3d\n", i+1, prime[i]);
		
	return 0;
	}