#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct dati {
  double* numeri; // usate questo array per salvare i numeri letti dal file
  double* prob;  // usate questo array per l'istogramma (un contatore puo' essere anche un array di double)
  double mini, maxi;
  int Nbin;
  int Nran;
};


void inizializza(struct dati*, char**);
void alloca_double(double**, int);
void leggi_numeri(struct dati*, char*);
void riempi_istogramma_e_normalizza(struct dati);
void stampa_prob(struct dati);

int main(int argc, char** argv) {

    struct dati X;

    if (argc!=4) {
	fprintf(stderr,"%s <quanti numeri> <quanti bin> <file>\n",argv[0]);
	exit(EXIT_FAILURE);
    }

    inizializza(&X,argv);

    leggi_numeri(&X,argv[3]);

    riempi_istogramma_e_normalizza(X);

    stampa_prob(X);

    free(X.numeri);
    free(X.prob);

    return EXIT_SUCCESS;
}

void inizializza(struct dati* X, char** argv) {

    X->Nran=atoi(argv[1]);
    X->Nbin=atoi(argv[2]);

    alloca_double(&(X->numeri),X->Nran);
    alloca_double(&(X->prob)  ,X->Nbin);

    return;
}

void alloca_double(double** punt, int quanti) {

    *punt=(double*)malloc(quanti*sizeof(double));
    if (NULL==*punt) {
	fprintf(stderr,"Errore di allocazione.\n");
	exit(EXIT_FAILURE);
    }

    return;

}

void leggi_numeri(struct dati* X, char* nomefile) {

    int i;
    FILE* finp;
    double reg;
    finp = fopen(nomefile, "r");

    for(i=0; i<(X->Nran); i++){
      fscanf(finp, "%lg", &reg);
      X->numeri[i] = reg;

      if(i==0){
	X->mini = X->numeri[i];
	X->maxi = X->numeri[i];
      }

      else if((X->numeri[i]) < (X->mini))
	X->mini = X->numeri[i];
      
      else if((X->numeri[i]) > (X->maxi))
	X->maxi = X->numeri[i];	
    }

    fclose(finp);
}


void riempi_istogramma_e_normalizza(struct dati X) {

    int i, index;
    double delta;
    double norma;

    delta = (X.maxi - X.mini) / (double) (X.Nbin-1);
    norma = delta * X.Nran;
    norma = 1/norma;

    for(index=0; index<(X.Nbin); index++)
      X.prob[index] = 0;
    
    for(i=0; i<(X.Nran); i++){
      index = (X.numeri[i] - X.mini) / delta + 0.5;
      X.prob[index]++;
    }

    for(index=0; index<(X.Nbin); index++)
      X.prob[index]*=norma;
}

void stampa_prob(struct dati X) {

    int i;
    double delta;

    delta=(X.maxi-X.mini)/(X.Nbin-1);
    for (i=0;i<X.Nbin;i++)
      fprintf(stdout,"%-12.6g %-.6g\n",X.mini+i*delta,X.prob[i]);
    return;

}
