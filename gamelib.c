#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "gamelib.h"

//Elenco variabili statiche
static struct Mago giocatore[2]; //vettore di maghi
static time_t t;
static int gioco_impostato=0; //per controllare se il gioco è impostato o no

//Elenco funzioni statiche
static void riempi_mazzo(struct Mago* mago, int p, int max);
static void visualizza_carte(struct Mago* mago, int i);
static char* nome_classe(int n);
static void pulisci();
static void pesca_carta(struct Mago* mag);
static void inserisci_mano(struct Mago* mago, struct Carta* carta_da_inserire);
static void stampa_mano(struct Mago* mago);
static void stampa_campo(struct Mago* mago);
static void gioca_carta(struct Mago* mago1, struct Mago* mago2);
static void attacca(struct Mago* mago1, struct Mago* mago2);
static void sposta_nel_campo(struct Mago* mago, int n);
static void rimuovi_creatura_dal_campo(struct Mago* mago1, struct Mago* mago2, int n);
static void infliggi_danno_campo_mago(struct Mago* mago1, struct Mago* mago2, int n);
static void guarisci_danno_campo_mago(struct Mago* mago1, int n);
static void attacca();
static int passa(int n);

void imposta_gioco(){
  //Dealloco tutta la memoria allocata in precedenza
  pulisci();

  srand((unsigned)time(&t));  //Genera numeri casuali in base al tempo attuale
  int nCarte=0; //Numero di carte che dovrò generare
  int classe=0; //variabile per leggere la classe del mago scelta
  int errore=0; //Variabile per segnare vari errori

  //creo i due maghi
  for(int i=0; i<2; i++){
    //imposto i punti vita dei giocatori
    giocatore[i].pv=20; //Imposto in punti vita dei maghi a 20
    printf(""green"+-------------------------------------+\n"reset_color);
    printf("Giocatore %d come ti chiami?\n",i+1);
    fgets(giocatore[i].nome,256,stdin);   //Imposto il nome del giocatore
    printf(""green"+-------------------------------------+\n"reset_color);
    do{ //faccio scegliere la classe finchè non immette un valore valido
      errore=0;
      printf("%s scegli una classe\n 1)tenebre\n 2)vita\n 3)luce \n",giocatore[i].nome);
      scanf("%d",&classe);
      while (getchar() != '\n');
      switch (classe) { //imposto la classe del mago
        case 1:
          giocatore[i].classe=tenebre;
          break;
        case 2:
          giocatore[i].classe=vita;
          break;
        case 3:
          giocatore[i].classe=luce;
          break;
        default:
          printf(red "Immetti un un input valido\n" reset_color);
          errore=1;
          break;
      }
    }while(errore!=0);
    giocatore[i].inizio_mazzo=NULL;
  }

  //Genero i 2 mazzi di carte e le mani
  printf(""green"+-------------------------------------+\n"reset_color);
  printf("Quante carte hanno i mazzi?\n");
  scanf("%d",&nCarte);
  nCarte=nCarte+5;  //aggiungo le carte che saranno messe nelle mani del giocatore
  for(int i=0;i<2;i++){ //creo e stampo i mazzi e le mani dei giocatori
      for(int j=nCarte; j>0; j--){
        riempi_mazzo(&giocatore[i], j, nCarte);   //creo il mazzo del giocatore
      }
      for(int j=0; j<5; j++){
        pesca_carta(&giocatore[i]); //creo la mano del giocatore
      }
    //stampo il mazzo
    printf(""green"+-------------------------------------+\n"reset_color);
    printf("|Giocatore %d:\n|Nome: %s\n|Classe: %s\n|Mazzo:\n",i+1,giocatore[i].nome, nome_classe(giocatore[i].classe));
    //stampo il mazzo
    visualizza_carte(&giocatore[i], i); //stampo il mazzo
    //stampo la mano
    stampa_mano(&giocatore[i]); //stampo la mano
  }
  gioco_impostato=1;  //il gioco è stato impostato, ora posso giocare, altrimenti non posso
}

