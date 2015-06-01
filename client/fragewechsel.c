/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Client
 *
 * Gruppe_04 Stroh,Strohm,Steinbinder
 * 
 * fragewechsel.c: Implementierung des Fragewechsel-Threads
 */

#include "fragewechsel.h"
#include "listener.h"
#include "common/util.h"

#include <unistd.h>


 //Thread fordert Fragen vom Server an

void *fragewechsel_main(int *_socket) {
    while(1){
        // warte auf Freigabe
        sem_wait(&sem_frage);
        sleep(3);
        // Frage vom Server anfordern
        questionRequest(*_socket);
    }
}
