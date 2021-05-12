#include "studente.h"

FILE * apriFile(char *name, char *mod){
    FILE * fp = NULL;

    fp = fopen(name, mod);

    if(fp == NULL)
        exit(-1);

    return fp;
}

FILE * chiudiFile(FILE *fp){
    //Questo controllo ci permette di evitare di chiudere un file già chiuso in precedenza
    if(fp != NULL)
        fclose(fp);
    return NULL;
}

int scriviStatistica(FILE* fp,Giocatore*giocatore){
    char *nome;
    nome = giocatore->nome;

    fp=apriFile(STATISTICHE_BIN,AGGIUNTA_BIN);

    if(fp==NULL) {
        printf("\nFile stat error: not opened correctly\n");
        return 1;
    }

    if(fp) {

        fwrite(nome,sizeof(char),strlen(nome),fp);

        fflush(stdin);
        fp=chiudiFile(fp);

    }

    if(fp!=NULL) {
        printf("\nFile stat error: not closed correctly\n");
        return 1;
    }

    return 1;
}

int leggiStatistica(FILE* fp){
    char *nome;
    int c=0;

    fp=apriFile(STATISTICHE_BIN,LETTURA_BIN);

    if(fp==NULL) {
        printf("\nFile stat error: not opened correctly\n");
        return 1;
    }

    if(fp) {

        while (fread(&nome, sizeof(char), LUNGHEZZA_NOME, fp) > 0) {
            printf("%d) ",c);
            printf("%s ha vinto la partita", nome);
            printf("\n");
            c++;
        }

        fp=chiudiFile(fp);
    }

    if(fp!=NULL) {
        printf("\nFile stat error: not closed correctly\n");
        return 1;
    }

    return 1;
}

int updateLoggerACapo(FILE *fp){
    char*frase;
    frase="\n\0";

    fp=apriFile(LOGGER_TXT,AGGIUNTA_TXT);

    if(fp==NULL) {
        printf("\nFile log error: not opened correctly\n");
        return 1;
    }

    if(fp) {

        fwrite(frase,sizeof(char),strlen(frase),fp);

        fflush(stdin);
        fp=chiudiFile(fp);

    }

    if(fp!=NULL) {
        printf("\nFile log error: not closed correctly\n");
        return 1;
    }

    return 1;
}

int scriviEliminazione(FILE *fp, Giocatore *giocatore){

    fp=apriFile(LOGGER_TXT,AGGIUNTA_TXT);

    if(fp==NULL) {
        printf("\nFile log error: not opened correctly\n");
        return 1;
    }

    if(fp) {

        //fprintf(fp,x);
        fprintf(fp, " %s E' STATO ELIMINATO! RIP \n", giocatore->nome);

        fp=chiudiFile(fp);

    }

    if(fp!=NULL) {
        printf("\nFile log error: not closed correctly\n");
        return 1;
    }

    return 1;
}

int scriviSuLogger(FILE *fp,LoggerInfo tipo,Giocatore*giocatore){
    char *frase,*col;

    fp=apriFile(LOGGER_TXT,AGGIUNTA_TXT);

    if(fp==NULL) {
        printf("\nFile log error: not opened correctly\n");
        return 1;
    }

    if(fp) {

        switch (tipo){

            case INIZIO:
                //frase = "PARTITA INIZIATA\0";
                //fwrite(frase,sizeof(char),strlen(frase),fp);
                fprintf(fp, "PARTITA INIZIATA\n");
                fprintf(fp, "INIZIA %s\n",giocatore->nome);
                fp=chiudiFile(fp);
                break;

            case COLORE:
                col = getColore(giocatore->colore);
                fprintf(fp, " %s HA SCELTO IL COLORE %s \n", giocatore->nome, col);
                fp=chiudiFile(fp);
                break;
            case VITTORIA:
                fprintf(fp, " %s VINCE LA PARTITA!! \n", giocatore->nome);
                fp=chiudiFile(fp);
                break;
            case ELIMINAZIONE:
                fprintf(fp, " %s E' STATO ELIMINATO! RIP \n", giocatore->nome);
                fp=chiudiFile(fp);
                break;
            case ATTACCO:
                fprintf(fp, "INIZIO FASE ATTACCO \n");
                fp=chiudiFile(fp);
                break;
            case PESCA:
                fprintf(fp, " %s PESCA UNA CARTA \n", giocatore->nome);
                fp=chiudiFile(fp);
                break;
            case MISCHIA:
                fprintf(fp, "MAZZO MISCHIATO \n");
                fp=chiudiFile(fp);
                break;
            case RINFORZI:
                fprintf(fp, "INIZIO FASE RINFORZI \n");
                fp=chiudiFile(fp);
                break;
            case CARICA:
                fprintf(fp, "PARTITA CARICATA! \n");
                fp=chiudiFile(fp);
                break;
            case SALVA:
                fprintf(fp, "PARTITA SALVATA! \n");
                fp=chiudiFile(fp);
                break;

        }
    }

    if(fp!=NULL) {
        printf("\nFile log error: not closed correctly\n");
        return 1;
    }

    return 1;

}

int scriviPosizionamentoSuFile(FILE *fp, Giocatore *giocatore,int idTerr){
    char*str;
    fp=apriFile(LOGGER_TXT,AGGIUNTA_TXT);

    if(fp==NULL) {
        printf("\nFile log error: not opened correctly\n");
        return 1;
    }

    if(fp) {

        str = getNomeTerritorio(idTerr);
        fprintf(fp, "%s POSSIEDE ORA IL TERRITORIO '%s' \n", giocatore->nome, str);

        fp=chiudiFile(fp);

    }

    if(fp!=NULL) {
        printf("\nFile log error: not closed correctly\n");
        return 1;
    }

    return 1;

}

