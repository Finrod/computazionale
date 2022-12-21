#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "random.h"

#define PSTEP 0.001    //step di incremento della probabilità di "UP" per la ricerca di P_cluster
#define UP +1
#define DOWN -1
#define RAND get_real()
#define DESTRA 0
#define SOTTO 1

struct sito{
  int spin;
  struct sito *destra;
  struct sito *sotto;
} *rete;

void initializing(int S, double prob){

  rete = (struct sito *)calloc(S, sizeof(struct sito));
  
  for(int i=0; i<S; i++){
    if(RAND < prob)
      rete[i].spin = UP;
    else
      rete[i].spin = DOWN;
  }
}

int siteUpdate(int posto, int **vicini, int direzione){

  int postoVicino = vicini[posto][direzione]
  
  if(rete[posto].spin == rete[postoVicino].spin){

    ...

  }
    
  return;
}


int main(int argc, char *argv[]){

  int L;
  int Storie;
  double ProbGuasto;
  
  L = atoi(argv[1]);
  Storie = atoi(argv[2]);

  set_Ecuyer2();
  seed = time(NULL)

  int fatto = 0;
  int S = L*L;
  ProbUp = PSTEP;

  int **vicini = (int **)malloc(S * sizeof(int *));
  for(int j=0; j<S; j++)
    vicini[j] = (int *)malloc(2 * sizeof(int));
  //riempire l'array vicini
  for(int j=0; j<S; j++){
    if((j%L) != L-1)
      vicini[j][DESTRA] = j+1;
    else
      vicini[j][DESTRA] = j - (L-1);
    
    if(j/L != L-1)
      vicini[j][SOTTO] = j + L;
    else
      vicini[j][SOTTO] = j - (L-1) * L;
  }


  while(fatto != 1){
    
    for(int i=0; i<Storie; i++){
           
      initializing(S, ProbUp);




      int Ncambiamenti;
      
      while(Ncambiamenti != 0){

	Ncambiamenti = 0;
	
	for(int j=0; j<S; j++){
	  Ncambiamenti += siteUpdate(j, vicini, DESTRA);
	  Ncambiamenti += siteUpdate(j, vicini, SOTTO);
	}
      }
      free(rete);
    }
    ProbGuasto += PSTEP;
  }
  
  

  return 0;
}