void combatti(){
  int scelta=0; //variabile scelta del giocatore
  int fine_partita=1; //variabile per controllare se la partita è finita
  int count=0;  //Variabile per controllare che il giocatore posso pescare o attaccare o giocare una carta solo una volta
  int n= rand()%2;  //random per decidere chi inizia a giocare
  int giocatoreAvversario=0;
  if(gioco_impostato==1){ //controllo che il gioco sia impostato
    do{
      if(n==0){
        giocatoreAvversario=n+1;
      }else{
        giocatoreAvversario=n-1;
      }
      printf(""green"+-------------------------------------+\n"reset_color);
      printf("É il turno di "blue"%s"reset_color"Punti vita= "blue"%d\n"reset_color"Punti vita avversario="red"%d\n"reset_color,giocatore[n].nome ,giocatore[n].pv, giocatore[giocatoreAvversario].pv);
      printf("COSA VUOI FARE?\n 1)Pesca \n 2)Giocare carta \n 3)Attaccare \n 4)Visualizza mano \n 5)Visualizza campo \n 6)Visualizza campo avversario \n 7)Pulisci terminale \n 8)Salta turno\n");
      scanf("%d",&scelta );
      while (getchar() != '\n');
      switch (scelta) {
        case 1: //il giocatore ha scelto di pescare una carta
          if(count==0){ //controllo che non lo abbia già fatto
            pesca_carta(&giocatore[n]); //pesco la carta
            stampa_mano(&giocatore[n]); //stampo la carta
            count++;  //aumento count per impedire di rifare questa scelta o le altre due che si possono fare una sola volta
          }else{
            printf(red"Hai già giocato, passa il turno\n"reset_color); //se avevo gia fatto una dele tre azioni da errore
          }
          break;
        case 2: //il giocatore ha scelto di giocare una carta
          if(count==0){ //controllo che non lo abbia già fatto
            gioca_carta(&giocatore[n],&giocatore[giocatoreAvversario]);
            count++;
          }else{
            printf(red"Hai già giocato, passa il turno\n"reset_color);
          }
          break;
        case 3: //il giocatore ha scelto di attaccare
          if(count==0){
            int i=0;
            for (i = 0; i < 4; i++) {   //controllo se il campo è vuoto
              if(giocatore[n].campo[i]!=NULL){  //se almeno una posizione non è vuota procedo
                attacca(&giocatore[n],&giocatore[giocatoreAvversario]);
                count++;
                break;
              }
            }
            if(i==4){
              printf("Non hai carte sul campo da giocare\n");
            }
          }else{
            printf(red"Hai già giocato, passa il turno\n"reset_color);
          }
          break;
        case 4: //il giocatore ha scelto di visualizzare la sua mano
          stampa_mano(&giocatore[n]);
          break;
        case 5: //il giocatore ha scelto di visualizzare il suo campo
          stampa_campo(&giocatore[n]);
          break;
        case 6: //il giocatore ha scelto di visualizzare il campo avversario
          stampa_campo(&giocatore[giocatoreAvversario]);
          break;
        case 7:
          system("clear");
          break;
        case 8: //il giocatore ha scelto di passare il turno
          n=passa(n);
          count=0;
          break;
        default:printf(red "Immetti un un input valido. Gli input validi sono: 1, 2, 3, 4, 5, 6, 7, 8\n"reset_color);
      }

      //se le carte nel mazzo dell'attuale giocatore sono finite la partita finirà quando passerà il turno
      if(giocatore[n].inizio_mazzo == NULL){
        printf("Carte nel mazzo finite, FINE PARTITA\n");
        fine_partita=0;
      }

      //controlo se il giocatore avversario abbia finito i punti vita, in caso il gioco finirà
      if(giocatore[giocatoreAvversario].pv<=0){
        printf("Giocatore %s ha 0 Punti Vita, FINE PARTITA\n",giocatore[giocatoreAvversario].nome);
        fine_partita=0;
      }

    }while(fine_partita!=0); //gioco finchè uno dei due finisce i punti viti o quando finisce uno dei due mazzi

    if(giocatore[0].pv>giocatore[1].pv){
      int nameLenght=strlen(giocatore[0].nome)-1;
      printf("+--------------------------+\n");
      printf(" %.*s  HA VINTO \n",nameLenght,giocatore[0].nome);
      printf("+--------------------------+\n");
    }else{
      int nameLenght=strlen(giocatore[1].nome)-1;
      printf("+--------------------------+\n");
      printf(" %.*s  HA VINTO \n",nameLenght,giocatore[1].nome);
      printf("+--------------------------+\n");
    }
    gioco_impostato=0;
  }else{
    printf(red"Errore, devi impostare il gioco prima\n"reset_color);
  }
}