void clean_stdin() {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

void Salvataggio(FILE *fp, SalvataggioPublic *p, SalvataggioGiocatori *g, SalvataggioTerreno *t, SalvataggioMazzo *m,int partecipanti){
    //Scrive sul file un blocco
    int i,j;

    fp=apriFile(SALVATAGGIO_BIN,SCRITTURA_BIN);

    if(fp==NULL) {
        printf("\nSave file error: not opened correctly\n");
    }

    if(fp) {

        clean_stdin();
        fwrite(p, sizeof(SalvataggioPublic), 1, fp);

        for(i=0;i<partecipanti;i++){
            clean_stdin();
            fwrite(&g[i], sizeof(SalvataggioGiocatori), 1, fp);
        }
        for(j=0;j<N_TERRITORI;j++){
            clean_stdin();
            fwrite(&t[j], sizeof(SalvataggioTerreno), 1, fp);
        }

        clean_stdin();
        fwrite(m, sizeof(SalvataggioMazzo), 1, fp);

        clean_stdin();
        fp=chiudiFile(fp);

    }

    if(fp!=NULL) {
        printf("\nSave file error: not closed correctly\n");
    }
}

void caricaPartita(FILE* fp){
    SalvataggioPublic *saverPublic;
    SalvataggioGiocatori *saverGiocatori;
    SalvataggioTerreno *saverTerreno;
    SalvataggioMazzo *saverMazzo;
    int aux,i,j,*it;
    Giocatore *giocatore;
    Nodo *mazzo;
    Informazione id;
    semeCarta semi[N_TERRITORI]={vino,birra,vino,birra,caffe,vino,birra,caffe,vino,caffe,caffe,birra,vino,caffe,
                                 birra,caffe,vino,caffe,birra,caffe,caffe,birra,caffe,caffe,birra,caffe};

    Carta* nuovaCarta;

    fp=apriFile(SALVATAGGIO_BIN,LETTURA_BIN);

    if(!fp) {
        printf("\nSave file error: not opened correctly or not found\n");
        fp=chiudiFile(fp);
        printf("\n\nVerrai Reindirizzato al menu' principale\n\n");
        //non e' stato possibile caricare la partita quindi l'utente verra' reindirizzato al menu' principale

        menuPrincipale();
    }

    if(fp){

        mazzo = (Nodo*)malloc(sizeof(Nodo));
        nuovaCarta = (Carta*)malloc(sizeof(Carta));
        saverPublic = (SalvataggioPublic*)malloc(sizeof(SalvataggioPublic));
        saverGiocatori = (SalvataggioGiocatori*)malloc(MAX_GIOCATORI*sizeof(SalvataggioGiocatori));
        saverTerreno = (SalvataggioTerreno*)malloc(N_TERRITORI*sizeof(SalvataggioTerreno));
        saverMazzo = (SalvataggioMazzo*)malloc(sizeof(SalvataggioMazzo));

        printf("\n\tLoading . . . ");

        //se il file di salvataggio non e' vuoto...

        fread(&saverPublic, sizeof(SalvataggioPublic),1,fp);

        //blocco generale
        selezione = saverPublic->numGiocatoriSal;
        giocatore = riempiGiocatore(giocatore);
        aux = saverPublic->idGiocatoreSal;
        inizializzaMazzoGioc(giocatore);

        if(selezione > 0 && selezione <= MAX_GIOCATORI){
            for(i=0;i<selezione;i++){
                if(DEBUG_MODE==1){
                    printf("\nReading player id: %s",saverGiocatori->nomeSal);
                }
                fread(&saverGiocatori, sizeof(SalvataggioPublic),1,fp);
            }
        } else {
            printf("\nErrore lettura savefile");
        }

        for(i=0;i<selezione;i++){
            j=0;
            if (saverGiocatori->mazzoPersonaleSal[j] == -1)
                printf("\nNessuna carta diponibile ('Player %d -> %s')",giocatore[i].idGiocatore,giocatore[i].nome);
            else{
                for(j=0;j<MAZZOPLUSJOLLY;j++){
                    nuovaCarta->IDcarta.nTerritorio = saverGiocatori->mazzoPersonaleSal[j];
                    //continue here
                    inserisciInMazzoGioc(&giocatore[i],nuovaCarta);
                }
            }
        }

        fread(&saverTerreno, sizeof(SalvataggioPublic),N_TERRITORI,fp);

        int c[N_TERRITORI];
        for(i=0;i<N_TERRITORI;i++){
            c[i] = saverTerreno[i].idTerritorioSal;
        }

        for(i=0;i<selezione;i++){
            for(j=0;j<N_TERRITORI;j++){
                giocatore[i].idGiocatore = saverTerreno[j].idTerritorioSal;
                giocatore[i].nArmateInTerritorio[j] = saverTerreno[j].numeroArmateSal;
            }
        }

        fread(&saverMazzo, sizeof(SalvataggioPublic),1,fp);
        inizializzaLista(mazzo);
        for (i=0;i<MAZZOPLUSJOLLY;i++){
            id.nTerritorio = saverMazzo->idCartaMazzoSal[i];
            inserisciCarta(mazzo,id,semi[id.nTerritorio]);
        }

        for(i=0;i<selezione;i++) {

            printf("Giocatore %d  -  %s ... ready!\n", giocatore[i].idGiocatore, giocatore[i].nome);

        }

        scriviSuLogger(fp, CARICA, giocatore);

        fp=chiudiFile(fp);

        if(fp!=NULL) {
            printf("\nSave file error: not closed correctly\n");
        }

        free(saverPublic);
        free(saverGiocatori);
        free(saverTerreno);
        free(saverMazzo);

        nuovoGiro(giocatore,mazzo,aux);
    }

}

void inserisciInMazzoGioc(Giocatore*giocatore,Carta*nuovaCarta){
    Nodo*node;
    node = (Nodo*) malloc(sizeof(Nodo));
    node->testa = giocatore->mazzoGiocatore.testa;

    if(node->testa==NULL) {
        nuovaCarta->prev = NULL;
        node->testa = nuovaCarta;
        node->coda = nuovaCarta;
    }
    else{
        nuovaCarta->prev=node->coda;
        node->coda->next=nuovaCarta;
        node->coda=nuovaCarta;
    }
}

void salvaPartita(Giocatore* giocatore,Nodo * mazzo,int aux,int partecipanti){
    FILE* fp = NULL;
    SalvataggioPublic *saverPublic;
    SalvataggioGiocatori *saverGiocatori;
    SalvataggioTerreno *saverTerreno;
    SalvataggioMazzo *saverMazzo;
    int counterCarte,size=1,i,j,selezione;

    Nodo*it = (Nodo*)malloc(sizeof(Nodo));

    saverPublic=(SalvataggioPublic*)malloc(sizeof(SalvataggioPublic));
    saverGiocatori=(SalvataggioGiocatori*)malloc(partecipanti* sizeof(SalvataggioGiocatori));
    saverTerreno=(SalvataggioTerreno*)malloc(N_TERRITORI* sizeof(SalvataggioTerreno));
    saverMazzo=(SalvataggioMazzo*)malloc(sizeof(SalvataggioMazzo));

    //salvataggio primi due punti
    saverPublic->numGiocatoriSal = partecipanti;
    saverPublic->idGiocatoreSal = aux;

    //salvataggio blocco giocatore (dipende dal numero di giocatori)
    for(i=0;i<partecipanti;i++) {
        counterCarte = 0;
        strcpy(saverGiocatori[i].nomeSal, giocatore[i].nome);
        saverGiocatori[i].coloreSal = giocatore[i].colore;

        if(giocatore[i].mazzoGiocatore.testa == NULL){

            if(DEBUG_MODE==1){
                printf("\nNessuna carta da salvare ");
                printf("per %s!",giocatore[i].nome);
            }

            /*Se non possiedono nessuna carta il valore va a -1*/

            for(j=0;j<MAZZOPLUSJOLLY;j++){
                saverGiocatori->mazzoPersonaleSal[j] = -1;
            }
            saverGiocatori[i].nCarteSal = counterCarte;

        }

        else {
            it->testa = giocatore[i].mazzoGiocatore.testa;

            j = 0;
            while (it->testa != NULL) {
                saverGiocatori->mazzoPersonaleSal[j] = it->testa->IDcarta.nTerritorio;
                it->testa = it->testa->next;
                counterCarte++;
                j++;
            }

            saverGiocatori[i].nCarteSal = counterCarte;
        }
    }

    //blocco tabellone
    int c=0;
    for(i=0;i<N_TERRITORI;i++){
        saverTerreno[i].idTerritorioSal = c;
        c++;
    }

    for(i=0;i<partecipanti;i++){
        for(j=0;j<N_TERRITORI;j++){
            if (giocatore[i].nArmateInTerritorio[j] >= 1) {
                saverTerreno[j].idTerritorioSal = giocatore[i].idGiocatore;
                saverTerreno[j].numeroArmateSal = giocatore[i].nArmateInTerritorio[j];
            }
        }
    }

    //saverPublic per il mazzo
    it->testa = mazzo->testa;
    int contaCarte = 0;
    while (it->testa!=NULL){
        it->testa = it->testa->next;
        contaCarte++;
    }
    saverMazzo->nCarteMazzoSal = contaCarte;

    /*Prendo la testa del mazzo e salvo uno per volta l'id della carta nel vettore definito di tipo SalvataggioPublic*/
    it->testa = mazzo->testa;

    for (i=0;it->testa!=NULL || i<MAZZOPLUSJOLLY;i++){
        saverMazzo->idCartaMazzoSal[i] = it->testa->IDcarta.nTerritorio;
        it->testa = it->testa->next;
    }

    clean_stdin();

    Salvataggio(fp, saverPublic, saverGiocatori, saverTerreno, saverMazzo,partecipanti);

    free(saverPublic);
    free(saverGiocatori);
    free(saverTerreno);

}

/**
 * Funzione per allocare dinamicamente lo spazio in memoria per una nuova carta della lista
 * @return
 */
Carta *allocaNuovaCarta(){
    Carta *nuovaCarta=(Carta*)malloc(sizeof(Carta));
    if(nuovaCarta==NULL)
        exit(-1);
    return nuovaCarta;
}

//inizializzazione della lista
/**
 * Questa funzione inizializza la lista (later -> mazzo) puntando la testa e la coda a NULL
 * @param lista
 */
void inizializzaLista(Nodo* lista){
    lista->testa=NULL;
    lista->coda=NULL;
}

//procedura per inserire una nuova carta nel mazzo
/**
 * Questa funzione alloca una nuova carta e assegna il valore del seme a ciascuna
 * @param lista
 * @param id
 * @param seme
 */
void inserisciCarta(Nodo* lista, const Informazione id, semeCarta seme){

    Carta *nuovaCarta=allocaNuovaCarta();
    nuovaCarta->IDcarta.nTerritorio=id.nTerritorio;         // assegno i dati al nuovo nodo e lo inizializzo
    nuovaCarta->seme=seme;
    nuovaCarta->next=NULL;

    if(lista->testa==NULL) {       //controllo che la lista non sia vuota
        nuovaCarta->prev = NULL;
        lista->testa = nuovaCarta; // in tal caso, testa e coda della lista saranno uguali
        lista->coda = nuovaCarta;
    }
    else{                         //altrimenti inserisco la nuova carta alla coda della lista
        nuovaCarta->prev=lista->coda;
        lista->coda->next=nuovaCarta;
        lista->coda=nuovaCarta;
    }
}

/**
 * questa procedura assegna progressivamente gli ID dei territori alle carte
 * @param lista
 */
void riempiListaConSeme(Nodo *lista){
    //semeCarta semi[N_TERRITORI]={2,1,2,1,0,2,1,0,2,0,0,1,2,0,1,0,2,0,1,0,0,1,0,0,1,0};
    semeCarta semi[N_TERRITORI]={vino,birra,vino,birra,caffe,vino,birra,caffe,vino,caffe,caffe,birra,vino,caffe,
                                 birra,caffe,vino,caffe,birra,caffe,caffe,birra,caffe,caffe,birra,caffe};

    Informazione n;
    n.nTerritorio=0;
    while(n.nTerritorio<N_TERRITORI){
        inserisciCarta(lista,n,semi[n.nTerritorio]);
        n.nTerritorio++;
    }
}

/**
 * procedura che serve per stampare (su schermo) la lista
 * utilizzata principalmente per debug
 * @param lista (later -> mazzo pubblico)
 */
void stampaLista(const Nodo lista){
    Carta *it;
    it=lista.testa;
    while(it!=NULL){
        printf("-----------------\n");
        printf("%d  ", it->IDcarta.nTerritorio);
        printf("%d  \n", it->seme);
        it=it->next;
    }
}

/**
 * procedura principale formata da uno switch case
 * viene chiesto all'utente di selezionare una delle varie opzioni
 */
void menuPrincipale() {
    int selezioneMenu;
    FILE *fp = NULL;

    printf("\n");
    printf("\t ########   ####   ######   ####  ##    ##     ###    \n");
    printf("\t ##     ##   ##   ##    ##   ##   ##   ##     ## ##   \n");
    printf("\t ##     ##   ##   ##         ##   ##  ##     ##   ##  \n");
    printf("\t ########    ##    ######    ##   #####     ##     ## \n");
    printf("\t ##   ##     ##         ##   ##   ##  ##    ######### \n");
    printf("\t ##    ##    ##   ##    ##   ##   ##   ##   ##     ## \n");
    printf("\t ##     ##  ####   ######   ####  ##    ##  ##     ## \n");
    printf("\n");

    printf("\n\nSeleziona un opzione \n");
    printf("\n1.Nuova Partita\n2.Carica Partita\n3.Records\n4.Credits\n0.Exit");
    printf("\n\n\n\t\tR.Pintore Inc. Copyright 2019");
    printf("\n:>");
    scanf("%d", &selezioneMenu);

    while (selezioneMenu != 0){
        switch (selezioneMenu) {

            case 1:
                nuovoGioco();
                break;
            case 2:
                caricaPartita(fp);
                break;
            case 3:
                leggiStatistica(fp);
                printf("\n:>");
                scanf("%d", &selezioneMenu);
                break;
            case 4:
                printf("\n\tMade in Cagliari \n\t\tby: \n\tRiccardo Pintore");
                printf("\n:>");
                scanf("%d", &selezioneMenu);
                break;
            case 0:
                printf("\nUscita in corso");
                break;
            default:
                printf("Critical error: failed selection");
                printf("\n:>");
                scanf("%d", &selezioneMenu);
        }
    }
}

/**
 * procedura che non riceve in ingresso nulla, avvia un nuovo gioco, utilizza funzioni precedentemente definite come
 * inizializza lista e riempilistaconseme
 * 1 selezione dei territori iniziali (pescano una carta e occupano il territorio) con successivo mashup del mazzo
 * (tutto in maniera automatica)
 */
void nuovoGioco() {
    int logger,check;
    int *aux,i,j,contatore;
    Giocatore *giocatore;
    Nodo *mazzo;
    char *bufferLog,*str;
    FILE *fp = NULL;
    Nodo *it = (Nodo*)malloc(sizeof(Nodo));

    aux=malloc(1* sizeof(int));

    mazzo = malloc(sizeof(Carta));
    inizializzaLista(mazzo);
    riempiListaConSeme(mazzo);

    if(DEBUG_MODE == 1){
        stampaLista(*mazzo);
        printf("\nmazzo caricato correttamente!\n\n");
    } else {
        system(CLEAR);
    }

    //l'utente seleziona il numero di partecipanti alla partita tramite un numero compreso tra 3 e 6

    giocatore = preparazione(giocatore,aux);

    //bufferLog = "PARTITA INIZIATA\0";


    //Inizia il ciclo della selezione dei colori

    printf("\nIl giocatore che iniziera' la partita sara' giocatore ID: %d\n\n",*aux);

    logger = scriviSuLogger(fp, INIZIO, &giocatore[*aux]);

    do {
        check = scegliColore(giocatore, *aux);
    } while (check == 1);

    printf("\n\n\tScelta del colore terminata!\n");

    shuffleCarte(mazzo,N_TERRITORI);

    if(DEBUG_MODE == 1){
        stampaLista(*mazzo);
    }

    printf("\n\n\tMazzo mischiato!!\n");

    inizializzaMazzoGioc(giocatore);

    //Piazzo le armate (corrispondenti alle carte possedute dal giocatore nel proprio mazzo) nel territorio opportuno

    it->testa = mazzo->testa;

    while(it->testa!=NULL){
        for(i=*aux;i<selezione;i++){
            territoriIniziali(giocatore,i,it);
        }
        if (*aux != 0) {
            for (i = 0; i < *aux; i++) {
                territoriIniziali(giocatore,i,it);
            }

        }
    }

    for(i=0;i<selezione;i++){
        printf("\n%s ti sono rimaste %d armate\n",giocatore[i].nome,giocatore[i].scortaArmate);
    }

    //aggiungo le carte Jolly
    aggiungiJolly(mazzo);

    contatore = contaNCarteMazzo(mazzo);

    shuffleCarte(mazzo,contatore);

    if(DEBUG_MODE == 1){
        stampaLista(*mazzo);
        printf("\n");
    }

    printf("\n\n\tMazzo mischiato!\n");

    printf("\n\tFase rinforzo dei territori!\n");

    for(j=0;j<selezione;j++){
        while (giocatore[j].scortaArmate > 0){

            for(i=*aux;i<selezione;i++) {
                riempiTerritori(giocatore,i);
            }
            if(*aux != 0){
                for(i=0;i<*aux;i++){
                    riempiTerritori(giocatore,i);
                }
            }

        }

    }

    printf("\n\tFase rinforzo completata!\n\n");

    for(i=0;i<selezione;i++){
        for(j=0;j<N_TERRITORI;j++){
            if(giocatore[i].nArmateInTerritorio[j] >= 1){
                str = getNomeTerritorio(j);
                printf("\n%s ha %d armate in %d '%s'",giocatore[i].nome,giocatore[i].nArmateInTerritorio[j],j,str);
            }
        }
    }

    /*
     * Ora che la preparazione e' terminata inizia il giro di turni vero e proprio
     * */

    nuovoGiro(giocatore,mazzo,*aux);

}

/**
 * funzione booleana che restituisce:
 * @param giocatore
 * @param mazzo
 * @return true in caso uno dei giocatori abbia vinto la partita (occupando tutti i territori)
 * @return false in caso nessuno abbia occupato tutti i territori
 */
_Bool controlloVittoria(Giocatore*giocatore,Nodo*mazzo){
    int i,counterVittoria,iArm;
    //Controllo per stabilire se un giocatore ha vinto la partita
    for(i=0;i<selezione;i++){
        counterVittoria = 0;
        for(iArm=0;iArm<N_TERRITORI;iArm++){
            if(giocatore[i].nArmateInTerritorio[iArm] >= 1){
                counterVittoria++;
                if(counterVittoria >= N_TERRITORI)
                    return true;
                    //finePartita(giocatore,i,mazzo);
            }
        }
    }
    return false;
}

void controlloSalvataggio(Giocatore*giocatore,Nodo*mazzo,int aux){
    int checkSalvataggio;
    int logger;
    FILE*fp=NULL;

    do {
        printf("\nVuoi salvare la partita? (0 Si - 1 No)\n:>");
        scanf("%d", &checkSalvataggio);

    } while (checkSalvataggio != SI && checkSalvataggio != NO); //controllo che non vengano immessi input indesiderati

    if(checkSalvataggio == SI) {

        logger = scriviSuLogger(fp, SALVA, giocatore);

        salvaPartita(giocatore,mazzo,aux,selezione);

        if(DEBUG_MODE == 1)
            printf("\nSalva Partita successful");

    }
}

void nuovoGiro(Giocatore* giocatore, Nodo* mazzo,int aux){

    do {
        controlloSalvataggio(giocatore,mazzo,aux);
        turno(giocatore,mazzo,aux);
    } while(!controlloVittoria(giocatore,mazzo));
}

/*inizia il giro di turni finche verra' decretato un vincitore*/
void turno(Giocatore*giocatore,Nodo*mazzo,int aux) {
    int i;

    for (i = aux; i < selezione; i++) {
        turnoCycle(giocatore, i, mazzo);
    }

    if (aux != 0) {
        for (i = 0; i < aux; i++)
        turnoCycle(giocatore, i, mazzo);
    }
}

void turnoCycle(Giocatore*giocatore,int i,Nodo*mazzo){
    _Bool flag=false;
    Conferme conf,x;
    int j;

    if (giocatore->status == false) {
        printf("\n%s - eliminato\tRIP", giocatore->nome);
    } else {
        printf("\n\tTurno di %s!",giocatore[i].nome);
        /*Fase Rinforzo*/
        rinforzo(&giocatore[i],mazzo);
        /*Fase Attacco*/
        flag = canAttack(&giocatore[i]);
        if(flag){
            printf("\n%s vuoi attaccare? (0-Si 1-No)", giocatore[i].nome);
            scanf("%d", &x);

            if (x == SI) {
                flag = canAttack(&giocatore[i]);
                printf("\n\tFase d\'Attacco!");
                while (flag == true) {

                    attacco(giocatore, i,mazzo);

                    printf("\nVuoi smettere di attaccare? (0-Si 1-No)");
                    scanf("%d",&x);

                    if (x==0){
                        flag = false;
                    }
                }
            }

            for(j=0;j<N_TERRITORI;j++){
                if(giocatore->nArmateInTerritorio[j] > 1)
                    flag = true;
            }

            if (flag) {
                do {
                    printf("\n%s vuoi effettuare uno spostamento strategico? (0 Si - 1 No)",giocatore[i].nome);
                    scanf("%d", &conf);
                } while (conf != SI && conf != NO);

                if (conf == SI) {
                    spostamento(&giocatore[i]);
                }
            }
        }
    }

    flag = controlloVittoria(giocatore,mazzo);
    if(flag){
        finePartita(giocatore,i,mazzo);
    }
}


void spostamento(Giocatore*giocatore){
    _Bool flag=false;
    int num,j,x,Terr,newTerr;
    Conferme conf;
    char*str,*str2;

    printf("\nLista armate che puoi spostare ");
    for(j=0;j<N_TERRITORI;j++){
        if(giocatore->nArmateInTerritorio[j] > 1){
            str = getNomeTerritorio(j);
            printf("\nID: %d '%s'",j,str);
        }
    }
    flag = false;
    printf("\nSeleziona ID: ");
    do{
        printf("\n:>");
        scanf("%d",&Terr);
        if(giocatore->nArmateInTerritorio[Terr] > 1){
            flag = true;
        } else {
            printf("\nErrore");
        }
    } while (!flag);
    printf("\nPuoi spostare in: ");

    for(j=0;j<N_TERRITORI;j++){
        if(giocatore->nArmateInTerritorio[j] >= 1 && j!=Terr){
            str = getNomeTerritorio(j);
            printf("\nID: %d '%s'",j,str);
        }
    }

    printf("\n\t---------- ");
    flag = false;
    printf("\nSeleziona il Territorio in cui vuoi spostare le armate");

    do{
        printf("\n:>");
        scanf("%d",&newTerr);

        if(giocatore->nArmateInTerritorio[newTerr] >= 1)
            flag = true;
        if(!flag)
            printf("\nErrore, non possiedi questo territorio");
    } while (!flag);


    str = getNomeTerritorio(Terr);
    str2 = getNomeTerritorio(newTerr);
    printf("\nSpostamento da %d '%s' a %d '%s'",Terr,str,newTerr,str2);
    x = giocatore->nArmateInTerritorio[Terr] - 1;
    printf("\nQuante armate desideri spostare MAX: %d",x);

    do{
        printf("\n:>");
        scanf("%d",&num);

        if(num < 0 || num > x)
            printf("\nNumero non valido!");
    } while (num < 0 || num > x);

    giocatore->nArmateInTerritorio[Terr] = giocatore->nArmateInTerritorio[Terr] - x;
    giocatore->nArmateInTerritorio[newTerr] = giocatore->nArmateInTerritorio[newTerr] + x;

    printf("\nOra hai le seguenti armate nei territori: ");

    for(j=0;j<N_TERRITORI;j++){
        if(giocatore->nArmateInTerritorio[j] >= 1){
            str = getNomeTerritorio(j);
            printf("\nID: %d '%s' Armate: %d",j,str,giocatore->nArmateInTerritorio[j]);
        }
    }
    printf("\n");

    printf("\nVuoi spostare altre armate? (0 SI - 1 NO)");
    do{
        printf("\n:>");
        scanf("%d",&conf);
    } while (conf != SI && conf != NO);
    if(conf == SI){
        spostamento(giocatore);
    }
}

_Bool canAttack(Giocatore*giocatore){
    int j;

    for(j=0;j<N_TERRITORI;j++){
        if(giocatore->nArmateInTerritorio[j] > 1){
            return true;
        }
    }

    return false;
}

void attacco(Giocatore*giocatore,int index,Nodo*mazzo) {
    int id, j,i, idTerrDif,idTerrAtt;
    Conferme x;
    _Bool flag = false,flagTerrAtt = false;
    FILE* fp=NULL;

    scriviSuLogger(fp, ATTACCO, giocatore);

    for (j = 0; j < N_TERRITORI; j++) {
        if (giocatore[index].nArmateInTerritorio[j] > 1) {
            flag = true;
        }
    }

    if (flag == false) {
        printf("\nNessun attacco possibile!");
        return;
    } else {

        printf("\nPuoi attaccare da: ");
        for (j = 0; j < N_TERRITORI; j++) {
            if (giocatore[index].nArmateInTerritorio[j] > 1) {
                printf("\nID: %d", j);
            }
        }

        do{
            printf("\nSeleziona l'ID del territorio: ");
            scanf("%d",&idTerrAtt);


            if(giocatore[index].nArmateInTerritorio[idTerrAtt] > 1){
                flagTerrAtt = true;
            }

            if(flagTerrAtt != true){
                printf("\nNon possiedi questo territorio! Devi selezionare un altro ID");
            }

            } while (flagTerrAtt != true);

        for(i=0;i<selezione;i++){
            if(i!=index)
                stampaTerritorioN(&giocatore[i]);
        }

            do {
                do{
                    flag = false;
                    printf("\nSeleziona l'ID del giocatore che vuoi attaccare\n:>");
                    scanf("%d", &id);

                    if(id == index)
                        printf("\nNon puoi attaccare te stesso");
                    else
                        flag=true;

                } while (!flag);

                flag = false;

                for (j = 0; j < N_TERRITORI; j++) {
                    if (giocatore[id].nArmateInTerritorio[j] >= 1) {
                        if (isAdjacent(idTerrAtt, j)) {
                            flag = true;
                            printf("\n%s possiede ID: %d", giocatore[id].nome, j);
                        }
                    }

                    if (id > selezione || id < 0 || giocatore[id].status == false) {
                        printf("Giocatore insistente o eliminato");
                    }
                }
            } while (id > selezione || id < 0 || giocatore[id].status == false || flag == false);

                /*
            for (j = 0; j < N_TERRITORI; j++) {
                if (giocatore[id].nArmateInTerritorio[j] >= 1) {
                    if(isAdjacent(idTerrAtt,j)){
                        printf("\n%s possiede ID: %d", giocatore[id].nome, j);
                    }
                }
            }
                 */

            flag = false;

            do {
                printf("\nSeleziona il territorio da attaccare\n:>");
                scanf("%d", &idTerrDif);

                if (idTerrDif < 0 || idTerrDif > N_TERRITORI)
                    printf("\nTerritorio non disponibile");
                else if (giocatore[id].nArmateInTerritorio[idTerrDif] == 0)
                    printf("\nQuesto territorio non appartiene a %s", giocatore[id].nome);
                else
                    flag = true;

            } while (flag != true);

            if(isAdjacent(idTerrAtt,idTerrDif) == true){

                combattimento(&giocatore[index],&giocatore[id],idTerrAtt,idTerrDif,mazzo);

                if(!giveStatus(&giocatore[id])){
                    giocatore[id].status = false;
                    transferMazzo(&giocatore[index],&giocatore[id]);
                }

            } else {
                for (j = 0; j < N_TERRITORI || flag == true; j++) {
                    if(giocatore[index].nArmateInTerritorio[j] > 1){
                        flag = isAdjacent(idTerrAtt,idTerrDif);
                        if(flag)
                            idTerrAtt = j;
                    }
                }

                if (flag == false) {
                    printf("\nErrore di selezione dei territori");
                } else {
                    combattimento(&giocatore[index],&giocatore[id],idTerrAtt,idTerrDif,mazzo);
                }


            } while (idTerrDif < 0 || idTerrDif > N_TERRITORI || flag == false);
        }

    if (DEBUG_MODE == 1){
        stampaTerritori(giocatore);
    }

}

void transferMazzo(Giocatore*giocatore,Giocatore*loser){
    int i;
    char *str;

    do{
        if (loser->mazzoGiocatore.testa == NULL){
            printf("\nMazzo di %d vuoto",loser->nome);
            return;
        }

        Carta* nuovoNodo = (Carta*)malloc(sizeof(Carta));

        nuovoNodo->IDcarta = loser->mazzoGiocatore.testa->IDcarta;
        nuovoNodo->seme = loser->mazzoGiocatore.testa->seme;

        nuovoNodo->next = giocatore->mazzoGiocatore.testa;
        nuovoNodo->prev = NULL;

        if (giocatore->mazzoGiocatore.testa != NULL){
            giocatore->mazzoGiocatore.testa->prev = nuovoNodo;
        }

        giocatore->mazzoGiocatore.testa = nuovoNodo;

        str = getNomeTerritorio(nuovoNodo->IDcarta.nTerritorio);
        printf("\nHai pescato %d '%s'",nuovoNodo->IDcarta.nTerritorio,str);

        if(DEBUG_MODE == 1){
            outputMazzo(giocatore);
        }

        loser->mazzoGiocatore.testa = loser->mazzoGiocatore.testa->next;

    } while(loser->mazzoGiocatore.testa != NULL);
}

void outputMazzo(Giocatore*giocatore){
    Nodo*it;

    it=(Nodo*)malloc(sizeof(Nodo));
    it->testa=giocatore->mazzoGiocatore.testa;
    while(it->testa!=NULL){
        printf("\nID_GIOC %d\tID_CARTA = %d",giocatore->idGiocatore,it->testa->IDcarta.nTerritorio);
    }
    free(it);
}

_Bool giveStatus(Giocatore*giocatore){
    int j;

    for(j=0;j<N_TERRITORI;j++){
        if(giocatore->nArmateInTerritorio[j] >= 1)
            return true;
    }
    return false;
}

void combattimento(Giocatore* gAtt,Giocatore* gDif,int idTerrAtt,int idTerrDif,Nodo*mazzo){
    int maxDadi = 0, xAtt=0,xDif=0,x,num;
    int i=0,j,maxA,maxD;
    int array[MAX_TIRI];
    char *str;
    _Bool flag;

    maxDadi = gAtt->nArmateInTerritorio[idTerrAtt];
    if(maxDadi > 3)
        maxDadi = 3;

    printf("\n\tAttaccante");
    printf("\n%s quanti dadi vuoi utilizzare? MAX: %d",gAtt->nome,maxDadi);
    do {
        scanf("%d",&xAtt);
    } while (xAtt<0 || xAtt>maxDadi);
    printf("\n%s ha selezionato %d dadi",gAtt->nome,xAtt);

    maxDadi = gDif->nArmateInTerritorio[idTerrDif];
    if(maxDadi > 3)
        maxDadi = 3;

    printf("\n\tDifensore");
    printf("\n%s quanti dadi vuoi utilizzare? MAX: %d",gDif->nome,maxDadi);
    do {
        scanf("%d",&xDif);
    } while (xDif<0 || xDif>maxDadi);
    printf("\n%s ha selezionato %d dadi",gDif->nome,xDif);

    if (DEBUG_MODE == 1){
        printf("\nTerritori ATT %s\n",gAtt->nome);
        stampaTerritorioN(gAtt);
        printf("\nTerritori DIF %s\n",gDif->nome);
        stampaTerritorioN(gDif);
    }

    /*Inizio combattimento vero e proprio*/

    for(i=0;i<(xAtt+xDif);i++){
        array[i]=tiroDado();
    }

    maxA=array[xAtt-1];
    for(i=0;i<xAtt;i++){
        if(maxA<array[i])
            maxA=array[i];
    }

    maxD=array[xDif-1];
    for(i=xAtt;i<xDif;i++){
        if(maxD<array[i])
            maxD=array[i];
    }

    if (maxA > maxD){

        gDif->nArmateInTerritorio[idTerrDif]--;
        printf("\n%s [ATT] vince la battaglia!",gAtt->nome);
        pescaCarta(&gAtt->mazzoGiocatore,mazzo);
        if(DEBUG_MODE == 1){
            printf("\nID Gioc %d 'Carta pescata' -> ",gAtt->idGiocatore);
            stampaMazzoGioc(gAtt);
        }

        if(gDif->nArmateInTerritorio[idTerrDif]==0){

            str=getNomeTerritorio(idTerrDif);
            printf("\nTerritorio '%s' conquistato da %s",str,gAtt->nome);
            x = gAtt->nArmateInTerritorio[idTerrAtt];
            x--;
            printf("\nQuante armate vuoi mettere nel tuo nuovo territorio? MAX: %d",x);

            flag = false;

            do{
                scanf("%d",&num);
                if(num < 0 || num > x)
                    printf("\nErrore, stai trasportando troppe armate");
            } while (num < 0 || num > x);

            for(j=0;j<N_TERRITORI;j++){
                if(gDif->nArmateInTerritorio[j] >=1)
                    flag = true;
            }
            if(!flag)
                eliminaGiocatore(gDif);

            gAtt->nArmateInTerritorio[idTerrDif] = num;
        }
    } else {
        gAtt->nArmateInTerritorio[idTerrAtt]--;

        int EasterEgg;
        EasterEgg = rand()%1001;
        printf("\n%s [DIF] vince la battaglia ",gDif->nome);
        if(EasterEgg == 1000)
            printf("ma non la guerra!");
    }
}

void stampaTerritorioN(Giocatore*giocatore){
    int j;
    char *str;
        for (j=0;j<N_TERRITORI;j++){
            if(giocatore->nArmateInTerritorio[j] >= 1){
                str=getNomeTerritorio(j);
                printf("\n%s (ID: %d) possiede %d '%s'",giocatore->nome,giocatore->idGiocatore,j,str);
            }
        }
        printf("\n\t--------------");
}

void stampaTerritori(Giocatore*giocatore){
    int i,j;
    char *str;
    printf("\n\tTABELLONE TERRITORIO");
    for(i=0;i<selezione;i++){
        for (j=0;j<N_TERRITORI;j++){
            if(giocatore[i].nArmateInTerritorio[j]>=1){
                str=getNomeTerritorio(j);
                printf("\n%s possiede %d '%s'",giocatore[i].nome,j,str);
            }
        }
        printf("\n\t-----------");
    }
}

void rinforzo(Giocatore*giocatore,Nodo*mazzo){
    int terr,j,contaArmate = 0,x=0;
    char*str;
    _Bool flag=false;

    for(j=0;j<N_TERRITORI;j++){
        if(giocatore->nArmateInTerritorio[j] >= 1)
            contaArmate++;
    }

    giocatore->scortaArmate = giocatore->scortaArmate + (contaArmate/3);
    giocatore->maxArmate = giocatore->maxArmate + giocatore->scortaArmate;

    contaArmate = 0;    //azzero la variabile

    giocaBonus(giocatore,mazzo);

    if(giocatore->maxArmate <= 100){

        printf("\n%s ha ora %d armate ", giocatore->nome, giocatore->scortaArmate);

    } else {
        printf("\n%s non puo' piu' ritirare delle armate",giocatore->nome);
        do{
            giocatore->scortaArmate = giocatore->scortaArmate - 1;
            giocatore->maxArmate = giocatore->maxArmate - 1;
        } while (giocatore->maxArmate > MAX_ARMATE);

    }

    printf("\n%s dove vuoi mettere le tue armate? ");
    printf("\nTerritori controllati: ");
    for(j=0;j<N_TERRITORI;j++){
        if(giocatore->nArmateInTerritorio[j] >= 1){
         str = getNomeTerritorio(j);
         printf("\nID: %d '%s'",j,str);
        }
    }

    while(giocatore->scortaArmate > 0){
        do{
            printf("\nInserisci l\'ID:\n:>");
            scanf("%d",&terr);

            if(giocatore->nArmateInTerritorio[terr] >= 1 && terr < N_TERRITORI)
                flag = true;

        } while(!flag);

        flag = false;
        do{
            printf("\nNumero armate MAX %d\n:>",giocatore->scortaArmate);
            scanf("%d",&x);

            if((giocatore->scortaArmate - x) >= 0)
                flag = true;
            else
                printf("\nHai selezionato troppe armate");

        } while(!flag);

        giocatore->scortaArmate = giocatore->scortaArmate - x;
    }
}

/**
 * Procedimento che permette di stabilire se il giocatore 'i' puo' giocare il bonus
 * di carte per avere il Bonus
 * @param giocatore
 * @param mazzo
 */
void giocaBonus(Giocatore*giocatore,Nodo*mazzo){
    int contaCaffe=0,contaBirra=0,contaVino=0,contaJolly=0;
    char*str;
    int j=0,sel,contaN=3,id,x;
    Nodo*it;
    it=(Nodo*)malloc(sizeof(Nodo));


    if(giocatore->mazzoGiocatore.testa!=NULL){
        it->testa = giocatore->mazzoGiocatore.testa;
        printf("\n");
        while(it->testa!=NULL){

            if(it->testa->seme == 0)
                contaCaffe++;
            if(it->testa->seme == 1)
                contaBirra++;
            if(it->testa->seme == 2)
                contaVino++;
            if(it->testa->seme == 3)
                contaJolly++;

            it->testa = it->testa->next;
            j++;
        }

        printf("\nHai %d carte caffe",contaCaffe);
        printf("\nHai %d carte birra",contaBirra);
        printf("\nHai %d carte vino",contaVino);
        printf("\nHai %d carte Jolly\n",contaJolly);

    }

    stampaMazzoGioc(giocatore);

    if(contaJolly==0){
        if (contaCaffe==3 || contaBirra==3 || contaVino==3){
            do{
            printf("\nVuoi usare una combinazione di carte per ottenere armate extra? (0-Si 1-No");
            scanf("%d",&sel);
            } while (sel < 0 || sel > 1);

            if(sel == 0){
                while(contaN > 0){
                    printf("\nScrivi gli id delle carte che intendi usare:");
                    do{
                        printf("\n:>");
                        scanf("%d",&id);
                        x=controllaCarta(giocatore,id,mazzo);
                    } while (x!=1);

                    contaN--;
                }

            }
        }
    } else if (contaCaffe==2 || contaBirra==2 || contaVino==2){
        printf("\nVuoi usare una combinazione di carte per ottenere armate extra?");
    } else {
        printf("\nNon puoi effettuare nessuna cobinazione!");
    }

    free(it);

}

/**
 * Inserimento in coda utilizzato per inserire le carte utilizzate nel mazzo
 * @param giocatore
 * @param id
 * @param mazzo
 * @return 1, se il trasferimento e' avvenuto, 0 altrimenti
 */
int controllaCarta(Giocatore*giocatore,int id,Nodo*mazzo){
    Nodo*it,*ultEle;
    it=(Nodo*)malloc(sizeof(Nodo));
    ultEle=(Nodo*)malloc(sizeof(Nodo));
    it->testa= giocatore->mazzoGiocatore.testa;

    while (it->testa!=NULL){
        if (id == it->testa->IDcarta.nTerritorio){
            Carta* nuovoNodo = (Carta*)malloc(sizeof(Carta));

            ultEle->testa = mazzo->testa;

            nuovoNodo->IDcarta = it->testa->IDcarta;
            nuovoNodo->seme = it->testa->seme;

            nuovoNodo->next = NULL;

            if (mazzo->testa == NULL) {
                nuovoNodo->prev = NULL;
                mazzo->testa = nuovoNodo;
            }

            while (ultEle->testa!=NULL)
                ultEle->testa = ultEle->testa->next;

            ultEle->testa->next = nuovoNodo;

            nuovoNodo->prev = ultEle->testa;

            it->testa->prev = it->testa->next;

            free(ultEle);
            free(it);

            return 1;
        }
    }


    free(it);
    printf("\nNon possiedi questa carta!");
    return 0;

}

void armateBonus(Giocatore*giocatore){
    int j,contaBonus = 0;

    //Studi Umanistici (0-1-2-3-4)
    for(j=0;j<4;j++){
        if (giocatore->nArmateInTerritorio[j] >= 1){
            contaBonus++;
        }
        if (contaBonus == FULL_TERRITORIO_CINQUE)
            giocatore->scortaArmate = giocatore->scortaArmate + BONUS_TRE;
    }

    //Scienze Economiche Giur. (5-6-7)
    for(j=5;j<7;j++){
        if (giocatore->nArmateInTerritorio[j] >= 1){
            contaBonus++;
        }
        if (contaBonus == FULL_TERRITORIO_TRE)
            giocatore->scortaArmate = giocatore->scortaArmate + BONUS_DUE;
    }

    //Medicina (8-9-10-11)
    for(j=8;j<11;j++){
        if (giocatore->nArmateInTerritorio[j] >= 1){
            contaBonus++;
        }
        if (contaBonus == FULL_TERRITORIO_QUATTRO)
            giocatore->scortaArmate = giocatore->scortaArmate + BONUS_DUE;
    }

    //Biologia (12-13-14-15-16)
    for(j=12;j<16;j++){
        if (giocatore->nArmateInTerritorio[j] >= 1){
            contaBonus++;
        }
        if (contaBonus == FULL_TERRITORIO_CINQUE)
            giocatore->scortaArmate = giocatore->scortaArmate + BONUS_QUATTRO;
    }

    //Ingegneria (17-18-19-20-21)
    for(j=17;j<21;j++){
        if (giocatore->nArmateInTerritorio[j] >= 1){
            contaBonus++;
        }
        if (contaBonus == FULL_TERRITORIO_CINQUE)
            giocatore->scortaArmate = giocatore->scortaArmate + BONUS_QUATTRO;
    }

    //Scienze (22-23-24-25)
    for(j=22;j<25;j++){
        if (giocatore->nArmateInTerritorio[j] >= 1){
            contaBonus++;
        }
        if (contaBonus == FULL_TERRITORIO_QUATTRO)
            giocatore->scortaArmate = giocatore->scortaArmate + BONUS_DUE;
    }
}

void finePartita(Giocatore *giocatore,int i,Nodo*mazzo){
    FILE*fp = NULL;
    char *frase;

    printf("\n\t%s Vince la partita!\n\n",giocatore[i].nome);

    scriviSuLogger(fp,VITTORIA,&giocatore[i]);
    scriviStatistica(fp,giocatore);

    free(giocatore);
    free(mazzo);

    /*Reindirizzamento al menu principale*/
    menuPrincipale();
}

/**
 * Questa funzione seleziona il numero di giocatori partecipanti
 * @return un valore intero (da 3 a 6)
 */
int selezione_numero_giocatori(){

    printf("Seleziona il numero di giocatori (da 3 a 6)\n");
    do{
        printf("\n:>");
        scanf("%d",&selezione);
        if(selezione<MIN_GIOCATORI || selezione>MAX_GIOCATORI)
            printf("errore nella selezione del numero dei giocatori\nInserisci nuovamente:");
        getchar();
    } while (selezione<MIN_GIOCATORI || selezione>MAX_GIOCATORI);

    return selezione;
}


//Questa funzione restituisce il primo giocatore qualora il numero di giocatori sia minore del massimo
Giocatore* riempiGiocatore(Giocatore *giocatore){
    giocatore=(Giocatore*)malloc(selezione * sizeof(Giocatore));
    return giocatore;
}

/**
 * Questa funzione seleziona casualmente un giocatore che sara' poi il primo ad iniziare il turno
 * @param giocatore
 * @return intero (da 0 a selezione)
 */
int scegliCasualmente(Giocatore *giocatore){
    int aux, giocatoreSelezionato;

    giocatoreSelezionato=selezione_numero_giocatori();
    //scelgo (3 - 4 - 5 - 6)

    aux=rand()%(giocatoreSelezionato);

    return aux;
}

/**
 * Questa funzione premette di scegliere vari campi come il nome, colore scelto e chi deve iniziare la partita
 * @param giocatore nuovo vettore dinamico di tipo 'Struct' Giocatore
 * @param paux giocatore iniziale (ID)
 * @return puntatore a Giocatore
 */
Giocatore* preparazione(Giocatore *giocatore,int *paux) {
    int aux,i;

    aux = scegliCasualmente(giocatore);
    *paux=aux;
    giocatore = riempiGiocatore(giocatore);
    InserimentoNomiG(giocatore);

    if(aux!=0){
        for(i=aux;i<selezione;i++) {
            giocatore[i].idGiocatore = i;
        }
        for (i=0;i<aux;i++){
            giocatore[i].idGiocatore = i;
        }
    } else {
        for (i=0;i<selezione;i++){
            giocatore[i].idGiocatore = i;
        }
    }

    for(i=0;i<selezione;i++) {

        printf("Giocatore %d  -  %s ... ready!\n", giocatore[i].idGiocatore, giocatore[i].nome);

    }

    switch (selezione){
        case 3:
            for(i=0;i<selezione;i++){
                giocatore[i].scortaArmate = ARMATE_IN3;
                giocatore[i].maxArmate = ARMATE_IN3;
            }
            break;
        case 4:
            for(i=0;i<selezione;i++){
                giocatore[i].scortaArmate = ARMATE_IN4;
                giocatore[i].maxArmate = ARMATE_IN4;
            }
            break;
        case 5:
            for(i=0;i<selezione;i++){
                giocatore[i].scortaArmate = ARMATE_IN5;
                giocatore[i].maxArmate = ARMATE_IN5;
            }
            break;
        default:
            for(i=0;i<selezione;i++){
                giocatore[i].scortaArmate = ARMATE_IN6;
                giocatore[i].maxArmate = ARMATE_IN6;
            }
            break;
    }

    return giocatore;
}

/**
 * questa procedura permette di far inserire dall'utente i nomi dei giocatori partecipanti
 * @param giocatore puntatore vettore dinamico di giocatori
 */
void InserimentoNomiG(Giocatore* giocatore){
    int i;

    for(i=0;i<selezione;i++){
        printf("\nInserisci il nome del giocatore %d: \n:>", i);
        setNomeGiocatori(giocatore,i);
    }
}

/**
 * questa procedura permette di far inserire dall'utente i nomi dei giocatori partecipanti
 * @param giocatore
 * @param i
 */
void setNomeGiocatori(Giocatore* giocatore,int i){

    scanf("%[^\n]s",giocatore[i].nome);
    getchar();

    }

/**
 * Questa funzione permette di far scegliere il colore dei giocatori all'utente
 * @param giocatore
 * @param i
 * @return valore intero,
 * se 1 errore
 * altrimenti tutti hanno selezionato il colore giusto
 */
int scegliColore(Giocatore* giocatore, int i) {
    ColoreGiocatori colore;
    int index, *intBufferLog;
    FILE *fp = NULL;
    char *bufferLog;

    printf("\nScegli un colore \n(0-Rosso, 1-Nero, 2-Viola, 3-Verde, 4-Giallo, 5-Blu)\n");

    for (index = i; index < selezione; index++) {
        printf("\n%s scegli: \n:>", giocatore[index].nome);
        scanf("%d", &colore);
        giocatore[index].colore = colore;
    }
    if (i != 0) {
        for (index = 0; index < i; index++) {
            printf("\n%s scegli: \n:>", giocatore[index].nome);
            scanf("%d", &colore);
            giocatore[index].colore = colore;
        }
    }

    for (index = 0; index < selezione - 1; index++) {
        if (giocatore[index].colore == giocatore[index + 1].colore) {
            printf("\nErrore, due giocatori hanno selezionato lo stesso colore\n");
            return 1;
        }
    }

    updateLoggerACapo(fp);

    //nel caso tutti i giocatori scelgano colori diversi mando nel log file l'output
    for (index = 0; index < selezione; index++) {

        scriviSuLogger(fp,COLORE,&giocatore[index]);
    }

        return 0;

}

/**
 * Funzione che simula il lancio di un dado a 6 facce
 * @return intero generato casualmente
 */
int tiroDado(){
    return MIN_DADO+rand()%(MAX_DADO-MIN_DADO+1);
}

/**
 * Questa funzione conta il numero delle carte in un determinato mazzo
 * @param mazzo
 * @return intero contatore
 */
int contaNCarteMazzo(Nodo*mazzo){
    Nodo* it;
    it=(Nodo*)malloc(sizeof(Nodo));
    it->testa = mazzo->testa;
    int conta = 0;

    while(it->testa!=NULL){
        conta++;
        it->testa = it->testa->next;
    }

    return conta;
}

/**
 * Aggiunge i Jolly al mazzo
 * @param mazzo lista
 */
void aggiungiJolly(Nodo*mazzo){
    Informazione n1,n2;
    n1.nTerritorio = ID_JOLLY1;
    n2.nTerritorio = ID_JOLLY2;

    inserisciCarta(mazzo,n1,jolly);
    inserisciCarta(mazzo,n2,jolly);

}

/**
 * Funzione che, dato in ingresso il param mazzo (lista), assegna i valori della lista in un array statico
 * e effettua il mischia carte generando randomicamente l'indice, spostando di conseguenza solo i valori
 * delle carte e non i puntatori
 * @param mazzo
 * @param dim dimensione array statico
 */
void shuffleCarte(Nodo * mazzo,int dim){
    int i,j,aux;
    int array[dim];
    Informazione arrayInformazione[dim],auxInf;

    Carta *it;

    it=mazzo->testa;

     for(i=0;i<dim;i++){
        array[i]=it->seme;
        arrayInformazione[i]=it->IDcarta;
        it=it->next;
     } //creo due vettori che prendono rispettivamente il seme e l'ID della carta, assicurandomi che a[ix];a[iy] ix=iy

     for(i=dim-1;i>1;i--){
        j=rand()%dim;

        aux=array[j];
        array[j]=array[i];
        array[i]=aux;

        auxInf=arrayInformazione[j];
        arrayInformazione[j]=arrayInformazione[i];
        arrayInformazione[i]=auxInf;

     }

    it=mazzo->testa;
    for(i=0;i<dim;i++){
        it->seme=array[i];
        it->IDcarta=arrayInformazione[i];
        it=it->next;
    }

     /* for i from n−1 downto 1 do
     j ← random integer such that 0 ≤ j ≤ i
     exchange a[j] and a[i]
      */
}

/**
 * Questa procedura da in output il mazzo del giocatore
 * @param giocatore
 */
void stampaMazzoGioc(Giocatore*giocatore){
    Nodo*it;
    char*str;
    int j=0;
    it=(Nodo*)malloc(sizeof(Nodo));

    if(giocatore->mazzoGiocatore.testa!=NULL){
        it->testa = giocatore->mazzoGiocatore.testa;
        printf("\n");
        while(it->testa!=NULL){
            str = getNomeTerritorio(it->testa->IDcarta.nTerritorio);
            printf("%d '%s' ",it->testa->IDcarta.nTerritorio,str);
            it->testa = it->testa->next;
            j++;
        }
        printf("\n");
    } else {
        printf("\nNessuna carta presente!");
    }

    free(it);
}

/**
 * Questa procedura permette di trasferire la prima carta del mazzo (testa) nella prima posizione della testa del mazzo del giocatore
 * @param giocatore vettore giocatori dinamico
 * @param mazzo pubblico
 */
void pescaCarta(Nodo* giocatore,Nodo *mazzo) {
    int i;
    char *str;

    if (mazzo->testa == NULL){
        printf("\nMazzo vuoto");
        return;
    }

    Carta* nuovoNodo = (Carta*)malloc(sizeof(Carta));

    nuovoNodo->IDcarta = mazzo->testa->IDcarta;
    nuovoNodo->seme = mazzo->testa->seme;

    //nuovoNodo->next = mazzo->testa;
    nuovoNodo->next = giocatore->testa;
    nuovoNodo->prev = NULL;

    if (giocatore->testa != NULL){
        giocatore->testa->prev = nuovoNodo;
    }

    giocatore->testa = nuovoNodo;

    str = getNomeTerritorio(nuovoNodo->IDcarta.nTerritorio);
    printf("\nHai pescato %d '%s'",nuovoNodo->IDcarta.nTerritorio,str);

    mazzo->testa = mazzo->testa->next;

}

/**
 * assegna i territori iniziali ai giocatori partecipanti
 * @param giocatore
 * @param i indice
 * @param it iteratore
 */
void territoriIniziali(Giocatore* giocatore,int i,Nodo *it){
    int idTerritorio;
    FILE*fp=NULL;
    char*str;

    if(it->testa==NULL)
        printf("\n- ");
    else{
        idTerritorio = it->testa->IDcarta.nTerritorio;
        giocatore[i].nArmateInTerritorio[idTerritorio] = 1;
        str=getNomeTerritorio(idTerritorio);
        scriviSuLogger(fp,PESCA,&giocatore[i]);
        printf("\n%s occupa il territorio: %d '%s'",giocatore[i].nome,idTerritorio,str);
        scriviPosizionamentoSuFile(fp,&giocatore[i],idTerritorio);
        giocatore[i].scortaArmate--;
        it->testa = it->testa->next;
    }
}

void eliminaGiocatore(Giocatore*giocSconfitto){
    FILE* fp = NULL;
    scriviEliminazione(fp, giocSconfitto);
    giocSconfitto->status = false;

    //passaggio mazzo
    //giocSconfitto->mazzoGiocatore.coda = giocatore[i].mazzoGiocatore.testa;
}

void riposizionaCarte(Giocatore*giocatore,int index,Nodo *mazzo){
    int i,j;

    for(i=index;i<selezione;i++){
        for(j=0;j<N_TERRITORI;j++){
            if(giocatore[i].mazzoGiocatore.testa==NULL)
                return;
            mazzo->testa = giocatore[i].mazzoGiocatore.testa;
            giocatore[i].mazzoGiocatore.testa = giocatore[i].mazzoGiocatore.testa->next;
            mazzo->testa=mazzo->testa->next;
        }
    }
    if (index != 0) {
        for (i = 0; i<index; i++) {
            for (j = 0; j < N_TERRITORI; j++) {
                if(giocatore[i].mazzoGiocatore.testa==NULL)
                    return;
                mazzo->testa = giocatore[i].mazzoGiocatore.testa;
                giocatore[i].mazzoGiocatore.testa = giocatore[i].mazzoGiocatore.testa->next;
                mazzo->testa=mazzo->testa->next;
            }
        }
    }
}

void inizializzaMazzoGioc(Giocatore*giocatore){
    int i,j;

    for(i=0;i<selezione;i++){
        giocatore[i].mazzoGiocatore.testa=NULL;
        giocatore[i].mazzoGiocatore.coda=NULL;
        giocatore[i].status = true;
        for(j=0;j<N_TERRITORI;j++){
            giocatore[i].nArmateInTerritorio[j]=0;
        }
    }
}

/**
 * Questa funzione permette all'utente di inserire le armate di rinforzo nel territorio
 * @param giocatore
 * @param i
 */
void riempiTerritori(Giocatore*giocatore,int i) {
    int j, counter, nRinforzi;
    counter = 3;
    char *str;

    if(giocatore[i].scortaArmate < 3)
        counter = giocatore[i].scortaArmate;

    if(giocatore[i].scortaArmate > 0){

        printf("\n%s puoi inserire armate in questi territori: ", giocatore[i].nome);
        for (j = 0; j < N_TERRITORI; j++) {
            if (giocatore[i].nArmateInTerritorio[j] >= 1) {
                str = getNomeTerritorio(j);
                printf("\nID:%d '%s'", j, str);
            }
        }
        printf("\n");

            while (counter > 0) {
                printf("\nArmate rimaste: %d\nArmate da piazzare: %d",giocatore[i].scortaArmate,counter);
                printf("\nInserisci l'ID territorio da rinforzare: ");
                printf("\n:>");
                scanf("%d", &j);

                if (giocatore[i].nArmateInTerritorio[j] < 1)
                    printf("\nNon possiedi questo territorio! ");
                else {
                    printf("\nQuanti ne vuoi inserire? MAX: %d", counter);
                    do {
                        printf("\n:>");
                        scanf("%d", &nRinforzi);

                        if(nRinforzi > counter){
                            printf("\nQuesto e' ILLEGALE");
                        }
                    } while (nRinforzi > counter || nRinforzi < 0);
                    counter = counter - nRinforzi;
                    giocatore[i].scortaArmate = giocatore[i].scortaArmate - nRinforzi;
                    giocatore[i].nArmateInTerritorio[j] = giocatore[i].nArmateInTerritorio[j] + nRinforzi;
                }
            }
    } else {
        printf("\n%s ha finito la fase di rinforzo iniziale!",giocatore[i].nome);
    }
}

/**
 * Questa funzione restituisce il nome del territorio corrispondente all'indice fornito in ingresso
 * @param nTerritorio
 * @return stringa di caratteri corrispondente al nome del territorio
 */
char* getNomeTerritorio(int nTerritorio){
    char*str;

    switch (nTerritorio){
        case 0:
            str="Psicologia\0";
            return str;
        case 1:
            str="Filosofia\0";
            return str;
        case 2:
            str="Pedagogia\0";
            return str;
        case 3:
            str="Lettere\0";
            return str;
        case 4:
            str="Lingue\0";
            return str;
        case 5:
            str="Economia\0";
            return str;
        case 6:
            str="Scienze_Politiche\0";
            return str;
        case 7:
            str="Giurisprudenza\0";
            return str;
        case 8:
            str="Odontoiatria\0";
            return str;
        case 9:
            str="Infermieristica\0";
            return str;
        case 10:
            str="Medicina\0";
            return str;
        case 11:
            str="Scienze_Motorie\0";
            return str;
        case 12:
            str="CTF\0";
            return str;
        case 13:
            str="Tossicologia\0";
            return str;
        case 14:
            str="Biologia\0";
            return str;
        case 15:
            str="Scienze_della_Natura\0";
            return str;
        case 16:
            str="Farmacia\0";
            return str;
        case 17:
            str="Ing_Chimica\0";
            return str;
        case 18:
            str="Ing_Meccanica\0";
            return str;
        case 19:
            str="Ing_Elettronica\0";
            return str;
        case 20:
            str="Architettura\0";
            return str;
        case 21:
            str="Ing_Civile\0";
            return str;
        case 22:
            str="Chimica\0";
            return str;
        case 23:
            str="Fisica\0";
            return str;
        case 24:
            str="Informatica\0";
            return str;
        case 25:
            str="Matematica\0";
            return str;
        default:
            printf("\nErrore Nome Territorio");
            return 0;
    }
}

char* getColore(ColoreGiocatori colore){
    char* str;
    switch (colore){
        case rosso:
            str="ROSSO\0";
            return str;
        case nero:
            str="NERO\0";
            return str;
        case viola:
            str="VIOLA\0";
            return str;
        case verde:
            str="VERDE\0";
            return str;
        case giallo:
            str="GIALLO\0";
            return str;
        case blu:
            str="BLU\0";
            return str;
    }
}