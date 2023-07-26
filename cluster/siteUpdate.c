/************************************UPDATE***********************************/

int siteUpdate(int posto, int **vicini, int direzione){
  
  int postoVicino = vicini[posto][direzione];
  int cambiamento = 0;                                     //mi dice se c'è stato update
  struct sito *testaSup, *codaSup;
  struct sito *testaInf, *codaInf;
  struct sito *aux;
  int sitoSup, sitoInf;
  
  if( (rete[posto].spin == rete[postoVicino].spin) &&      //controllo se c'è bisogno di fare l'update
       (rete[posto].padre != rete[postoVicino].padre) ){
    
    cambiamento = 1;
    
    if(rete[posto].padre < rete[postoVicino].padre){       //trovo chi è il "cluster" padre e chi è il "cluster" figlio
      /* testaSup = rete[postoVicino].padre;
      testaInf = rete[posto].padre;*/
      sitoSup = postoVicino;
      sitoInf = posto;
    }
    else{
      /*testaSup = rete[posto].padre;
	testaInf = rete[postoVicino].padre;*/
      sitoInf = postoVicino;
      sitoSup = posto;
    }

    /*if(rete[posto].figlio < rete[postoVicino].figlio){
      codaSup = rete[postoVicino].figlio;
      codaInf = rete[posto].figlio;
    }
    else{
      codaSup = rete[posto].figlio;
      codaInf = rete[postoVicino].figlio;
      }*/

    testaSup = rete[sitoSup].padre;
    testaInf = rete[sitoInf].padre;
    codaSup = rete[sitoSup].figlio;
    codaInf = rete[sitoInf].figlio;

    
    

    aux = testaSup;                                     //assegno il nuovo figlio a tutti i siti del cluster padre
    aux->padre = testaInf;
    while(aux != aux->next) {
      aux = aux->next;
      aux->padre = testaInf;
    }

    /*   aux = testaInf;
    aux->figlio = codaSup;
    while(aux != aux->next){                             //assegno il nuovo padre a tutti i siti del cluster figlio
      aux = aux->next;
      aux->figlio = codaSup;      
      }*/
    
    codaInf->next = testaSup;
    testaSup->former = codaInf;                         //collego tra loro i due padri e i due figli
    

  }
    
  return cambiamento;
}
