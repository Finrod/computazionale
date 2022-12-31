/* -> PLOT: stampa su file la traiettoria del camminatore e, se possibile, ne stampa il grafico;
   -> MSD: stampa in std output ad ogni tempo la media delle posizoni per ogni coordinata spaziale e lo spostamento quadratico medio;
   -> HISTO: se possibile stampa al tempo finale
                                                1. l'istogramma e la distribuzione cumulativa per due coodinate spaziali
						2. istogramma e la distribuzione cumulativa della distanza dall'origine
						3. in std output i valori per costruire l'istogramma di P(x,y) 
*/


#include "random.h"
#include "statistic.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>




#define UNMEZZO 0.5


/****************************************/


int main(int argc, char *argv[]){

  if(argc != 4){
    printf("Wrong imput! --> DIMESIONS STEPS STORIES\n");
    exit(-1);
  }

  seed = time(NULL);
  set_Ecuyer2();
  
  int DIM = atoi(argv[1]);
  int STEPS = atoi(argv[2]);
  int STORIE = atoi(argv[3]);
  if(DIM<1 || STORIE<1 || STEPS<1){
    printf("Wrong impunt! Imput values must be >= 1\n");
    exit(-1);
  }
  
  int *X = (int *) calloc (STORIE*DIM, sizeof(int));               //ARRAY CHE CONTIENE LE COORDINATE DI TUTTE LE PARTICELLE AD UN CERTO TEMPO t
                                                                   /*L'array al tempo iniziale e' costruito in questo modo:
								          | X1 | X2 | X3 | ...... | x_DIM |
									  ---------------------------------
								     | S1 |  0 |  0 |  0 | ...... |   0   |
								     | S2 |  0 |  0 |  0 | ...... |   0   |
								     | S3 |  0 |  0 |  0 |
								       .     .
								       .     .
								       .     .
								       .
								       .
								     | S_STORIE |

								    */
  if(X==NULL){
    printf("Something went wrong during the allocation!  :(\n");
    exit(-1);
  }
  

#ifdef MSD
  double *MEAN  = (double *)  calloc (DIM, sizeof(double));         //ARRAY DELLE MEDIE
  double *MEAN2  = (double *)  calloc (DIM, sizeof(double));        //ARRAY DELLE MEDIE^2
  
  if(MEAN == NULL || MEAN2 == NULL){
    printf("Something went wrong during the allocation!  :(\n");
    exit(-1);
  } 
#endif
  
  
#ifdef HISTO 
 double *x1 = (double*) calloc(STORIE, sizeof(double));            //ARRAY PER ISTOGRAMMI DI UNA VARIABILE E PER LA DISTANZA
 double *x2 = (double*) calloc(STORIE, sizeof(double));           
 double *DISTANCE = (double*) calloc(STORIE, sizeof(double));
 
 if(x1 == NULL || x2 == NULL || DISTANCE == NULL){
   printf("Something went wrong during the allocation!  :(\n");
   exit(-1);
 }
#endif
 

#ifdef PLOT
 int COMANDI = 6;
 char *comandi[]={"set term x11 0", "set title 'Grafico del camminatore'", "", "", "", ""};
 
 FILE *gnuplotPipe;
 gnuplotPipe = popen ("gnuplot -persistent","w");
 if(gnuplotPipe == NULL){
   printf("Something went wrong...\n");
   exit(-1);
 }
 
 if(DIM < 4 && STORIE == 1){
   if(DIM == 1){
     comandi[2] = "set xlabel 't'";
     comandi[3] = "set ylabel 'x'";
     comandi[4] = "plot 'plot.dat' u 2:5 title 'x(t)' pt '.'";
   }
   if(DIM == 2){
     comandi[2] = "set xlabel 'x'";
     comandi[3] = "set ylabel 'y'";
     comandi[4] = "plot 'plot.dat' u 5:6 title 'y(x)' pt '.'";
   }
   if(DIM == 3){
     comandi[2] = "set xlabel 'x'";
     comandi[3] = "set ylabel 'y'";
     comandi[4] = "set zlabel 'z'";
     comandi[5] = "splot 'plot.dat' u 5:6:7 title 'z(x,y)' pt '.'";
   }
 }
  
 FILE *pf;
 pf = fopen ("plot.dat", "w");
 if(pf == NULL){
   printf("Something went wrong...\n");
   exit(-1);
 }
 
 for(int j=0; j<STORIE; j++){
   fprintf(pf, "T %d S %d  ", 0, j+1);                         //STAMPA INIZIALE t=0 
   for(int k=0; k<DIM; k++){
     fprintf(pf, "%d ", 0);
   }
   fprintf(pf, "\n");
 }
 fprintf(pf, "\n\n");
#endif


#ifdef MSD
 printf("%16s ", "t");
  for(int k=0; k<DIM; k++)
    printf("%16s%d%s ", "<x_", k+1, ">");
  printf("%16s\n", "MSD");
  
  printf("%16d", 0);                                           //PRIMA STAMPA t=0
  for(int k=0; k<DIM; k++)
    printf("%16d", 0);
  printf("%16d\n", 0);
#endif    



  

/***************************SCORRO GLI STEPS *********************************/
  
  for(int t=0; t<STEPS; t++){                            

#ifdef MSD    
    double msd = 0;
#endif 

/****************************SCORRO LE STORIE ********************************/
    
    for(int j=0; j<STORIE; j++){                         

      int direzione = get_intero (2 * DIM - 1);
      int spostamento = (direzione % 2) ? 1 : -1 ;                  //capisco se va a destra o sinistra
      direzione = direzione * UNMEZZO;                              //determino la componente da incrementare

      X[direzione + j * DIM] += spostamento;                        //faccio lo spostamento


#ifdef MSD   
      MEAN[direzione] += X[direzione + j * DIM];                          
      MEAN2[direzione] += X[direzione + j * DIM] * X[direzione + j * DIM];
#endif

      
#ifdef PLOT
      fprintf(pf, "T %d S %d  ", t+1, j+1);                        //STAMPA DELLE POSIIZONI AL TEMPO T E ALLA STORIA S
      for(int k=0; k<DIM; k++)
	fprintf(pf, "%d ", X[k + j * DIM]);
      fprintf(pf, "\n");
#endif    
      
    }

    
#ifdef PLOT
    fprintf(pf, "\n\n");
#endif
        

#ifdef MSD         
    printf("%16d", t + 1);                                         //STAMPA DELLA STATISTICA
    for(int k=0; k<DIM; k++){ 
      MEAN[k] /= STORIE;
      MEAN2[k] /= STORIE;
      msd += MEAN2[k];   
      printf("%16g", MEAN[k]);
      MEAN[k]=0;
      MEAN2[k]=0;
    }   
    printf("%16g\n", msd);  
    msd = 0;
#endif

  }

#ifdef PLOT
  fclose(pf);	
  if(DIM < 4 && STORIE == 1){
    for(int i=0; i<COMANDI; i++)
      fprintf(gnuplotPipe, "%s\n", comandi[i]);
    pclose(gnuplotPipe);
  }
#endif

  
#ifdef HISTO 
  int Xbins;
  int Ybins;
  double Xstep;
  double Ystep;
  double M;
  int *HIST;
  stat statX;
  stat statY;


  for(int j=0; j<STORIE; j++){
    x1[j] = X[j * DIM];
    if(DIM >= 2)
      x2[j] = X[j * DIM + 1];
  }

  if(DIM >= 2){
    statY = get_statistic(x2, STORIE);
    print_statistic( statY );                                       //ISTOGRAMMA PER UN'UNICA VARIABILE
  }  
  statX = get_statistic(x1, STORIE);
  print_statistic( statX );                                         //L'ISTOGRAMMA PER UN'UNICA VARIABILE


  
  if(DIM == 2){
    Xbins = statX.bins;                                             //LA SCELTA DI QUANTI BINS E' NELLA FUNZIONE "get_statistic"
    Ybins = statY.bins;
    
    Xstep = (statX.max - statX.min) / (double) Xbins;
    Ystep = (statY.max - statY.min) / (double) Ybins;
    M = 1. / ((double) STORIE * Xstep * Ystep);                      //COSTANTE DI NORMALIZZAZIONE

  
    HIST = (int *)  calloc ( (Xbins * Ybins) , sizeof(int));         //MATRICE ISTOGRAMMA
    if(HIST == NULL){
      printf("Something went wrong during the allocation!  :(\n");
      exit(-1);
    }
  }

  for(int j=0; j<STORIE; j++){
    if(DIM == 2){
      int binx = (x1[j] - statX.min) / Xstep + 0.5;
      int biny = (x2[j] - statY.min) / Ystep + 0.5;
    
      
      int pos = binx + Xbins * biny;
      HIST[pos] ++;
    }
    
    double sum = 0;
    for(int i=0; i<DIM; i++)
      sum += X[j * DIM + i] * X[j * DIM + i];
    DISTANCE[j] = pow(sum, 0.5);
    
  }

  print_statistic( get_statistic( DISTANCE, STORIE));
  
  if(DIM == 2){
    printf("#%16s %16s %16s", "x", "y", "P(x,y)");
    for(int i=0; i<Ybins; i++){
      for(int j=0; j<Xbins; j++){
	
	double x = (statX.min)+ j*Xstep;
	double y = (statY.min)+ i*Ystep;
	double P = (double)  HIST[j + i * Xbins] * M;

	if(P != 0)
	  printf("%16g %16g %16g\n", x, y, P);
      }
    }
  }

  
  if(DIM == 2)
    free(HIST);
  free(DISTANCE);
  free(x1);
  free(x2);
#endif

#ifdef MSD
  free(MEAN);
  free(MEAN2);
#endif

  free(X);
    
  return 0;
}
