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
    max = (max > dat[i]) ? dat[i] : max;
    min = (min < dat[i]) ? dat[i] : min;
     
  }

  mean /= N;
  mean2 /= N;
  double stdev = mean2 - (mean*mean);
  
  double step = (max - min) / bins;

  int *hist = (int*) malloc(bins*sizeof(int));

  for(int i=0; i<bins; i++){
    int bin = (int) (dat[i] - min) / step;
    hist[bin]++;
  }

  hist[bins - 1] += hist[bins];

  return (stat){min, max, mean, mean2, stdev, hist, step, N, bins};
}


void print_statistic(stat statistic){

  printf("#FOLLOW PRINCIPAL STATISTICAL INDICATORS:\n");
  printf("#MEAN: %.4g\n", statistic.mean);
  printf("#MEAN2: %.4g\n", statistic.mean2);
  printf("#MAX: %.4g\n", statistic.max);
  printf("#MIN: %.4g\n", statistic.min);
  printf("#STDEV %.4g\n", statistic.stdev);
  printf("#HIST STEP: %.4g\n", statistic.step);

  printf("#HISTOGRAM %16s %16s %16s\n", "x", "P(x)", "Q(x)");
  double sum = 0;

  for(int i = 0; i<statistic.bins; i++){
    sum += statistic.hist[i];
    double x = (statistic.min + statistic.step/2)+ i*statistic.step;
    double P = statistic.hist[i] / statistic.N;
    double Q = sum / statistic.N;
    printf("%16g %16g %16g\n", x, P, Q);
  }

  printf("\n\n");
}