void ComeSiGioca(){
  printf(""green"+-------------------------------------+\n"reset_color);
  printf("|            COME SI GIOCA            |\n");
  printf(""green"+-------------------------------------+\n"reset_color);
  printf("-Impostare il gioco\n");
  printf("-Una volta impostato il gioco premere combatti per iniziare a giocare\n");
  printf("-Una volta iniziato a giocare avrai diverse scelte, le prime tre potrai farle solo una volta a turno:\n");
  printf("  -Pesca: peschi una carta dal mazzo, se hai la mano piena la carta pescata viene distrutta\n");
  printf("  -Gioca carta: puoi giocare una carta dal mazzo, in base al tipo di carta puoi fare diverse cose:\n");
  printf("    -Creatura: puoi posizionare la carta sul campo\n");
  printf("    -Rimuovi Creatura: puoi rimuovere una ditruggere una creatura avversaria se presente nel campo\n");
  printf("    -Infliggi Danno: puoi infliggere del danno ad una creatura avversaria sul campo o al mago\n");
  printf("    -Guarisci Danno: puoi aumentare i PV di una creatura sul campo o a te (te inteso il mago)\n");
  printf("  -Attacca: attacchi l'avversario utilizzando una creatura sul campo\n");
  printf("  -Visualizza Mano: visualizzi le carte che hai nella tua mano\n");
  printf("  -Visualizza Campo: visualizzi il tuo campo\n");
  printf("  -Visualizza Campo Avversario: visualizzi il campo avversario\n");
  printf("  -Pulisci Terminale: cancella tutte le scritte nel terminale così da poter capire meglio quello che si sta facendo\n");
  printf("  -Passa: passi il turno all'avversario\n");
  printf("\nIl gioco finisce quando uno dei due maghi ha 0 PV, o quando uno dei due finisce le carte del mazzo \n");
  printf("Vince il giocatore con i PV più alti\n");
  printf("\nPremi invio per tornare al menù\n");
  while (getchar() != '\n');
}

void crediti(){
  printf(""green"+-------------------------------------+\n"reset_color);
  printf(""green"|"reset_color"               CREDITI               "green"|\n");
  printf(""green"+-------------------------------------+\n"reset_color);
  printf("PROJECT MANAGER: Francesco Santini\n");
  printf("GAME DESIGNER: Francesco Santini\n");
  printf("LEAD PROGRAMMER: Filippo Notari\n");
  printf("PROGRAMMER: Filippo Notari\n");
  printf("ENGINE PROGRAMMER: Filippo Notari\n");
  printf("MULTIPLAYER PROGRAMMER: Filippo Notari\n");
  printf("LEAD TESTER: Filippo Notari\n");
  printf("GAME TESTER: Filippo Notari\n");
  printf("\nPremi invio per tornare al menù\n");
  while (getchar() != '\n');
}

void termina_gioco(){
  pulisci();
  printf("+---------+\n");
  printf("| GOODBYE |\n");
  printf("+---------+\n");
  sleep(3);
  system("clear");
}

