#include<stdio.h>

typedef struct {
  double min;
  double max;
  double mean;
  double mean2;
  double stdev;
  int *hist;
  double step;
  int N;
  int bins;
} stat;

stat get_statistic(double *dat, int N, int bins){

  double min=0;
  double max=0;
  double mean=0;
  double mean2=0;
  
  for(int i=0; i<N; i++){
    mean += dat[i];
    mean2 += dat[i] * dat[i];
    max = (max < dat[i]) ? dat[i] : max;
    min = (min > dat[i]) ? dat[i] : min;
     
  }

  mean /= N;
  mean2 /= N;
  double stdev = mean2 - (mean*mean);
  
  double step = (max - min) / bins;

  int *hist = (int*) malloc(bins*sizeof(int));

  for(int i=0; i<N; i++){
    int bin =  (dat[i] - min) / step + 0.5;
    hist[bin]++;
  }
  
      
  return (stat){min, max, mean, mean2, stdev, hist, step, N, bins};
}


void print_statistic(stat statistic){

  FILE *pf;
  pf = fopen ("__stat_analysis__.dat", "w");
    
  FILE *gnuplotPipe;
  gnuplotPipe = popen ("gnuplot -persistent","w");

  if(gnuplotPipe == NULL || pf == NULL)
    printf("Something went wrong...");


  char *comandi[] = {"set term wxt 0", "set title 'Probability distribution'", "set xlabel 'x'", "set ylabel 'P'", "plot [:][0:]  '__stat_analysis__.dat' u 1:2 title 'P(x)' w boxes", "set term wxt 1", "set title 'Cumulative distribution'", "set xlabel 'x'", "set ylabel 'Q'", "plot '__stat_analysis__.dat' u 1:3 title 'Q(x)' w l"};

  fprintf(pf, "#FOLLOW PRINCIPAL STATISTICAL INDICATORS:\n");
  fprintf(pf, "#MEAN: %.4g\n", statistic.mean);
  fprintf(pf, "#MEAN2: %.4g\n", statistic.mean2);
  fprintf(pf, "#MAX: %.4g\n", statistic.max);
  fprintf(pf, "#MIN: %.4g\n", statistic.min);
  fprintf(pf, "#STDEV %.4g\n", statistic.stdev);
  fprintf(pf, "#HIST STEP: %.4g\n", statistic.step);

  fprintf(pf, "#HISTO %16s %16s %16s\n", "x", "P(x)", "Q(x)");
  double sum = 0;

  for(int i = 0; i<statistic.bins; i++){
    sum += statistic.hist[i];
    double x = (statistic.min)+ i*statistic.step;
    double P = (double) statistic.hist[i] / statistic.N;
    double Q = sum / statistic.N;
    fprintf(pf, "%16g %16g %16g\n", x, P, Q);
  }

  for(int i=0; i<10; i++)
    fprintf(gnuplotPipe, "%s\n", comandi[i]);

  free(statistic.hist);
  fclose(pf);
  pclose(gnuplotPipe);
}
