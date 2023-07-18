#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef enum {FALSE, TRUE} bool;

#define NUMERO_FILOSOFI 2

pthread_t filosofi[NUMERO_FILOSOFI];
bool haForchetta[NUMERO_FILOSOFI];
int pos;

void schermoMenu();
void inizializzaArray(bool arrayDiBool[NUMERO_FILOSOFI]);
void stampaStato(int pos,  char* messaggio);
void* prendiForchetta(void* numForchette);
void creaThread(pthread_t filosofi[NUMERO_FILOSOFI], int* numForchette);
void signal(int* s);
void wait(int* s);
void inizializzaArray(bool haMangiato[NUMERO_FILOSOFI]);

void schermoMenu() {
    printf("+-------------------------------------------------------------+\n");
    printf("|  Applicazione per gestire il problema dei filosofi a cena.  |\n");
    printf("+-------------------------------------------------------------+\n");
   

}


int numFilosofi() {
    int numero;
    
    do {
        printf("Inserisci un numero maggiore di 5: ");
        scanf("%d", &numero);
        
        if (numero <= 5) {
            printf("Il numero inserito non è maggiore di 5. Riprova.\n");
        }
    } while (numero <= 5);
    
    return numero;
}






int main() {
    int numForchette = NUMERO_FILOSOFI;
    schermoMenu();
    int numeroInserito = numFilosofi();
    printf("Il numero inserito è: %d\n", numeroInserito);
    inizializzaArray(haForchetta);
    printf("+------------------------ESECUZIONE---------------------------+\n");
    while (TRUE) {
        creaThread(filosofi, &numForchette);
    }
    return 0;
}

void inizializzaArray(bool arrayDiBool[NUMERO_FILOSOFI]) {
    for (int i = 0; i < NUMERO_FILOSOFI; i++) {
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

void stampaStato(int pos,  char* messaggio) {
    printf("FILOSOFO: %d %s\n", pos + 1, messaggio);
}


//codice relatico a tread e semafori
void creaThread(pthread_t filosofi[NUMERO_FILOSOFI], int* numForchette) {
    for (pos = 0; pos < NUMERO_FILOSOFI; pos++) {
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









