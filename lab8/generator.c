#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "statistic.h"
#include "random.h"


int main(int argc, char *argv[]){

  if(argc!=6){
    printf("N BINS LCG_METHOD A B \n");
    printf("1-->MINIMAL_STD 2-->RANDU 3-->L'ECUYER2 4-->MY_LCG\n");
    printf("A B are parameters of the distribution (interval of definition || MU and SIGMA of the normal distribution)\n"); 
    exit(-1);
  }
  
  int N = atoi(argv[1]);
  int bins = atoi(argv[2]);
  int choice = atoi(argv[3]);
  double a = atof(argv[4]);
  double b = atof(argv[5]);

  if(N<=0 || bins<1 || choice<1 || choice>4){
    printf("Wrong imput:\n");
    printf("N>0 BINS>0 1<=LCG<=4\n");
    exit(-1);
  }

  seed = (RANDOM) time(NULL);

  double *array = (double *) malloc(N*sizeof (double));

  if(choice == 1)
    set_minimal_std();
  else if(choice == 2)
    set_randu();
  else if(choice == 3)
    set_Ecuyer2();
  else if(choice == 4){
    RANDOM A, B, M;
    printf("#Insert your LCG values (I'=(B+A*I)percentM):\n");
    printf("#a: ");
    scanf("%llu", &A);
    printf("#b: ");
    scanf("%llu", &B);
    printf("#mod: ");
    scanf("%llu", &M);
    set_my_LCG(A, B, M);                  
  }
    
#ifdef UNIFORM 
  for(int i=0; i<N; i++)
    array[i] = (b-a) * get_real() + a;
#endif

#ifdef GAUSSIAN
  for(int i = 0; i<N; i++)
    array[i] = b * get_normal()  + a;
#endif

#ifdef RANDU
  for(int i=0; i<N; i++)
    array[i] = (b-a) * get_real() + a;

  for(int i=2; i<N; i++)
  printf("RANDU %16g %16g %16g\n", array[i-2], array[i-1], array[i]);
  
  printf("\n\n");
#endif
  
  stat statistic = get_statistic(array, N, bins);
  print_statistic(statistic);
  
  free(array);
  
  return 0;
 }