//attacco il mago o creatura avversaria con una creatura
static void attacca(struct Mago* mago1, struct Mago* mago2){
  int scelta_creatura_attaccante=0;
  int scelta_creatura=0;
  int scelta_bersaglio=0;
  int errore=0;
  do{
    errore=0;
    stampa_campo(mago1);
    printf("Con quale creatura vuoi attaccare?\n");
    scanf("%d",&scelta_creatura_attaccante);
    if(mago1->campo[scelta_creatura_attaccante]!=NULL){
      printf("Vuoi attacare il campo o il mago?\n 1)Mago   2)Campo\n");
      scanf("%d",&scelta_bersaglio);
      if(scelta_bersaglio==1){
        int danni_inflitti=(mago1->campo[scelta_creatura_attaccante])->punti_vita;
        mago2->pv=mago2->pv-danni_inflitti;
        free(mago1->campo[scelta_creatura_attaccante]);
        mago1->campo[scelta_creatura_attaccante]=NULL;
        if(mago2->pv>0){
          printf("Hai fatto %d danni al mago avversario\n", danni_inflitti);
        }else{
          printf("Hai ucciso il mago avversario\n");
        }

      }else{
        if(scelta_bersaglio==2){
          stampa_campo(mago2);
          printf("Quale creatura vuoi attaccare?\n");
          scanf("%d",&scelta_creatura);
          if(mago2->campo[scelta_creatura]!=NULL){
            mago2->campo[scelta_creatura]->punti_vita=(mago2->campo[scelta_creatura]->punti_vita)-(mago1->campo[scelta_creatura_attaccante])->punti_vita;
            if(mago2->campo[scelta_creatura]->punti_vita<=0){
              free(mago2->campo[scelta_creatura]);
              mago2->campo[scelta_creatura]=NULL;
              free(mago1->campo[scelta_creatura_attaccante]);
              mago1->campo[scelta_creatura_attaccante]=NULL;
              printf("Hai ucciso la creatura\n");
            }else{
              printf("Hai fatto %d danni alla craetura avversaria\n", (mago1->campo[scelta_creatura_attaccante])->punti_vita);
              mago1->campo[scelta_creatura_attaccante]=NULL;
              free(mago1->campo[scelta_creatura_attaccante]);
            }
          }else{
            printf(red"Errore, scegli una creatura\n"reset_color);
            errore=1;
          }
        }
      }
    }else{
      printf(red"Errore, scegli una creatura\n" reset_color);
      errore=1;
    }
  }while(errore!=0);
}

//Pesco una carta dal mazzo
static void pesca_carta(struct Mago* mago){
  struct Carta* pTemp= mago->inizio_mazzo;
  if(pTemp==NULL){
    printf("Il mazzo è vuoto\n");
  }else{
    if(pTemp->pNext == NULL){ //pesco la l'ultima (la prima) carta presente nel mazzo
      printf("Ultima carta nel mazzo\n");
      inserisci_mano(mago, pTemp);
      mago->inizio_mazzo=NULL;
    }else{
      while((pTemp->pNext)->pNext!= NULL) //trovo la fine del mazzo
        pTemp= pTemp->pNext;
      inserisci_mano(mago, pTemp->pNext);
      pTemp->pNext = NULL;
    }
  }

}

//Gioco una carta
static void gioca_carta(struct Mago* mago1, struct Mago* mago2){
  int scelta=0;
  int errore=0;
  do{
    stampa_mano(mago1);
    printf("Quale carta vuoi giocare? \n");
    scanf("%d",&scelta);
    while (getchar() != '\n');
    errore=0;
    if(mago1->mano[scelta]!=NULL){
      switch ((mago1->mano[scelta])->tipo) {  //in base a quale carta voglio giocare farò diverse cose
        case creatura:
          sposta_nel_campo(mago1,scelta);
          break;
        case rimuovi_creatura:
          rimuovi_creatura_dal_campo(mago1,mago2,scelta);
          break;
        case infliggi_danno:
          infliggi_danno_campo_mago(mago1,mago2,scelta);
          break;
        case guarisci_danno:
          guarisci_danno_campo_mago(mago1,scelta);
          break;
        default:
          printf(red "Immetti una posizione valida\n" reset_color);
          errore=1;
          break;
        }
      }else{
        printf("Non hai nessuna carta in questa posizione\n");
        errore=1;
      }

  }while(errore!=0);
}

//Aggiungo vita ad una cretura sul campo o al mago
static void guarisci_danno_campo_mago(struct Mago* mago1, int n){
  int scelta=0;
  int scelta_creatura=0;
  int errore=0;
  do{
    errore=0;
    printf("Vuoi curare il campo o il mago?\n 1)Mago   2)Campo\n"); //sceglo chi curare
    scanf("%d",&scelta );
    if(scelta==1){  //scelgo di curare il mago
      int danni_guariti=(mago1->mano[n])->punti_vita;
      mago1->pv=mago1->pv+danni_guariti;  //aumento i pv del mago aggiungendo quelli della carta usata
      free(mago1->mano[n]);   //dealloco la carta usata
      mago1->mano[n]=NULL;
      printf("Hai aggiunto %d vita al mago\n", danni_guariti);
    }else{
      if(scelta==2){  //scelgo di curare una carta nel campo
        stampa_campo(mago1);
        printf("Quale creatura vuoi guarire?\n");
        scanf("%d",&scelta_creatura);
        if(mago1->campo[scelta_creatura]!=NULL){  //controllo che abbia scelto una creatura
          mago1->campo[scelta_creatura]->punti_vita=(mago1->campo[scelta_creatura]->punti_vita)+(mago1->mano[n])->punti_vita;
          printf("Hai aggiunto %d punti vita alla craetura\n", (mago1->mano[n])->punti_vita);
          mago1->mano[n]=NULL;
          free(mago1->mano[n]);
          }
        }else{
          printf(red "Errore, scegli una creatura\n" reset_color);
          errore=1;
        }
      }
  }while(errore!=0);
}

