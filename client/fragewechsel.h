/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Client
 *
 * Gruppe_04 Stroh,Strohm,Steinbinder
 * 
 * fragewechsel.h: Header für den Fragewechsel-Thread
 */

#ifndef FRAGEWECHSEL_H
#define FRAGEWECHSEL_H

#include "../common/rfc.h"
#include <semaphore.h>

sem_t sem_frage;

void *fragewechsel_main(int*);

#endif
