/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Server
 * 
 * Gruppe_04 Stroh,Strohm,Steinbinder
 *
 * clientthread.h: Header für den Client-Thread
 */

#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <stdint.h>

void *client_thread_main(int*);
int questionTimer(uint8_t*,int,int);

#endif