//infliggo danni ad una creatura sul campo o al mago evversario
static void infliggi_danno_campo_mago(struct Mago* mago1, struct Mago* mago2, int n){
  int scelta=0; //variabile per la scelta mago o campo
  int scelta_creatura=0;  //variabile scelta creatura da attaccare
  int errore=0;
  do{
    errore=0;
    printf("Vuoi attacare ilcredit campo o il mago?\n 1)Mago   2)Campo\n");
    scanf("%d",&scelta );
    if(scelta==1){  //scelgo il mago
      int danni_inflitti=(mago1->mano[n])->punti_vita;
      mago2->pv=mago2->pv-danni_inflitti; //imposto i punti vita del mago avversario rimuvendo quelli della carta utilizzata
      free(mago1->mano[n]); //dealloco la carta giocata
      mago1->mano[n]=NULL;  //imposto il vettore dove era la carta a null
      if(mago2->pv>0){  //se i punti vita del mago sono maggiori 0 dico quanti  danni ho fatto, senno dico che il mago avversario è morto
        printf("Hai fatto %d danni al mago avversario\n", danni_inflitti);
      }else{
        printf("Hai ucciso il mago avversario\n");
      }

    }else{
      if(scelta==2){  //scelgo una creatura
        stampa_campo(mago2);
        printf("Quale creatura vuoi attaccare?\n");
        scanf("%d",&scelta_creatura);
        if(mago2->campo[scelta_creatura]!=NULL){ //controllo che la posizione non sia vuota
          mago2->campo[scelta_creatura]->punti_vita=(mago2->campo[scelta_creatura]->punti_vita)-(mago1->mano[n])->punti_vita; // rimuovo i punti vita
          if(mago2->campo[scelta_creatura]->punti_vita<=0){ //se i punti vita della creatura sono minori di 0 la dealloco
            free(mago2->campo[scelta_creatura]);  //dealloco la carta attaccata
            mago2->campo[scelta_creatura]=NULL; //imposto a null il posto nel campo
            free(mago1->mano[n]); //dealloco la carta giocata
            mago1->mano[n]=NULL;   //imposto a null la posizione dove era la carta
            printf("Hai ucciso la creatura\n");
          }else{
            printf("Hai fatto %d danni alla craetura avversaria\n", (mago1->mano[n])->punti_vita);
            mago1->mano[n]=NULL;
            free(mago1->mano[n]);
          }
        }else{
          printf(""red "Errore, scegli una creatura\n" reset_color);
          errore=1;
        }
      }
    }

  }while(errore!=0);
}

//rimuovo una cratura dal banco
static void rimuovi_creatura_dal_campo(struct Mago* mago1, struct Mago* mago2, int n){
  int scelta=0;
  int errore=0;
  stampa_campo(mago2);  //stampo il campo avversario
  do{
    errore=0;
    printf("Quale creatura vuoi rimuovere?\n");
    scanf("%d",&scelta );

    if((mago2->campo[scelta])->tipo==creatura){ //controllo se la carta scelta è una creatura
      free(mago1->mano[n]); //delloco la carta giocata
      mago1->mano[n]=NULL;  //imposto il vettore della mano dove era la carta a null
      free(mago2->campo[scelta]); //dealloco la carta avversaria rimossa
      mago2->campo[scelta]=NULL;  //imposto il vettore campo dove era la carta a null
      printf("Carte distrutte\n");
      stampa_campo(mago2);  //stampo il nuovo campo
    }else{
      printf(""red "Errore, scegli una creatura\n" reset_color);
      errore=1;
    }
  }while(errore!=0);
}

