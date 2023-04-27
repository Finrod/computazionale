#include<stdlib.h>
#include<stdio.h>
#include "doubleoscillator.h"



int main (int argc, char **argv ){

  parameters param = {1, 1};
  //parameters param = {2 * M_PI, 0., 0.2, 2./3.};
  double tempo = 0;

  FILE *pf;
  pf = fopen ("pendolodoppio.dat", "w");
  if(pf == NULL){
    printf("Something went wrong...\n");
    exit(-1);
  }

  FILE *gnuplotpipe = popen("gnuplot -p", "w");
  if(gnuplotpipe == NULL){
    printf("Something went wrong...\n");
    exit(-1);
  }


  double t  = 20;
  double dt  = 0.001;
  int tsteps = rint(t/dt);

  double phase [DIM] = {M_PI/2, -M_PI/2, 0, 0};
  //double phase [DIM] = {M_PI/2, 0.};

  double x1, x2, y1, y2;

  fprintf(pf, "# %16s %16s %16s %16s %16s\n", "time", "theta1", "theta2", "p1", "p2");
  fprintf(pf, "%16g %16g %16g %16g %16g\n", tempo, phase[0], phase[1], phase[2], phase[3]);   //ATTENZIONE TOLTO PHASE 3 E 4
  
  

  for(int i = 1; i <= tsteps; i++){
  
    tempo = i *dt;
    
    rungekutta(phase, phase, tempo, dt, (void*)&param);
    
    /*if(phase[0] > M_PI)
      phase[0] -= 2 * M_PI;
      else if(phase[0] < - M_PI)
      phase[0] += 2 * M_PI;   //QUESTO NON C'ERA PRIMA*/
    
    x1 = param.L*sin(phase[0]);
    y1 = -param.L*cos(phase[0]);
    x2 = x1 + param.L*sin(phase[1]);
    y2 = y1 - param.L*cos(phase[1]);
    
    fprintf(pf, "%16g %16g %16g %16g %16g\n", tempo, phase[0], phase[1], phase[2], phase[3]); //COME SOPRA

    for(int j=0; j<3; j++){

      fprintf(gnuplotpipe, "set term x11\n");
      
      fprintf(gnuplotpipe, "plot [%g:%g][%g:%g] '-' u 1:($1!=%g ? $2:1/0) title'' w l linecolor 1, '-' u 1:($1!=0 ? $2:1/0) title'' w lp pt 7 ps 2 linecolor 1\n", -2*param.L, 2*param.L, -2*param.L, 2*param.L, x2);


      //fprintf(gnuplotpipe, "plot [%g:%g][%g:%g] '-' u 1:($1==0 ? $2:1/0) title'' w lp pt 7 ps 2\n", -2*param.L, 2*param.L, -2*param.L, 2*param.L);


      
      fprintf(gnuplotpipe, "%d %d\n", 0, 0);
      fprintf(gnuplotpipe, "%16g %16g\n", x1, y1);
      fprintf(gnuplotpipe, "%16g %16g\n", x2, y2);

      fprintf(gnuplotpipe, "e\n");
      fflush(gnuplotpipe);
      }
  }

  
  fclose(pf);
  return 0;
}
