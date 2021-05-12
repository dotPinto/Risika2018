//
// Created by dotPinto-PC on 06/03/2019.
//

#ifndef FP_BASE_STUDENTE_H
#define FP_BASE_STUDENTE_H

#include "docente.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define DEBUG_MODE 1

#define MIN_DADO 1
#define MAX_DADO 6
#define MAX_TIRI 6
#define MIN_GIOCATORI 3
#define MAX_GIOCATORI 6
#define LUNGHEZZA_NOME 24      //lunghezza max per il nome giocatore
#define ID_JOLLY1 26
#define ID_JOLLY2 27
#define MAX_CARATTERI_LOGGER 50
#define MAZZOPLUSJOLLY 28
#define GENERAZIONE_CASUALE_SEMI 3
#define MAX_ARMATE 100
#define VINO 6
#define BIRRA 8
#define CAFFE 10
#define BONUS_DUE 2
#define BONUS_TRE 3
#define BONUS_QUATTRO 4
#define FULL_TERRITORIO_TRE 3
#define FULL_TERRITORIO_QUATTRO 4
#define FULL_TERRITORIO_CINQUE 5

#if DEBUG_MODE == 1
#define ARMATE_IN3 12
#define ARMATE_IN4 12
#define ARMATE_IN5 12
#define ARMATE_IN6 12
#else
#define ARMATE_IN3 35
#define ARMATE_IN4 30
#define ARMATE_IN5 25
#define ARMATE_IN6 20
#endif

#define SALVATAGGIO_BIN "salvataggio.rsk"   //Nome del file binario
#define AGGIUNTA_BIN "ab"                   //Apertura in modalita' accodamento
#define SCRITTURA_BIN "wb"
#define LETTURA_BIN  "rb"                   //Apertura in modalita' lettura
#define LETTURA_E_ACCOD_BIN "a+b"       //Apertura in modalita' lettura piu' scrittura
#define STATISTICHE_BIN "Stats.bin"

#define LOGGER_TXT "Log.txt"   //Nome del file binario
#define AGGIUNTA_TXT "a"                    //Apertura in modalita' accodamento
#define SCRIVI_TXT "w"
#define LETTURA_TXT  "r"                    //Apertura in modalita' lettura

#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif

/*dichiaro una variabile globale per rendere piu' semplice l'uso della variabile stessa (selezione = numero di partecipanti)*/
int selezione;

// Enumerazione colore giocatori
typedef enum {rosso, nero, viola, verde, giallo, blu} ColoreGiocatori;
//Enumerazione semi delle carte
typedef enum {caffe, birra, vino, jolly} semeCarta;

typedef enum{SI , NO} Conferme;

typedef enum{INIZIO,COLORE,ELIMINAZIONE,ATTACCO,PESCA,MISCHIA,RINFORZI,VITTORIA,CARICA,SALVA}LoggerInfo;

/*creazione della Nodo doppiamente concatenata per l'utilizzo delle carte */
typedef struct{
    int nTerritorio;
}Informazione;

typedef struct carta{
    Informazione IDcarta;  //ID rappresentante il territorio
    semeCarta seme;        //seme della carta
    struct carta *next;    //puntatore che punta al nodo successivo
    struct carta *prev;    //puntatore che punta al nodo precedente
} Carta;

typedef struct{
    Carta *testa;        //Testa della lista
    Carta *coda;        //Coda della lista
}Nodo;

//struttura di ogni giocatore
typedef struct {
    char nome[LUNGHEZZA_NOME];
    int idGiocatore;
    int nArmateInTerritorio[N_TERRITORI];
    int scortaArmate;
    int maxArmate;
    _Bool status;
    ColoreGiocatori colore;
    Nodo mazzoGiocatore;
} Giocatore;

typedef struct{
    char frase[MAX_CARATTERI_LOGGER];
}Log;

/*
 * 4 tipi di strutture salvataggio
 * la prima contiene le informazioni base (numero giocatori e giocatore corrente
 * la seconda (di N elementi - N sta per il numero di partecipanti
 * la terza sta per il territorio (K elementi - K sta per N_TERRITORI (Macro))
 * per ultimo il mazzo
 * */

typedef struct {
    int numGiocatoriSal;
    int idGiocatoreSal;

} SalvataggioPublic;

typedef struct {
    //Blocco personale singolo giocatore!
    char nomeSal[LUNGHEZZA_NOME];
    ColoreGiocatori coloreSal;
    int nCarteSal;
    int mazzoPersonaleSal[MAZZOPLUSJOLLY];     //vettore degli id delle carte per singolo giocatore
}SalvataggioGiocatori;

typedef struct {
    //blocco del territorio
    int idTerritorioSal;    //(intero, valori possibili [0,1...,25])
    int idPropietarioSal;   //[0,1,2,3,4,5]
    int numeroArmateSal;    //[0,1...,99,100]
}SalvataggioTerreno;

