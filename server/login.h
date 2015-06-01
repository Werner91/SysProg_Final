/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Server
 * 
 * Gruppe_04 Stroh,Strohm,Steinbinder
 *
 * login.h: Header für das Login
 */

#ifndef LOGIN_H
#define LOGIN_H

#include <stdbool.h>

void* login_main(int);
void setGameRunning();
bool getGameRunning();

#endif
