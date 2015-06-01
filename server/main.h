/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Server
 *
 * Gruppe_04 Stroh,Strohm,Steinbinder
 *
 * main.h: Hauptprogramm des Servers
 */

#ifndef MAIN_H_
#define MAIN_H_


void set_port(char*);
int get_port();
void show_help();
void process_commands(int, char**);
void setSingleInstance(int);
void closeSingleInstance(int);
void endServer();
void INThandler(int);

int startLoader();
int loadCatalogs();
void loadQuestions(char*);

#endif /* MAIN_H_ */
