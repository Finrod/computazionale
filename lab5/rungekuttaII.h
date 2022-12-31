#include<stdlib.h>
#include<math.h>


void psum (double *a, double *b, double *sum){
  for (int i=0; i<DIM ; i++)
    sum [i] = a [i] + b [i];
}
void pfma (double c, double *a, double *b, double *result){      // result = a*c + b
  for (int i=0; i<DIM ; i++)
    result [i] = fma (c, a [i], b[i]);
}

void drift(double *arrin, double *arrout, double t, double dt, void *param);
void energy (double *E, double *p, void *param);
void rungekutta ( double *arrin, double *arrout, double t, double dt, void *param){

  double k [DIM];
  double h [DIM];

  //k1
  drift (arrin, k, t, dt, param);

  //k2
  pfma (0.5, k, arrin, h);
  drift (h, k, t + 0.5 * dt, dt, param);
  psum (k, arrin, arrout);
}
  
