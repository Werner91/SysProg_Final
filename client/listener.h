/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Client
 *
 * Gruppe_04 Stroh,Strohm,Steinbinder
 * 
 * listener.h: Header für den Listener-Thread
 */

#ifndef LISTENER_H
#define LISTENER_H

#include "common/rfc.h"


// Spieler - ID + Name
typedef struct {
	int id;
	char name[PLAYER_NAME_LENGTH];
} SPIELER;


void receivePlayerlist(PACKET);
void receiveCatalogList(PACKET);
void receiveCatalogChange(PACKET);
int receiveErrorMessage(PACKET);
void questionRequest(int);
void *listener_main(int *);

#endif
