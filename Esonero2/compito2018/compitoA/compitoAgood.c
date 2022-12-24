#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned long long RTYPE;

/*generatore di l'Ecuyer (GCL puramente moltiplicativo a 64 bit)*/
#define MOLTIPLICATORE (1181783497276652981ULL)

struct walker {
  int x, y;
  int direzioneProibita;
  /*
    direzioneProibita prende valori 0,1,2,3.
    Ogni valore corrisponde ad una delle quattro direzioni nel reticolo.
                              (1)
    0 direzione x+             y+
    1 direzione y+             |   
    2 direzione x-    (2)  x- --- x+ (0)
    3 direzione y-             |
                               y-
                              (3)
    Esempio: se in un dato istante il camminatore 
    si muove nella direzione 2 (x-), si imposta
    direzioneProibita=0;
    al tempo successivo potrà continuare verso sinistra 
    (2, x-), oppure andare in alto (1, y+), o in basso (3, y-)
    con uguale probabilità.
    Esempio2: se in un dato istante il camminatore 
    si muove nella direzione 1 (y+), si imposta
    direzioneProibita=3;
    al tempo successivo potrà continuare verso l'alto 
    (1, y+), oppure andare a destra (0, x+), o a sinistra (2, x-)
    con uguale probabilità.
   */
};

void    stampa_finale   (int kmax, int numw, 
			 double* r2, double* rw);
void    un_passo        (RTYPE* seed, struct walker* w);
void    una_passeggiata (RTYPE* seed, struct walker* w, 
			 int kmax, double *r2, double* r4);
void    controllo_e_init(int argc, char** argv, RTYPE* seed, 
			 int* kmax, int* numw);
double* alloca_double   (int quanti);

/** MAIN **/

int main(int argc, char** argv){

  RTYPE seed; 
  int kmax, numw, n;
  /*Il tempo massimo sara' 2^kmax (potenza kmax-esima di 2)*/
  struct walker zonzo;
  double *r2, *r4;

  controllo_e_init(argc,argv, &seed, &kmax, &numw);

  r2=alloca_double(kmax);
  r4=alloca_double(kmax);

  for (n=0;n<numw;n++) {
    una_passeggiata(&seed, &zonzo, kmax, r2, r4);
  }

  stampa_finale(kmax,numw,r2,r4);

  free(r2); free(r4);

  return 0;    
}

/**********/


void controllo_e_init(int argc, char** argv, RTYPE* seed, 
		      int* kmax, int* numw){

  if(argc!=4) {
    fprintf(stderr,"Usami in questo modo:\n");
    fprintf(stderr,"%s <seme generatore> <k> <numero camminatori>\n", argv[0]);
    fprintf(stderr,"il tempo massimo sarà 2^k. Inserire k>5\n");
    exit(-1);
  }
  *seed=atol(argv[1]); 
  *kmax=atoi(argv[2]); 
  *numw=atoi(argv[3]); 

  if ( (*kmax) <=5 ) {
    fprintf(stderr,"Usami in questo modo:\n%s <seme generatore> <k> <numero camminatori>\n", argv[0]);
    fprintf(stderr,"il tempo massimo sarà 2^k. Inserire k>5\n");
    exit(-2);    
  } 

  return;
}

double* alloca_double(int quanti) {

  double *ris;
  ris=(double *)calloc(quanti, sizeof(double));
  if (ris==NULL) {
    fprintf(stderr,"Allocazione fallita. Esco.");
    exit(-4);
  }
  return ris;

}

void una_passeggiata(RTYPE* seed, struct walker* w, int kmax, double *r2, double* r4) {
    
  int t, k, next;
  double reg=0;

  /*All'inizio siamo nell'origine e nessuna delle direzioni 0(x+),1(y+),2(x-),3(y-) è proibita*/
  w->x=0;
  w->y=0;
  w->direzioneProibita=-1; /*un numero qualunque diverso da 0,1,2,3*/
  t=0;
  /*primo traguardo a t=2, in generale next=2^(k+1)*/
  next=2;
  for(k=0;k<kmax;k++) {
    while(t<next) {
      un_passo(seed,w); t++;
    }
    /*ora misuriamo e aggiorniamo il prossimo traguardo, misuro a tempi potenze di 2*/
    reg=(w->x)*(w->x) + (w->y)*(w->y);
    r2[k]+=reg;
    r4[k]+=reg*reg;
    next*=2;
  }
  return;
}

void un_passo(RTYPE* seed, struct walker* w) {

  RTYPE mossaCasuale;

  int fatto=0;

  while (fatto==0) {
    (*seed)=(*seed)*MOLTIPLICATORE;
    mossaCasuale=(*seed)>>62;
    /*mossaCasuale può valere solo 0,1,2,3*/
    /*direzioneProibita contiene il valore di casuale che va scartato*/
    if (mossaCasuale != w->direzioneProibita) {
      if (mossaCasuale==0) w->x++; 
      else if (mossaCasuale==1) w->y++;
      else if (mossaCasuale==2) w->x--;
      else if (mossaCasuale==3) w->y--;
      else {
	fprintf(stderr,"numero casuale inaspettato. Termino.\n");
	exit(-3);
      }
      w->direzioneProibita=(mossaCasuale+2)%4;
      /*Con la convenzione adottata, la direzione proibita è 
	l'opposta di quella in cui mi sono appena mosso (casuale)*/
      fatto=1;
    }
    /*Se la mossa è valida, fatto=1 e si esce automaticamente dal ciclo*/
  }
  return;

}

void stampa_finale(int kmax, int numw, double* r2, double* r4) {

  int k, next;
  FILE* fp;

  fp=fopen("medie.out","w");
  /*Stampiamo i risultati solo da t=32 per eliminare il transiente*/
  fprintf(fp,"## t           r^2   err\n");
  next=32;
  for (k=4;k<kmax;k++) {
    r2[k]/=numw;
    r4[k]/=numw;
    r4[k]-=r2[k]*r2[k];
    r4[k]=sqrt(r4[k]/(numw-1));
    fprintf(fp,"%-10d %g %g\n", next, r2[k], r4[k]);
    next*=2;
  }

  fclose(fp);
}
