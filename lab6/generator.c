#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "statistic_new.h"
#include "random.h"


int main(int argc, char *argv[]){

  if(argc<3){
    printf("insert: N LGC_METHOD\n");
    printf("1-->MINIMAL STD\t 2-->RANDU\t 3-->ECUYER2 4-->MY LCG\n");
    exit(-1);
  }
  
  int N = atoi(argv[1]);
  int choice = atoi(argv[2]);

  seed = time(0);
  
  double *array = (double *) calloc(N, sizeof (double));
  if(choice == 1)
    set_minimal_std();
  else if(choice == 2)
    set_randu();
  else if(choice == 4)
    set_my_LCG(1, 2, 3);
  else if(choice == 3)
    set_Ecuyer2();

 
  for(int i=0; i<N; i++)
    array[i] = get_real();
  
  stat statistic = get_statistic(array, N);
  print_statistic(statistic);
  return 0;
}
