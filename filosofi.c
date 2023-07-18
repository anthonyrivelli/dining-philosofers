#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef enum { FALSE, TRUE } bool;

pthread_t* filosofi;
bool* haForchetta;
int pos;

void schermoMenu();
int leggiNumFilosofi();
void inizializzaArray(bool* arrayDiBool, int numFilosofi);
void stampaStato(int pos, char* messaggio);
void* prendiForchetta(void* numForchette);
void creaThread(pthread_t* filosofi, int* numForchette, int numFilosofi);
void signal(int* s);
void wait(int* s);


void schermoMenu() {
    printf("+-------------------------------------------------------------+\n");
    printf("|  Applicazione per gestire il problema dei filosofi a cena.  |\n");
    printf("+-------------------------------------------------------------+\n");
}

int leggiNumFilosofi(){
    int numFilosofi;
    printf("Inserisci il numero di filosofi ---> ");
    scanf("%d", &numFilosofi);
    while(numFilosofi <2){
        printf("***ERRORE Il numero dei filosofi deve essere almeno 2\n");
        printf(" Riprova ---> ");
        scanf("%d", &numFilosofi);   
    }
}

int main() {
    schermoMenu();
    int numFilosofi = leggiNumFilosofi();
    filosofi = malloc(numFilosofi * sizeof(pthread_t));
    haForchetta = malloc(numFilosofi * sizeof(bool));
    int numForchette = numFilosofi;
    inizializzaArray(haForchetta, numFilosofi);
    printf("+------------------------ESECUZIONE---------------------------+\n");
    while (TRUE) {
        creaThread(filosofi, &numForchette, numFilosofi);
    }

    //rilascio della memoria dalla malloc
    free(filosofi);
    free(haForchetta);
    return 0;
}



void inizializzaArray(bool* arrayDiBool, int numFilosofi) {
    for (int i = 0; i < numFilosofi; i++) {
        arrayDiBool[i] = FALSE;
    }
}

void* prendiForchetta(void* numForchette) {
    int aspetta, segnale;
    int num = 2;
    pthread_join(filosofi[pos], NULL);
    if (*(int*)numForchette >= num && !haForchetta[pos]) {
        wait((int*)numForchette);
        haForchetta[pos] = TRUE;
        stampaStato(pos, "HA PRESO LA FORCHETTA");
        printf("Numero forchette: %d\n", *(int*)numForchette);
        pthread_join(filosofi[pos], NULL);
        pthread_exit(NULL);
    } else if (haForchetta[pos] && *(int*)numForchette < num) {
        stampaStato(pos, "STA MANGIANDO");
        haForchetta[pos] = FALSE;
        signal((int*)numForchette);
        printf("Numero forchette: %d\n", *(int*)numForchette);
        pthread_exit(NULL);
    } else if (!haForchetta[pos] && *(int*)numForchette < num) {
        stampaStato(pos, "STA PENSANDO");
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}

void stampaStato(int pos, char* messaggio) {
    printf("FILOSOFO: %d %s\n", pos + 1, messaggio);
}

//Parte relativa ai tread e i semafori
void creaThread(pthread_t* filosofi, int* numForchette, int numFilosofi) {
    for (pos = 0; pos < numFilosofi; pos++) {
        usleep(100000);
        pthread_create(&filosofi[pos], NULL, prendiForchetta, (void*)numForchette);
        pthread_join(filosofi[pos], NULL);
    }
}

void wait(int* s) {
    while (*s < 0) {
        pthread_exit(NULL);
    }
    (*s)--;
}

void signal(int* s) {
    (*s)++;
}
