#include <stdio.h> 
#define NALPHA 26 
int main() 
{ 
  int count[NALPHA][NALPHA]; 
  int c, i, j; 
  int first, second; 
  
  for( i = 0 ; i < NALPHA ; i++ ) { 
    for( j = 0 ; j < NALPHA ; j++ ) { 
      count[i][j] = 0; 
    } 
  } 
  second = ' '; 
  while( ( c = getchar() ) != EOF ) { 
    first = second;
    second = c; 
    if(first-'a'>=0&&first-'a'<26&&second-'a'>=0&&second-'a'<26) { 
      count[first-'a'][second-'a']++; 
    } 
  }
  
  // 表示
  for( i = 0 ; i < NALPHA ; i++ ) { 
    for( j = 0 ; j < NALPHA ; j++ ) { 
      printf ("[%c][%c] = %d\n", 'a'+i, 'a'+j, count[i][j] ); 
    } 
  } 
  return 0;
}
