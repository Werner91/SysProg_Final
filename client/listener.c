/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Client
 *
 * Gruppe_04 Stroh,Strohm,Steinbinder
 * 
 * listener.c: Implementierung des Listener-Threads
 */

#include "listener.h"
#include "gui/gui_interface.h"
#include "common/rfc.h"
#include "common/sockets.h"
#include "common/networking.h"
#include "main.h"
#include "fragewechsel.h"
#include "../common/rfc.h"

#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h>


int clientID;
bool game_is_running = false;




/*
 * Funktion wertet empfangen Katalog aus und
 * fuegt diesen der GUI hinzu
 */
void receiveCatalogList(PACKET _packet) {
    // Antwort auswerten und anzeigen
    if(ntohs(_packet.header.length) > 0){
        infoPrint("%s", _packet.content.catalogname);
    	char buffer_array[ntohs(_packet.header.length)];
	    strncpy(buffer_array, _packet.content.catalogname,ntohs(_packet.header.length));
	    buffer_array[ntohs(_packet.header.length)] = '\0';
	    preparation_addCatalog(buffer_array);
    }
}






/*
 * Funktion wertet die Liste mit den Spielern aus und aktualisiert
 * die Spielerliste in der GUI
 */
void receivePlayerlist(PACKET _packet){

	SPIELER userlist[MAX_PLAYERS];
	int spielerzahl = 0;

	spielerzahl = ntohs(_packet.header.length)/37; // 37 == Groesse PLAYERLIST 1 Spieler
	infoPrint("Anzahl Spieler in der Playerlist: %i",spielerzahl);

	// Playerlist leeren (GUI)
	preparation_clearPlayers();

	/*
	for (int i = 1; i <= spielerzahl; i++) { // i = reihenfolge der Spieler
		game_setPlayerName(i, ""); //Schreibt Namen in die GUI
		game_setPlayerScore(i, 0);
	}
	*/

	if (game_is_running && spielerzahl < 2) {
		guiShowMessageDialog("Zu wenig Spieler! Spiel wird beendet!", 1); //1=gui main geht nach Bestätigen zum Aufrufer zurück
		pthread_exit(0);
	}

	for(int i = 0; i< spielerzahl; i++){
		// schreibe Spieler_ID in Spielerliste
		userlist[i].id = _packet.content.playerlist[i].id;
		// mehr als 4 Spieler? -> Fehlermeldung
		printf("Spielerliste: %s\n",_packet.content.playerlist[i].playername );
		if(i > MAX_PLAYERS){
			infoPrint("Maximale Anzahl an Spieler erreicht!");
			exit(1);
		}

		// kopiere Name aus Paket in Spielerliste
		strncpy(userlist[i].name, _packet.content.playerlist[i].playername,PLAYER_NAME_LENGTH);

		// Ausgabe der angemeldeten Spielernamen
		infoPrint("%s ist angemeldet", userlist[i].name);
		preparation_addPlayer(userlist[i].name); //Füge namen zur GUI hinzu

		game_setPlayerName(i + 1, _packet.content.playerlist[i].playername);
		game_setPlayerScore(i + 1, ntohl(_packet.content.playerlist[i].score));
		if ((clientID == userlist[i].id)) {
			game_highlightPlayer(i+1);
			infoPrint("clientID %i trifft zu, hebe den Namen des Spielers hervor", clientID);
		}
	}
}







/*
 * Funktion wertet Fehlernachrichten aus
 */
int receiveErrorMessage (PACKET _packet){
	char error_message[MAX_MESSAGE_LENGTH];
	// hole Errornachricht
	strncpy (error_message, _packet.content.error.errormessage, ntohs (_packet.header.length)-1);
	// Nullterminierung
	error_message[ntohs (_packet.header.length)-1]= '\0';
	// zeige Errordialog + gebe Fehler auf Konsole aus
	// beende Client falls fataler Error
	if(_packet.content.error.errortype == ERR_SPIELLEITERLEFTGAME){
		guiShowErrorDialog(error_message, 1);
		return 1;
	} else if (_packet.content.error.errortype == ERR_FATAL) {
		guiShowErrorDialog(error_message, 1); // GUI wird beendet und Dialogbox für fehler ausgegeben
		return 1;
	} else if ((_packet.content.error.errortype == ERR_TOOFEWPLAERS) && game_is_running) {
		guiShowMessageDialog(error_message, 0);
	}

	return 0;
}






/*
 * Funktion wertet empfangen Katalog aus und
 * setzt diesen in der GUI auf aktiv
 */
void receiveCatalogChange(PACKET _packet){
  if (ntohs(_packet.header.length) > 0) {
    char buffer_array[ntohs(_packet.header.length)];
    strncpy(buffer_array, _packet.content.catalogname, ntohs(_packet.header.length));
    buffer_array[ntohs(_packet.header.length)] = '\0';
    preparation_selectCatalog(buffer_array);
  }
}





/*
 * Funktion fordert eine neue Frage vom Server an
 */
void questionRequest(int _socketDeskriptor){
	PACKET packet;
	packet.header.type = RFC_QUESTIONREQUEST;
	packet.header.length = 0;
	sendPacket(packet, _socketDeskriptor);
	infoPrint("Question Request versendet!");
}


