#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_FILOSOFI 5

pthread_t filosofi[NUM_FILOSOFI];
int haPresoLaForchetta[NUM_FILOSOFI];
int pos;

void schermoIniziale();
void inizializza(int arrayDiBool[NUM_FILOSOFI]);
void stampaStato(int pos, char* messaggio);
void *prendiForchetta(void* numeroForchette);
void creaThread(pthread_t filosofi[NUM_FILOSOFI],int* numeroForchette);
void signal(int* s);
void wait(int* s);
void inizializza(int haMangiato[NUM_FILOSOFI]);

int main(){
    int numeroForchette = NUM_FILOSOFI;
    schermoIniziale();
    inizializza(haPresoLaForchetta);
    int continua = 1;
    while(1){
        creaThread(filosofi,&numeroForchette);
    }
    return 0;
}

void inizializza(int arrayDiBool[NUM_FILOSOFI]){
    for(int i = 0; i < NUM_FILOSOFI; i++){
        arrayDiBool[i] = 0;
    }
}


void *prendiForchetta(void* numeroForchette){
    int n;
    int aspetta, segnale;
    n = NUM_FILOSOFI/2;
    pthread_join((pthread_t)filosofi[pos],0);
    
    if(*(int*)numeroForchette >= n && haPresoLaForchetta[pos] == 0){
        
        wait(*(int*)numeroForchette);
        haPresoLaForchetta[pos] = 1;
        stampaStato(pos, "HA PRESO LA FORCHETTA");
        printf("Numero forchette: %d\n", *(int*)numeroForchette);
        pthread_join(filosofi[pos],0);
        pthread_exit(0);
        
    } else if(haPresoLaForchetta[pos] == 1 && *(int*)numeroForchette < n){
        stampaStato(pos,"STA MANGIANDO");
        haPresoLaForchetta[pos] = 0;
        
        signal(*(int*)numeroForchette);
        printf("Numero forchette: %d\n", *(int*)numeroForchette);
        pthread_exit(0);
    } else if(haPresoLaForchetta[pos] == 0 && *(int*)numeroForchette < n){
        stampaStato(pos,"STA PENSANDO");
        pthread_exit(0);
    }
    
    pthread_exit(0);
}

void creaThread(pthread_t filosofi[NUM_FILOSOFI], int* numeroForchette) {
    int pos;
    for (pos = 0; pos < NUM_FILOSOFI; pos++) {
        usleep(0.00001);
        pthread_create(&filosofi[pos], NULL, prendiForchetta, (void*)numeroForchette);
        pthread_join(filosofi[pos], NULL);
    }
}

void wait(int* s) {
    while (*s < 0)
        pthread_exit(NULL);
    (*s)--;
}

void signal(int* s) {
    (*s)++;
}

void stampaStato(int pos, char* messaggio) {
    printf("FILOSOFO: %d %s\n", pos + 1, messaggio);
}

void schermoIniziale() {
    printf("+-------------------------BENVENUTO--------------------------+\n");
    printf("|Quest'applicazione gestisce il problema dei cinque filosofi.|\n");
    printf("+------------------------------------------------------------+\n");
}