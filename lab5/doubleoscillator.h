#define DIM 4 //ATTENZIONE CAMBIATO DA 4 A 2

#include<stdlib.h>
#include<math.h>
#include "rungekuttaII.h"

#define g 9.81


typedef struct {double m, L;} parameters;
//typedef struct {double T, gamma, forz, Fomega;} parameters;




void drift (double *pin, double *pout, double t, double dt, void *param){
  
  parameters *P = (parameters *) param;

  /*pout[0] = dt * ( (pin[2] - pin[3]*cos(pin[0] - pin[1])) / (2*P->m*P->L*P->L - P->m*P->L*P->L*cos(pin[0] - pin[1])) );

  pout[1] = dt * ( (2*pin[3] - pin[2]*cos(pin[0] - pin[1])) / (2*P->m*P->L*P->L - P->m*P->L*P->L*cos(pin[0] - pin[1])) );

  pout[2] = P->m * P->L*P->L * pout[0]*pout[1] * sin(pin[0] - pin[1]) / (dt*dt);

  pout[3] = pout[2] - P->m * g * P->L * sin(pin[1]);*/


  double m = P->m;
  double L = P->L;
  
  pout[0] = dt * ( pin[2] - pin[3] * cos(pin[0] - pin[1]) ) / ( m * L * L ) / ( 2 - cos(pin[0] - pin[1]) * cos(pin[0] - pin[1]) );

  pout[1] = dt * ( 2*pin[3] - pin[2] * cos(pin[0] - pin[1]) ) / ( m * L * L ) / ( 2 - cos(pin[0] - pin[1]) * cos(pin[0] - pin[1]) );

  pout[2] = dt * m * L * ( L * sin(pin[0] - pin[1]) * pout[0]/dt * pout[1]/dt - 2 * g * sin(pin[0]) );

  pout[3] = - dt * m * L * ( L * sin(pin[0] - pin[1]) * pout[0]/dt * pout[1]/dt + g * sin(pin[1]) );

  /*double temp = pin [0];
  
  pout[0] = dt * pin[1];
  pout[1] = dt * (- ( 2 * M_PI/P->T) * (2 * M_PI/P->T) * sin(temp) - P->gamma * pin [1] + P->forz * cos( P->Fomega * t));*/
}