typedef struct {
    //blocco mazzo pubblico
    int nCarteMazzoSal;     //(intero, valori possibili: 0,1,…,27)
    int idCartaMazzoSal[MAZZOPLUSJOLLY];
}SalvataggioMazzo;

//Dichiarazione funzioni e procedure di preparazione

void inizializzaLista(Nodo* lista); //dichiarazione procedura per inizializzazione della lista
Carta *allocaNuovaCarta();          //dichiarazione funzione per l'allocazione dinamica di una carta della lista
void inserisciCarta(Nodo* lista, const Informazione id, semeCarta seme); //dichiarazione procedura per l'inserimento di una nuova carta nella lista
void stampaLista(const Nodo lista);
void inserisciCartaGiocatore(Nodo* mazzo,Giocatore*giocatore);
void aggiungiJolly(Nodo*mazzo);
int contaNCarteMazzo(Nodo*mazzo);
void inserisciInMazzoGioc(Giocatore*giocatore,Carta*nuovaCarta);

int selezione_numero_giocatori();
void riempiListaConSeme(Nodo *lista);
int tiroDado();                 //dichiarazione funzione del lancio del dado
int scegliCasualmente(Giocatore *giocatore);
Giocatore* preparazione(Giocatore *giocatore,int *paux);
void InserimentoNomiG(Giocatore* giocatore);
void setNomeGiocatori(Giocatore* giocatore,int i);
Giocatore* riempiGiocatore(Giocatore *giocatore);
int scegliColore(Giocatore* giocatore, int i);
char* getColore(ColoreGiocatori colore);
void inizializzaMazzoGioc(Giocatore*giocatore);
char* getNomeTerritorio(int nTerritorio);
int controllaCarta(Giocatore*giocatore,int id,Nodo*mazzo);
void shuffleCarte(Nodo*mazzo,int dim);
void pescaCarta(Nodo* giocatore,Nodo *mazzo);
void transferMazzo(Giocatore*giocatore,Giocatore*loser);
void outputMazzo(Giocatore*giocatore);

//Dichiarazione funzioni e procedure di gioco
void menuPrincipale();
void nuovoGioco();
void nuovoGiro(Giocatore* giocatore, Nodo* mazzo,int aux);
void turno(Giocatore*giocatore,Nodo*mazzo,int aux);
void turnoCycle(Giocatore*giocatore,int i,Nodo*mazzo);
_Bool controlloVittoria(Giocatore*giocatore,Nodo*mazzo);
void controlloSalvataggio(Giocatore*giocatore,Nodo*mazzo,int aux);
void finePartita(Giocatore *giocatore,int i,Nodo*mazzo);
void stampaMazzoGioc(Giocatore*giocatore);
void giocaBonus(Giocatore*giocatore,Nodo*mazzo);
void territoriIniziali(Giocatore* giocatore,int i,Nodo *it);
void eliminaGiocatore(Giocatore*giocatoreAtt);
void riempiTerritori(Giocatore*giocatore,int i);
void stampaTerritori(Giocatore*giocatore);
void stampaTerritorioN(Giocatore*giocatore);
_Bool canAttack(Giocatore*giocatore);
_Bool giveStatus(Giocatore*giocatore);
void rinforzo(Giocatore*giocatore,Nodo*mazzo);
void attacco(Giocatore*giocatore,int index,Nodo*mazzo);
void combattimento(Giocatore* gAtt,Giocatore* gDif,int idTerrAtt,int idTerrDif,Nodo*mazzo);
void spostamento(Giocatore*giocatore);

//func files
FILE * apriFile(char *name, char *mod);
FILE * chiudiFile(FILE *fp);
void salvaPartita(Giocatore* giocatore,Nodo * mazzo,int aux,int partecipanti);
void caricaPartita(FILE* fp);
void Salvataggio(FILE *fp, SalvataggioPublic *p, SalvataggioGiocatori *g, SalvataggioTerreno *t, SalvataggioMazzo *m,int partecipanti);
int scriviSuLogger(FILE *fp,LoggerInfo tipo,Giocatore*giocatore);
int scriviColoriSuFile(FILE *fp, Giocatore *giocatore);
int scriviPosizionamentoSuFile(FILE *fp, Giocatore *giocatore,int idTerr);
int scriviVittoriaSuFile(FILE *fp, Giocatore *giocatore);
int updateLoggerACapo(FILE *fp);
int scriviEliminazione(FILE *fp, Giocatore *giocatore);
int leggiStatistica(FILE* fp);
int scriviStatistica(FILE* fp,Giocatore*giocatore);
void clean_stdin();

#endif //FP_BASE_STUDENTE_H
