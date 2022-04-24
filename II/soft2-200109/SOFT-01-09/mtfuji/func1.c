#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mtfuji.h"

int f_dimension()
{
  return 2;
}

void f_gradient(const double x[],double g[],int n,Sample samp[])
{
  g[0] = 0;
  g[1] = 0;
  for(int i=0;i<n;i++){
    g[0] += -2*(samp[i].temp-x[0]*samp[i].alt-x[1])*samp[i].alt;
    g[1] += -2*(samp[i].temp-x[0]*samp[i].alt-x[1]);
  }
}

double calc_norm(const int dim, double v[])
{
  double tmp = 0;
  for (int i = 0; i < dim; i++) {
    tmp += v[i] * v[i];
  }
  tmp = sqrt(tmp);
  return tmp;
}

int optimize(const double alpha, const int dim, double x[],int n,Sample samp[],void (*calc_grad)(const double [], double [],int ,Sample []))
{
  // 勾配ベクトルを記録する領域を確保
  double *g = malloc(dim * sizeof(double));

  int iter = 0;
  while (++iter < 10000) {

    // 引数で渡された関数を使って勾配ベクトルを計算
    (*calc_grad)(x,g,n,samp);

    // 勾配ベクトルのノルムを評価
    const double norm = calc_norm(dim, g);
    printf("%3d norm = %7.4f", iter, norm);
    for (int i = 0; i < dim; i++) {
      printf(", x[%d] = %7.4f", i, x[i]);
    }
    printf("\n");

    if (norm < 0.001) break;

    // 最急降下法による更新
    for (int i = 0; i < dim; i++) {
      x[i] -= alpha * g[i];
    }
  }

  free(g);

  return iter;
}

int cmp( const void *p, const void *q ) {
  if (((Sample*)p)->alt>((Sample*)q)->alt) return 1;
  else return -1;
}