void *listener_main(int* _sockDeskriptor){

    // warte auf Antwort des Servers
    PACKET response = recvPacket(*_sockDeskriptor);

    // RFC_LOGINRESPONSEOK
    if(response.header.type == RFC_LOGINRESPONSEOK){
        infoPrint("Login Response Ok");
        clientID = response.content.loginresponseok.clientid;

        infoPrint("Verfuegbare Kataloge anfordern");
        catalogRequest();
    }
	// RFC_ERRORWARNING
    else if(response.header.type == RFC_ERRORWARNING){
        char message[(ntohs(response.header.length))];
        strncpy(message, response.content.error.errormessage,ntohs(response.header.length) - 1);
        // Nullterminierung des Arrays
        message[ntohs(response.header.length) - 1] = '\0';
        // zeige in GUI Fehlermeldung an
        infoPrint("Fehler: %s\n", message);
        guiShowErrorDialog(message, response.content.error.errortype);
        guiQuit();
        exit(0);
    }
	// Verbindung verloren
    else {
        // zeige in GUI Fehlermeldung an
        infoPrint("Verbindung zum Server verloren!");
        guiShowErrorDialog("Verbindung zum Server verloren!", response.content.error.errortype);
        guiQuit();
        exit(0);
    }

    //An GUI mitteilen ob der Spieler Spielleiter ist
    if(clientID == 0){
        preparation_setMode(PREPARATION_MODE_PRIVILEGED);
    }
    else {
        preparation_setMode(PREPARATION_MODE_NORMAL);
    }






    // Empfangsschleife
	int stop = 0;
	while(stop == 0){
		PACKET packet = recvPacket(*_sockDeskriptor); //holt Packet aus Server
		switch (packet.header.type){
            // RFC_CATALOGRESPONSE
            case RFC_CATALOGRESPONSE:
                receiveCatalogList(packet);
                infoPrint("Catalogresponse erhalten");
                break;
            // RFC_CATALOGCHANGE
            case RFC_CATALOGCHANGE:
                receiveCatalogChange(packet);
                infoPrint("Catalogchange erhalten");
                break;
			// RFC_PLAYERLIST
			case RFC_PLAYERLIST:
				receivePlayerlist(packet);
				infoPrint("Spielerliste erhalten");
				break;
			// RFC_STARTGAME
			case RFC_STARTGAME:
				infoPrint("Spiel gestartet!");
				game_is_running = true;
				// hole Frage
				questionRequest(*_sockDeskriptor);

				// Vorbereitungsfenster ausblenden und Spielfenster anzeigen
				game_showWindow();
				preparation_hideWindow();
				break;
			//RFC_QUESTION
			case RFC_QUESTION:
				infoPrint("Question erhalten");

				char msg_arr[50];
				if (packet.header.length != 0) {
					game_clearAnswerMarksAndHighlights();
					game_setQuestion(packet.content.question.question);
					for (int i = 0; i <= 3; i++) {
						game_setAnswer(i, packet.content.question.answers[i]);
					}
					game_setControlsEnabled(1);
					sprintf(msg_arr, "Sie haben %i Sekunden Zeit\n",packet.content.question.timeout);
					game_setStatusIcon(0);

				} else if (ntohs(packet.header.length) == 0) {
					sprintf(msg_arr, "Alle Fragen beantwortet, bitte Warten.\n");
					game_setStatusText(msg_arr);
					game_setStatusIcon(0);
				}

				game_setStatusText(msg_arr);
				break;
			case RFC_QUESTIONRESULT:
				infoPrint("Frageauswertung erhalten!");

				game_setControlsEnabled(0);

				if (ntohs(packet.header.length) > 0) {
					infoPrint("Korrekte Antwort: %i", packet.content.questionresult.correct);
					infoPrint("Spieler Antowrt: %i", packet.content.questionresult.timeout);
					uint8_t playerSelection = getAnswerSelection();
					for (int i = 0; i < NUM_ANSWERS; i++) {
						if (packet.content.questionresult.correct & (1 << i)) { // schiffte 1 um i nach links

							game_markAnswerCorrect(i);
							if(!(playerSelection & (1 << i))){
								game_highlightMistake(i);
							}

						} else {

							game_markAnswerWrong(i);
							if((playerSelection & (1 << i))){ //playerSelection gibt bitweise zurück welche antworten angeklickt wurden
								game_highlightMistake(i);
							}
						}
					}

					if (packet.content.questionresult.timeout != 0) {
						game_setStatusText("Zeit vorbei");
						game_setStatusIcon(3);
					} else if (selection != packet.content.questionresult.correct) {
						game_setStatusText("Falsch");
						game_setStatusIcon(2);
					} else {
						game_setStatusText("Richtige Antwort!");
						game_setStatusIcon(1);
					}
					sem_post(&sem_frage); //fragesemaphor aufrufen
				}

				break;
			case RFC_GAMEOVER:
				sprintf(msg_arr, "Glückwunsch, Sie wurden %i.\n", packet.content.playerrank);
				guiShowMessageDialog(msg_arr, 1); //1=gui main geht nach Bestätigen zum Aufrufer zurück

				pthread_exit(0);
				return NULL;
				break;
			// RFC_ERRORWARNING
			case RFC_ERRORWARNING:
				stop = receiveErrorMessage(packet);
				break;
			default:
				break;
			}
	}
	return 0;
}
