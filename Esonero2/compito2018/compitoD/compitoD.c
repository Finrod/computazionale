#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*questo è 2^31 e serve a normalizzare i numeri random*/
#define NORMA (0x80000000)

struct dati {

  int numr; /*quantità di numeri casuali da generare*/
  double *lista; /*elenco di numeri casuali generati*/
  double *h; /*istogramma*/

};

void    stampa_tutto     (struct dati* esp1, struct dati* esp2, 
			  double min, double max, int nbins);
void    istogramma       (struct dati* esp, double min, 
			  double max, int nbins);
void    cerca_estremi    (struct dati* esp1, struct dati* esp2, 
			  double* min, double* max);
double  box_muller       (void);
double  gaussiano        (void);
double* alloca_double    (int quanti);
void    controlla_e_init (int argc, char**argv, 
			  struct dati* esp1, struct dati* esp2, int* nbins);

int main(int argc, char** argv) {

  int i;

  double min, max;

  int nbins;

  struct dati esperimento1, esperimento2;

  controlla_e_init(argc, argv, &esperimento1, &esperimento2, &nbins);


  for(i=0;i<esperimento1.numr;i++) {
    esperimento1.lista[i]=box_muller();

  }

  for(i=0;i<esperimento2.numr;i++) {
    esperimento2.lista[i]=gaussiano();
  }

  cerca_estremi(&esperimento1,&esperimento2,&min,&max);

  istogramma(&esperimento1,min,max,nbins);
  istogramma(&esperimento2,min,max,nbins);

  stampa_tutto(&esperimento1,&esperimento2,min,max,nbins);

  return 0;

}

void controlla_e_init(int argc, char**argv, 
		      struct dati* esp1, struct dati* esp2, int* nbins) {

  int numr;
  unsigned long long int seed;

  if(argc!=4) {
    fprintf(stderr,"Usami in questo modo:\n");
    fprintf(stderr,"%s <seme generatore> <quanti numeri> <nbins>\n", argv[0]);
    exit(-1);
  }

  seed=atol(argv[1]);/******ho messo atol()*******/
  srand48(seed);

  numr=atoi(argv[2]); 
  *nbins=atoi(argv[3]); 

  if(numr<=0 || *nbins <=0) {
    fprintf(stderr,"Usami in questo modo:\n");
    fprintf(stderr,"%s <seme generatore> <quanti numeri> <nbins>\n", argv[0]);
    fprintf(stderr,"<quanti numeri> e <nbins> devono essere interi positivi\n");
    exit(-2);
  }

  esp1->numr=esp2->numr=numr;

  esp1->lista = alloca_double(numr);
  esp2->lista = alloca_double(numr);
  esp1->h     = alloca_double(*nbins);/*******ho messo l'asterisco*********/
  esp2->h     = alloca_double(*nbins);/*******ho messo l'asterisco*********/

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

double gaussiano() {

#define N_AUX (1000)
  int i;
  double r;
  double ris=0;

  for(i=0; i<N_AUX; i++){
    r=(double)lrand48()/ NORMA;
    if(r<0.5)
      ris++;
    else
      ris--;
  }
  ris/=sqrt(N_AUX);
  return ris;

}

double box_muller(){

  double r1,r2;
  double ris=0;

  r1=(double)lrand48() / NORMA;/*******ho messo /NORMA******/
  r2=(double)lrand48() / NORMA;/*******ho messo /NORMA******/

  ris=sqrt(-2.*log(r1))*cos(2.*M_PI*r2);

  return ris;

}

void cerca_estremi(struct dati* esp1, struct dati* esp2, double* min, double* max) {

  int i;
  *min = *max = esp1->lista[0];
  
  for(i=0; i<esp1->numr; i++){
    if(esp1->lista[i]<*min)
      *min = esp1->lista[i];
    if(esp2->lista[i]<*min)
      *min = esp2->lista[i];
    if(esp1->lista[i]>*max)
      *max = esp1->lista[i];
    if(esp2->lista[i]>*max)
      *max = esp2->lista[i];
  }
    
  return;

}

void istogramma(struct dati* esp, double min, double max, int nbins) {

  int i;
  double reg ,norma, binsize;
  int indice;

  binsize=(max-min)/(nbins-1);
  norma=esp->numr * binsize;/******ho aggiunto binsize******/

  for(i=0;i<esp->numr;i++) {
    reg=esp->lista[i];
    /*in questo modo indice*binsize è già il centro del bin*/
    indice=(int)(0.5+(reg-min)/binsize);
    esp->h[indice]++;
  }

  for (i=0;i<nbins;i++) {
    esp->h[i]/=norma;
  }

  return;

}

void stampa_tutto(struct dati* esp1, struct dati* esp2, double min, double max, int nbins) {

  int i;
  double binsize=(max-min)/(nbins-1);
  double reg;
  for(i=0;i<nbins;i++) {
    reg=min+i*binsize;
    fprintf(stdout,"%g %g %g\n",reg,esp1->h[i],esp2->h[i]);/******ho messo ->********/
  }
  return;
  
}
