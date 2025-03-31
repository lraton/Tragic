#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"

int main() {
  int risposta = 0;
  printf(""red "Avvia a schermo intero per visualizzare meglio...\n");
  while (getchar() != '\n');  // aspetto che preme invio così da scandire le operazioni
  //pulisco il terminale
  system("clear");
  do{
    //Stampo l'ascii art solo quando apro il gioco la prima volta, dopo che ho impostato il gioco e quando finisce una partita
    if(risposta==0 || risposta==1 || risposta==2){
      //Sperando che il compilatore non distrugga tutto, in caso questi sotto dovrebbero essere 2 maghi e la scritta tragic
      printf(""reset_color"                    ____                                                                        ____                 \n");
      printf("                  .'* *.'                                                                     .'* *.'                \n");
      printf("               __/_*_*(_                                                                   __/_*_*(_                 \n");
      printf("              / _______ \\                                                                 / _______ \\              \n");
      printf("             _\\_)/___\\(_/_                                                               _\\_)/___\\(_/_           \n");
      printf("            / _((\\- -/))_ \\                                                             / _((\\- -/))_ \\          \n");
      printf("            \\ \\())(-)(()/ /      ▄▄▄█████▓ ██▀███  ▄▄▄        ▄████  ██▓ ▄████▄         \\ \\())(-)(()/ /          \n");
      printf("             ' \\(((()))/ '       ▓  ██▒ ▓▒▓██ ▒ ██▒████▄     ██▒ ▀█▒▓██▒▒██▀ ▀█          ' \\(((()))/ '            \n");
      printf("            / ' \\)).))/ ' \\      ▒ ▓██░ ▒░▓██ ░▄█ ▒██  ▀█▄  ▒██░▄▄▄░▒██▒▒▓█    ▄        / ' \\)).))/ ' \\          \n");
      printf("           / _ \\ - | - /_  \\     ░ ▓██▓ ░ ▒██▀▀█▄ ░██▄▄▄▄██ ░▓█  ██▓░██░▒▓▓▄ ▄██▒      / _ \\ - | - /_  \\         \n");
      printf("          (   ( .;''';. .'  )      ▒██▒ ░ ░██▓ ▒██▒▓█   ▓██▒░▒▓███▀▒░██░▒ ▓███▀ ░     (   ( .;''';. .'  )            \n");
      printf("          _\\ __ /    )\\ __ /_       ▒ ░░   ░ ▒▓ ░▒▓░▒▒   ▓▒█░ ░▒   ▒ ░▓  ░ ░▒ ▒  ░    _\\ __ /    )\\ __ /_         \n");
      printf("            \\/  \\   ' /  \\/          ░      ░▒ ░ ▒░ ▒   ▒▒ ░  ░   ░  ▒ ░  ░  ▒          \\/  \\   ' /  \\/        \n");
      printf("             .'  '...' ' )         ░        ░░   ░  ░   ▒   ░ ░   ░  ▒ ░░                .'  '...' ' )               \n");
      printf("              / /  |  \\ \\                 ░          ░  ░      ░  ░  ░ ░                 / /  |  \\ \\            \n");
      printf("             / .   .   . \\                                          ░                   / .   .   . \\            \n");
      printf("            /   .     .   \\                       Un gioco di carte                    /   .     .   \\           \n");
      printf("           /   /   |   \\   \\                                                          /   /   |   \\   \\         \n");
      printf("         .'   /    b    '.  '.                                                      .'   /    b    '.  '.         \n");
      printf("     _.-'    /     Bb     '-. '-._                                              _.-'    /     Bb     '-. '-._     \n");
      printf(" _.-'       |      BBb       '-.  '-.                                       _.-'       |      BBb       '-.  '-.  \n");
      printf("(________mrf\\____.dBBBb.________)____)                                     (________mrf\\____.dBBBb.________)____)\n");
    }

    if(risposta==1 || risposta==2 || risposta==3 || risposta==4){
      printf(""green"+-------------------------------------+\n");
      printf("| "reset_color"MAGO BENTORNATO NEL MENU PRINCIPALE"green" |\n");
      printf("+-------------------------------------+"reset_color);
    }else{
      while (getchar() != '\n');  // aspetto che preme invio così da scandire le operazioni
      printf(""green"+------------------------------------+\n");
      printf("| "reset_color"MAGO BENVENUTO NEL MENU PRINCIPALE "green"|\n");
      printf("+------------------------------------+"reset_color);
    }
    printf("\nINSERISCI UN COMANDO: \n 1) Imposta gioco \n 2) Combatti \n 3) Come si gioca \n 4) Crediti \n 5) Termina gioco \n");
    scanf("%d",&risposta);
    while (getchar() != '\n');
    switch (risposta) {
      case 1:
        imposta_gioco();
        break;
      case 2:
        combatti();
        break;
      case 3:
        ComeSiGioca();
        break;
      case 4:
        crediti();
        break;
      case 5:
        termina_gioco();
        break;
      default: printf(""red "Immetti un un input valido. Gli input validi sono: 1, 2, 3\n" reset_color);
    }
  }while(risposta!=5);
}
