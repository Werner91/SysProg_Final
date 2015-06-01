/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Server
 * 
 * Gruppe_04 Stroh,Strohm,Steinbinder
 *
 * catalog.h: Header für die Katalogbehandlung und Loader-Steuerung
 */

#ifndef CATALOG_H
#define CATALOG_H

#include "common/rfc.h"

#include <stdbool.h>


// enthaelt Namen der verfuegbaren Fragenkataloge
typedef struct CatalogArray {
	char CatalogName[31];
} CATALOGARRAY;


int addCatalog(char* name, int i);
void sendCatalog(int);
void setActiveCatalog(PACKET);
PACKET getActiveCatalog();
bool isCatalogChosen();
Question* getQuestion(int);
void setShMem(char*);


#endif
