// Dichiarazione delle funzioni da chiamare in main.c
// Definizione dei tipi necessari alla libreria

//colori per il terminale
#define red "\x1b[31m"
#define yellow "\x1b[33m"
#define blue "\x1b[34m"
#define green "\x1b[32m"
#define reset_color "\x1b[0m"


//funzioni di gioco
void imposta_gioco();
void combatti();
void ComeSiGioca();
void crediti();
void termina_gioco();

//enum
enum Classe_mago{ tenebre, vita, luce};

enum Tipo_carta{ creatura,rimuovi_creatura, infliggi_danno, guarisci_danno};

//struct
struct Mago{
  char nome[25];
  int pv;
  enum Classe_mago classe;
  struct Carta* inizio_mazzo;
  struct Carta* mano[6];
  struct Carta* campo[4];
};

struct Carta{
  enum Tipo_carta tipo;
  int punti_vita;
  struct Carta* pNext;
};
