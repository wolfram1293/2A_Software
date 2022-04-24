#ifndef MTFUJI_MTFUJI_H
#define MTFUJI_MTFUJI_H

#define N 64
typedef struct
{
  char loc[N];   // location name
  double alt;  // altitude (km)
  double temp; // temperature (centigrade)
} Sample;

int f_dimension();
void f_gradient(const double x[],double g[],int n,Sample samp[n]);
double calc_norm(const int dim, double v[]);
int optimize(const double alpha, const int dim, double x[],int n,Sample samp[],void (*calc_grad)(const double [], double [],int ,Sample []));
int cmp( const void *p, const void *q );

#endif