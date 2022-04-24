#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mtfuji.h"

int main(const int argc, const char **argv)
{
  FILE *fp;
  if ( (fp = fopen("data.csv","r")) == NULL ) {
    printf("cannot open file\n");
    return 0;
  }

  int l=0;
  char str[N];
  while(fgets(str, N, fp) != NULL) {
      l++;
  }
  fseek(fp,0L,SEEK_SET);
  Sample samp[l];
  //printf("%d\n",l);

  for(int i=0;i<l;i++){
      fscanf(fp,"%[^,],%lf,%lf\n",&samp[i].loc,&samp[i].alt,&samp[i].temp);
  }
  printf("load\n");
  fclose(fp);

  /*
  for(int i=0;i<l;i++){
    printf("%s %lf %lf\n",samp[i].loc,samp[i].alt,samp[i].temp);
  }
  printf("\n");
  */

  qsort(samp,l,sizeof(Sample),cmp);

  const double alpha = (argc == 2) ? atof(argv[1]) : 0.01;
  const int dim = f_dimension();
  double *x = malloc(dim * sizeof(double));
  for (int i = 0; i < dim; i++) {
    x[i] = 0;
  }

  printf("alpha = %f\n", alpha);

  const int iter = optimize(alpha,dim,x,l,samp,f_gradient);

  printf("number of iterations = %d\n", iter);

  printf("\n");
  printf("a=%lf b=%lf\n",x[0],x[1]);

  double temp_fuji=x[0]*3.77612+x[1];
  printf("Estimated temperature on Mt.Fuji=%lf(°C)\n",temp_fuji);
  printf("\n");
  printf("location altitude temperature\n");
  for(int i=0;i<l;i++){
    printf("%s %lf %lf\n",samp[i].loc,samp[i].alt,samp[i].temp);
  }
  free(x);

  return 0;
}