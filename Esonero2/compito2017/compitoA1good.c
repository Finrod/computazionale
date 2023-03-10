#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void riempi_elenco(double*, int);
void stampa_numeri(double*, int);

int main(int argc, char** argv) {

    unsigned int seed;
    int Nran;
    double* numeri;

    if (argc!=3) {
	fprintf(stderr,"%s <seed> <quanti numeri>\n",argv[0]);
	exit(EXIT_FAILURE);
    }
    seed=atoi(argv[1]); // atoi() trasforma una stringa nel valore intero rappresentato
    Nran=atoi(argv[2]);

    srand(seed);

    numeri = (double*)calloc(Nran, sizeof(double));
    if (numeri == NULL) {
      fprintf(stderr,"Errore di allocazione.\n");
      exit(EXIT_FAILURE);
    }
    riempi_elenco(numeri,Nran);

    stampa_numeri(numeri,Nran);
    free(numeri);

    return EXIT_SUCCESS;
}

void riempi_elenco(double* punt, int quanti) {
  int i;
  double r;
  double f;
  for (i=0;i<quanti;i++) {
    r=1.-rand()/((double)RAND_MAX+1);
    f=rand()/((double)RAND_MAX+1);
    punt[i]=sqrt(-2.*log(r))*cos(2.*M_PI*f);
  }
}

void stampa_numeri(double* punt, int quanti) {
  int i;
  double p;
  for(i=0;i<quanti;i++) {
    p = punt[i];
    fprintf(stdout,"%.14e\n",p);
  }
}