//sposto la carta dalla mano al campo
static void sposta_nel_campo(struct Mago* mago, int n){
  int i=0;
  for (i = 0; i < 4; i++) { //trovo il primo posto libero sul campo
    if(mago->campo[i]==NULL){
      mago->campo[i]=mago->mano[n]; //se c'è spazio posiziono la carta
      mago->mano[n]=NULL; //rimuovo la carta dalla mano
      stampa_campo(mago); //stampo il campo
      break;
    }
  }
  if(i==4){ //se il campo è pieno stampo il campo
    stampa_campo(mago);
    printf(""red "Campo pieno, non posso giocare la carta\n" reset_color);
  }
}

//stampo il campo
static void stampa_campo(struct Mago* mago){
  printf(""green"+-------------------------------------+\n"reset_color);
  printf("|Campo:\n");
  for(int j=0; j<4;j++){
    if(mago->campo[j] == NULL) // posizione campo vuota
    {
      printf("| %d) Spazio libero\n",j);
    }
    else
    {
    // stampo tutte le carte nel campo
      struct Carta* pScan = mago->campo[j];
        switch (pScan->tipo) {
          case 0:
            printf("| %d) " yellow "Tipo: "blue"Creatura"yellow" Punti vita:"blue" %d\n"reset_color, j,pScan->punti_vita);
            break;
          case 1:
            printf("| %d) " yellow "Tipo: "blue"Rimuovi_creatura" yellow " Punti vita:"blue" %d\n"reset_color, j,pScan->punti_vita);
            break;
          case 2:
            printf("| %d) " yellow "Tipo: "blue"Infliggi_danno" yellow " Punti vita:"blue" %d\n"reset_color, j,pScan->punti_vita);
            break;
          case 3:
            printf("| %d) " yellow "Tipo: "blue"Guarisci_danno" yellow " Punti vita:"blue" %d\n"reset_color, j,pScan->punti_vita);
            break;
        }
    }
  }
  printf(""green"+-------------------------------------+\n"reset_color);
}

//Funzione passa
static int passa(int n){
  if(n==0){
    n=1;
  }else{
    n=0;
  }
  return n;
}

//Aggiungo la carta pescata alla mano
static void inserisci_mano(struct Mago* mago, struct Carta* carta_da_inserire){
  int i=0;
  for (i = 0; i < 6; i++) { //aggiungo la carta pescato nel primo posto vuoto della mano
    if(mago->mano[i]==NULL){
      mago->mano[i]=carta_da_inserire;
      break;
    }
  }
  if(i==6){ //se non c'è posto distruggo la carta
    printf(""red "Mano piena, distruggo la carta\n"reset_color);
    free(carta_da_inserire);
  }
}

//funzione aggiungi alla coda del mazzo
static void riempi_mazzo(struct Mago* mago, int p, int max){
  // Cre una nuova carta
  struct Carta *pNew = (struct Carta*) malloc(sizeof(struct Carta));

  //imposto casualmente i tipi di carta con percentuali 40%,25%,20%,15%
  int random=rand()%100 +1;
  if(random<=40){
    pNew->tipo=creatura;
  }else{
    if(random>40 && random<=65){
      pNew->tipo=rimuovi_creatura;
    }else{
      if(random>65 && random<=85){
        pNew->tipo=infliggi_danno;
      }else{
        pNew->tipo=guarisci_danno;
      }
    }
  }

  //imposto i punti vita in base alla posizione e tipo carta
  random = rand()%(10)+10;  //prendo una valore random tra 10 e 19
  double punti_vita =(double)(7.0/max)*p+(random/10.0); //con un complesso sitema di specchi e leve faccio funzionare il calcolo per impostare i punti vita random da 1 a 8 ma che aumentano in base alla posizione nel mazzo

  if(mago->classe==vita && pNew->tipo==creatura){ //in base alla classe del mago aumento la vita di alcuni tipi di carte
    pNew->punti_vita=punti_vita+(punti_vita/2);
  }else{
    if(mago->classe==tenebre && pNew->tipo==infliggi_danno){
      pNew->punti_vita=punti_vita*2;
    }else{
      if(mago->classe==luce && pNew->tipo==guarisci_danno){
        pNew->punti_vita=(punti_vita*3)+(punti_vita/2);
      }else{
        pNew->punti_vita=punti_vita;
        }
      }
    }

  pNew->pNext= NULL;  //imposto la carta successiva a null
  if(mago->inizio_mazzo == NULL){ //Nessuna carta nel mazzo
    mago->inizio_mazzo = pNew; // La prima carta è quella appena crata
  }else{
    struct Carta* pTemp=mago->inizio_mazzo;
    while(pTemp->pNext != NULL){  //trovo l'ultima carta
      pTemp= pTemp->pNext;
    }
    pTemp->pNext=pNew;
  }
  return;
}

