#include<stdlib.h>
#include<stdio.h>
#include "oscillator.h"



int main (int argc, char **argv ){

  parameters param = {2 * M_PI, 0.5, 1.5, 2./3.};
  double tempo = 0;

  FILE *gnuplotPipe;
  gnuplotPipe = popen ("gnuplot -persistent","w");
  if(gnuplotPipe == NULL){
    printf("Something went wrong...\n");
    exit(-1);
  }
  
  FILE *pf;
  pf = fopen ("pendolo.dat", "w");
  if(pf == NULL){
    printf("Something went wrong...\n");
    exit(-1);
  }
  
#ifdef SIMULATION
  double t  = 100;
  double dt  = 0.001;
  int tsteps = rint(t/dt);
  double phase [DIM] = {M_PI/2, 0.};
  double E, E0, err = 0;
  
  int COMANDI = 10;
  char *comandi[] = {"set term wxt 0", "set title 'Grafico della traiettoria'", "set xlabel 't[s]'", "set ylabel 'x'", "plot  'pendolo.dat' u 1:2 title 'x(t)' w lines", "set term wxt 1", "set title 'Spazio delle fasi'", "set xlabel 'x'", "set ylabel 'dx/dt'", "plot  'pendolo.dat' u 2:3 title 'v(x)' w points pt 7", };
#endif
  
#ifdef POINCARE
  double Fperiod = 2 * M_PI / param.Fomega;
  double t  = Fperiod * 3100.;
  double dt  = Fperiod / 1000.;
  int tsteps = rint(t/dt);
  double term = Fperiod * 100.;
  double phase [DIM] = {M_PI /2., 0.};

  int COMANDI = 5;
  char *comandi[] = {"set term wxt 0", "set title 'Sezione di Poincaré'", "set xlabel 'x'", "set ylabel 'dx/dt'",  "plot  'pendolo.dat' u 2:3 title 'v(x)'  w points pt 7 lc 3"};
#endif

#ifdef ATTRAZIONE
  double t = 93.;
  double dt = 0.01;
  int tsteps = rint(t/dt);
  double x0;
  double v0;
  double delta = M_PI / 500.;

  int COMANDI = 5;
  char *comandi[] = {"set term x11 0", "set title 'Bacini attrazione'", "set xlabel 'x_0'", "set ylabel 'v_0'",  "plot [-pi:pi][-pi:pi]  '<grep P attrazione1.5.dat' u 2:3 title '' w points pt '.' lc 5, '<grep M attrazione1.5.dat' u 2:3 title '' w points pt '.' lc 8"};
  
  for(x0=-M_PI; x0<M_PI; x0+=delta){
    for(v0=-M_PI; v0<M_PI; v0+=delta){
      double phase [DIM]= {x0,v0};
#endif

#ifdef BIFORCAZIONE
   double Fperiod = 2 * M_PI / param.Fomega;
   double t  = Fperiod * 100.;
   double dt  = Fperiod / 100.;
   int tsteps = rint(t/dt);
   double phase [DIM] = {M_PI/2., 0.};

   double fmin = 0.9;
   double fmax = 1.5;
   double df = 0.0001;

   int COMANDI = 5;
   char *comandi[] = {"set term x11 0", "set title 'Diagramma di biforcazione'", "set xlabel 'F_0'", "set ylabel 'v'",  "plot 'pendolo.dat' title 'v(F_0)' w points pt '.' lc 8"};

   for(double f=fmin; f<=fmax; f+=df){
     param.forz=f;
#endif
  
  
#ifdef SIMULATION
  energy(&E0, phase, &param);
  
  fprintf(pf, "# %16s %16s %16s %16s %16s\n", "time", "x", "v", "Energy", "err");
  fprintf(pf, "%16g %16g %16g %16g %16g\n", tempo, phase[0], phase[1], E0, err);
#endif
  

  for(int i = 1; i <= tsteps; i++){
  
      tempo = i *dt;
      
      rungekutta(phase, phase, tempo, dt, (void*)&param);
      
     
      if(phase[0] > M_PI)
	phase[0] -= 2 * M_PI;
      else if(phase[0] < - M_PI)
	phase[0] += 2 * M_PI;

#ifdef SIMULATION
      energy(&E, phase, &param);
      err = (E-E0) / E0;
      fprintf(pf, "%16g %16g %16g %16g %16g\n", tempo, phase[0], phase[1], E, err);
#endif

#ifdef POINCARE
      if((tempo > term) && (i%1000 == 0))
	fprintf(pf, "%16g %16g %16g\n", tempo,  phase[0], phase[1]);
#endif
  }
   

#ifdef ATTRAZIONE  
  if(phase[1]>0)
    fprintf(pf ,"P %16g %16g\n", x0, v0);
  else
    fprintf(pf, "M %16g %16g\n", x0, v0);
    }
  }  
#endif

#ifdef BIFORCAZIONE
    fprintf(pf, "%16g %16g\n", f, phase[1]);
  }
#endif

  
  for (int i=0; i < COMANDI; i++)
    fprintf(gnuplotPipe, "%s\n", comandi[i]);

  fclose(pf);
  pclose(gnuplotPipe);

  return 0;
}
