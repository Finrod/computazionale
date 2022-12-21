// Il programma crea N numeri random dal generatore GCL minimal standard e verifica che il primo e il secondo momento siano corretti

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define a 16807
#define m 2147483647
#define DRAND ((double)minimal_standard()/m)

unsigned long long int myrand;

unsigned long long int minimal_standard(){

  myrand=(myrand*a)%m;
  return myrand;
}

void x_medio(double* data,int N){

  int i;
  double mean,err,this;

  mean=err=0;
  
  for(i=0;i<N;i++){
    this=data[i];
    mean+=this;
    err+=this*this;
  }
  mean/=(double)N;
  err/=(double)N;

  printf("# x_medio: %g  errore: %g\n",mean,sqrt((err-mean*mean)/(double)N));
  
}

void x_quad(double* data,int N){

  int i;
  double mean,err,this;

  mean=err=0;
  
  for(i=0;i<N;i++){
    this=data[i];
    mean+=this*this;
    err+=this*this*this*this;
  }

  mean/=(double)N;
  err/=(double)N;

  printf("# x_quad: %g  errore: %g\n",mean,sqrt((err-mean*mean)/(double)N));
  
}

int main(int argc, char *argv[]){

  int N,i;
  double *d_rand;
  
  if(argc!=2 && argc!=3){
    fprintf(stderr,"usage %s N [seed]\n",argv[0]);
    exit(1);
  }

  N=atoi(argv[1]);

  if(argc==2){
    FILE *f=fopen("/dev/urandom","r");
    fread(&myrand,4,1,f);
    fclose(f);
  }else{
    myrand=(unsigned long long)(argv[2]);
  }

  d_rand=malloc(N*sizeof(double));

  printf("# %d numeri random generati con gcl Minimal Standard\n",N);
  printf("# valor medio atteso: 0.5 ; momento secondo atteso: 1/3\n");
  printf("# verifica dei momenti 1 e 2 alla fine del file\n");
  
  for(i=0;i<N;i++){
    d_rand[i]=DRAND;
    printf("%g\n",d_rand[i]);
  }

  x_medio(d_rand,N);
  x_quad(d_rand,N);

  free(d_rand);
  
  return 0;
}

