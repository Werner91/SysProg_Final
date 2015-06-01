/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Server
 * 
 * Gruppe_04 Stroh,Strohm,Steinbinder
 *
 * score.h: Header für den Score Agent
 */

#ifndef SCORE_H
#define SCORE_H

#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>


sem_t semaphor_score;

void* score_main();

#endif
