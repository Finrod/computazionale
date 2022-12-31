#include "random.h"
#include <time.h>
#include <stdio.h>
#include <mcheck.h>


#define MY_EMPTY 0
#define DIM 2
#define UNMEZZO 0.5




void shuffle(int *V, int N){
  for(int i=0; i<N; i++)
    V[i]=i;
  
  for(int i=0; i<N; i++){
    int r = get_intero(i, N-1);
    int aux =  V[i];
    V[i] = V[r];
    V[r] = aux;
  }
}



int periodic(int a, int b){

  int module = a % b;
  if(module < 0)
    module += b;

  return module;
}

int DA_x_A_i(int *Pos, int L, int riga){                     //converte dalle coordinate al corrispondente indice una casella di una matrice
  int i;
  if(DIM == 2)
    i = Pos[riga] + L * Pos[riga + 1];
  else if(DIM == 3)
    i = Pos[riga] + L * Pos[riga + 1] + L * L * Pos[riga + 2];
  return i;
}

void spostamento(int *spazio, int *Pos, int *realPos, int *name, int N, int L){
  
  shuffle(name, N);                                          //shuffle per decidere l'ordine di spostamento

  for(int i=0; i<N; i++){

    int oldPlace = DA_x_A_i(Pos, L, DIM*name[i]);             /*dalle coordinate passo all'indice della griglia prima dello spostamento
								(DIM*name[i] inidvidua la riga dell'array corrispondente alla name[i] particella)*/
    
    int indice = get_intero (0, 2 * DIM - 1);
    int spostamento = (indice % 2) ? 1 : -1 ;                 //determino il verso (spostamento puo' assumere valore +1 o -1)
    indice *= UNMEZZO;                                        //determino la direzione (cioè qual è la coordinata da incremenentare)
 
    indice += DIM * name[i];                                              
    Pos[ indice ] +=  spostamento;

    Pos[ indice ] = periodic(Pos[ indice ], L);               //condizioni al contorno

    int newPlace = DA_x_A_i(Pos, L, DIM*name[i]);             //dalle coordinate passo all'indice della griglia dopo lo spostamento
    
    if(spazio[ newPlace ] == MY_EMPTY){
      
      spazio[ newPlace ] = name[i]+1;
      spazio[ oldPlace ] = MY_EMPTY;
      realPos[ indice ] += spostamento;      
    }
    
    else{
      Pos [ indice ] -= spostamento;
      Pos[ indice ] = periodic(Pos[ indice ], L);
    }
  }
}


double distance2(int *A, int *B, int i){                             //R^2  

  double d2 = 0;
  
  for(int j=0; j<DIM; j++){
    int index = i * DIM + j;
    d2 += (A[ index ] - B[ index ]) * (A[ index ] - B[ index ]);
  }

  return d2;
}


double measure(int *Pos, int *zeroPos, int N, double *incertezze, int j){
  
  double mean = 0.;
  double mean2 = 0.;
  
  for(int i=0; i<N; i++){
    mean += distance2(Pos, zeroPos, i);
    mean2 += distance2(Pos, zeroPos, i) * distance2(Pos, zeroPos, i);
  }

  mean /= N;                                                          //media sulle particelle della distanza^2 percorsa
  mean2 /= N;
  
  incertezze[j] = sqrt((mean2 - mean*mean)/ N);                       //incertezza sulla stimata tramite std
  
  return mean;  
}


void DA_i_A_xyz(int *coordinate, int indice, int L){                  //converte da un indice alle corrispondenti coordinate di una casella di una matrice
  if(DIM == 2){
    coordinate[1] = indice / L;
    coordinate[0] = indice % L;
  }

  if(DIM == 3){
    coordinate[2] = indice / (L*L);
    int aux = indice - coordinate[2] * L * L;
    coordinate[1] = aux / L;
    coordinate[0] = aux % L;
  } 
}


void initializing(int *SPACE, int *zeroPos, int *Pos, int *realPos, int *name, int N, int L, int V){

    for(int i=0; i<V; i++)
      SPACE[i] = MY_EMPTY;

    
    shuffle(name, V);                                      //shuffle per decidere dove mettere le particelle (l'array name contiene l'indice della griglia)
    
    
    for(int i=0; i<N; i++){
      SPACE[name[i]] = i+1;
      
      int coordinate[DIM];
      DA_i_A_xyz(coordinate, name[i], L);

      for(int j=0; j<DIM; j++){
	zeroPos[i * DIM + j] = coordinate[j];
	Pos[i * DIM + j] = coordinate[j];
	realPos[i * DIM + j] = coordinate[j];
      }
    }
}



 void plot( FILE *gnuplotPipe, int *Pos, int N, int L, int V){
  
  fprintf(gnuplotPipe, "set term x11\n");
  fprintf(gnuplotPipe, "set xlabel 'x'\n set ylabel 'y'\n set zlabel 'z'\n set title 'V=%d'\n", V);

  if(DIM == 2){
    fprintf(gnuplotPipe, "plot [0:%d][0:%d] '-' u 1:2 title '' w p pt 7 ps 2\n", L, L);

    for(int i=0; i<N; i++)
      fprintf(gnuplotPipe, "%16d %16d\n", Pos[i * DIM], Pos[i * DIM + 1]);
  }
  else if(DIM == 3){
    fprintf(gnuplotPipe, "splot [0:%d][0:%d][0:%d] '-' u 1:2:3 title '' w p pt 7 ps 1\n", L, L, L);

    for(int i=0; i<N; i++)
      fprintf(gnuplotPipe, "%16d %16d %16d\n", Pos[i * DIM], Pos[i * DIM + 1], Pos[i * DIM + 2]);
  }
  fprintf(gnuplotPipe, "e\n");
  fflush(gnuplotPipe);
}





