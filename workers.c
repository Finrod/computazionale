#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define MAXN 100001


int min_workers(int N, int T, int Ti[]) {

  long int sum=0;
  long int NW;
  int imin;
  int imax;
  int time;
  int fatto=0;

  for(int i = 0; i<N ; i++)
    sum+=Ti[i];
  
  NW=sum/T + 1;

  int* W = (int *) calloc(MAXN, sizeof(int));

  
  for(int i=0; i<NW; i++)
    W[i]=Ti[i]; 

  NW--;

  while((fatto == 0)){
    NW++;
    imin=0;
    imax=0;

    W[NW-1]=Ti[NW-1]; 


    for(int j=NW; j<N; j++){      //scorro tutto Ti a partire da W
      
      for(int i=0; i<NW; i++){
	if(W[i] < W[imin])
	  imin=i;
	else if((j=N-1) && (W[i] > W[imax]))
	  imax=i;
      }
      
      W[imin]=W[imin]+Ti[j];      
    }

    if(W[imin] > W[imax])
      time=W[imin];
    else
      time=W[imax];

    if(time<T || (NW == MAXN-1))
      fatto=1;
  }

  free(W);
  return NW;
}

int main() {

     freopen("2023-workers_workers-3_1675790312.txt", "r", stdin); // DECOMMENTA QUA SE VUOI LEGGERE DA FILE
    // freopen("output.txt", "w", stdout); // DECOMMENTA QUA SE VUOI SCRIVERE DA FILE

    int N, T;
    int Ti[MAXN];

    assert(2 == scanf("%d %d", &N, &T));

    for(int i=0; i<N; i++) 
        assert(1 == scanf("%d", &Ti[i]));

    printf("%d\n", min_workers(N, T, Ti));

    return 0;
}