//Funzione stampo il mazzo
static void visualizza_carte(struct Mago* mago, int i){
  if(mago->inizio_mazzo == NULL) // Mazzo vuoto
  {
    printf("Il mazzo è vuoto");
  }else{
  // stampo tutte le carte del mazzo
    struct Carta* pScan = mago->inizio_mazzo;
    do{
      switch (pScan->tipo) {
        case 0:
          printf("| " yellow "Tipo: "blue"Creatura"yellow" Punti vita:"blue" %d\n"reset_color,pScan->punti_vita);
          break;
        case 1:
          printf("| " yellow "Tipo: "blue"Rimuovi_creatura" yellow " Punti vita:"blue" %d\n"reset_color,pScan->punti_vita);
          break;
        case 2:
          printf("| " yellow "Tipo: "blue"Infliggi_danno" yellow " Punti vita:"blue" %d\n"reset_color,pScan->punti_vita);
          break;
        case 3:
          printf("| " yellow "Tipo: "blue"Guarisci_danno" yellow " Punti vita:"blue" %d\n"reset_color,pScan->punti_vita);
          break;
      }
      // vado alla carta successiva
      pScan = pScan->pNext;
    }while(pScan!= NULL); //ultima carta
  }
  return;
}

//Stampo la mano
static void stampa_mano(struct Mago* mago){
  printf(""green"+-------------------------------------+\n"reset_color);
  printf("|Mano:\n");
  for(int j=0; j<6;j++){
    if(mago->mano[j] == NULL) // posizione vuota
    {
      printf("| %d) Spazio libero\n",j);
    }else{
    // stampo le tutte carte della mano
      struct Carta* pScan = mago->mano[j];
        switch (pScan->tipo) {
          case 0:
            printf("| %d) " yellow "Tipo: "blue"Creatura"yellow" Punti vita:"blue" %d\n"reset_color, j,pScan->punti_vita);
            break;
          case 1:
            printf("| %d) " yellow "Tipo: "blue"Rimuovi_creatura" yellow " Punti vita:"blue" %d\n"reset_color, j,pScan->punti_vita);
            break;
          case 2:
            printf("| %d) " yellow "Tipo: "blue"Infliggi_danno" yellow " Punti vita:"blue" %d\n"reset_color, j,pScan->punti_vita);
            break;
          case 3:
            printf("| %d) " yellow "Tipo: "blue"Guarisci_danno" yellow " Punti vita:"blue" %d\n"reset_color, j,pScan->punti_vita);
            break;
        }
    }
  }
  printf(""green"+-------------------------------------+\n"reset_color);
}

//libero la memoria
static void pulisci(){
  for(int i=0; i<2;i++){
    struct Carta* ptemp;
    while(giocatore[i].inizio_mazzo != NULL){ //pulisco il mazzo del giocatore rimuovendo tutte le carte
      ptemp=giocatore[i].inizio_mazzo;
      giocatore[i].inizio_mazzo=giocatore[i].inizio_mazzo->pNext;
      free(ptemp);
    }
    for(int j=0; j<6; j++){ //pulisco la mano del giocatore svuotando tutto il vettore
      free(giocatore[i].mano[j]);
      giocatore[i].mano[j]=NULL;
    }
    for(int j=0; j<4; j++){ //pulisco il campo del giocatore svuotando tutto il vettore
      free(giocatore[i].campo[j]);
      giocatore[i].campo[j]=NULL;
    }
  }
}

//converto il valore della classe
static char* nome_classe(int n){
  switch (n) {
    case 0:return "Tenebre";
    case 1: return "Vita";
    case 2: return "Luce";
    default: return "Errore, classe non trovata";
  }
}