/*************************************MAIN****************************************/

int main(int argc, char *argv[]){

#ifdef CLOCK
  clock_t begin = clock();
#endif

  if(DIM != 2 && DIM !=3){
    printf("Wrong dimensions. Dimensions allowed are 2 or 3\n");
    exit(-1);
  }
  
  if(argc != 5){
    printf("Wrong imput -> RHO(volumetric density) LENGTH NUM_STEPS STORIES\n");
    exit(-1);
  }  

  double rho = atof(argv[1]);
  int L = atoi(argv[2]);
  int STEPS = atoi(argv[3]);
  int STORIES = atoi(argv[4]);
  if(rho<0 || L<0 || STEPS<0 || STORIES<0){
    printf("Wrong imput, imput values must be positive!\n");
    exit(-1);
  }

  int V;
  if(DIM == 2)
    V = L*L;
  else
    V = L*L*L;
  int N =  rho * V;
  double A = 0.;
  double sum_pesi = 0.;

  int *SPACE = (int *) calloc(V, sizeof(int));                       //MATRICE GRIGLIA
  int *Pos = (int *) calloc(N * DIM, sizeof(int));                   //POSIZIONI DELLE PARTICELLE CONSIDERANDO LE CONDIZIONI AL BORDO
  int *zeroPos = (int *) calloc(N * DIM, sizeof(int));               //POSIIZONI INIZIALI
  int *realPos = (int *) calloc(N * DIM, sizeof(int));               //POSIZIONI REALI DELLE PARTICELLE
  
  /*Gli array Pos, zeroPos e realPos ad un generico tempo sono costruiti in questo modo (DIM=3):
								                                      | X1 | X2 | X3 | 
									                              ----------------
								                                 | p1 |  5 |  7 |  7 |
								                                 | p2 |  9 |  0 |  0 |
								                                 | p3 |  4 |  8 |  2 |
												 .     .
												 .     .
												 .     .
												 .
												 .
												 | p_N |                                                             (p=particella)

  */
  
  
  int *aux = (int *) calloc(V, sizeof(int));                          //ARRAY AUSILIARIO PER INIZIALIZZARE ALEATORIAMENTE IL RETICOLO 
  int *name = (int *) calloc(N, sizeof(int));                         //CONTIENE I FLAG PER OGNI PARTICELLA (p1 --> 1,  p2 --> 2, ...) 
  double *incertezza = (double *) calloc(STORIES, sizeof(double));

  
  if(SPACE == NULL || zeroPos == NULL || Pos == NULL || realPos == NULL || name == NULL || aux == NULL){
    printf("Something went wrong during the allocation! :(\n");
    exit(-1);
  }

#ifdef PLOT  
  FILE *gnuplotPipe = popen("gnuplot -p", "w");
#endif
  
  seed = time(NULL);
  set_Ecuyer2();
  
#ifdef LEGENDA
  printf("#RETICULAR GAS :    GRID_SIZE %5d DENSITY %5g TIME_STEPS %5d\n", L, rho, STEPS);
  printf("#              :    PARTICOLS_NUMBER %5d VOLUME %5d STORIES %5d\n", N, V, STORIES);
#endif
  

  for(int j=0; j<STORIES; j++){                                   /*************STORIE*************/
        
    initializing(SPACE, zeroPos, Pos, realPos, aux, N, L, V);
    
    for(int t=0; t<STEPS; t++){                                   /***********STEP TEMPORALE***********/

      spostamento(SPACE, Pos, realPos, name, N, L);               /*************MOVIMENTO*************/

#ifdef PLOT      
      if(STORIES == 1)
	plot(gnuplotPipe, Pos, N, L, V);
#endif
      
    }
                                                                   /**************MISURE***************/
    double deltaR2 = measure(realPos, zeroPos, N, incertezza, j);  
    A += deltaR2 * 1/(incertezza[j] * incertezza[j]);
    sum_pesi += 1/(incertezza[j] * incertezza[j]);
  }
  
  A /= (sum_pesi * 2 * DIM * STEPS);                              //spostamento quadratico medio mediato sulle storie e pesato con l'inverso delle incertezze^2
  double sigma_A = 1 / (sqrt(sum_pesi) * 2 * DIM * STEPS);        //incertezza sulla media pesata
  printf("%16d %16g %16g %16g\n", STEPS, rho, A, sigma_A);  

#ifdef CLOCK
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("#time spent: %g\n", time_spent);
#endif
   
  free(SPACE);
  free(Pos);
  free(zeroPos);
  free(realPos);
  free(aux);
  free(name);
  free(incertezza);

  return 0;
}
