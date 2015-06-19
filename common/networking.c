/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Gemeinsam verwendete Module
 *
 * netwokring.c: Implementierung der Funktionen zum Senden & Empfangen von Nachrichten
 *
 */

#include "server/user.h"
#include "networking.h"
#include "sockets.h"
#include "common/util.h"
#include "rfc.h"
#include "util.h"
#include "../client/gui/gui_interface.h"
#include "../server/user.h"

#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

/*
 * versende Nachricht ueber Socket
 * param PACKET packet Nachricht die versendet werden soll
 * int socketDeskriptor Socketdeskriptor ueber den die Nachricht versendet wird
 */
void sendPacket(PACKET packet, int socketDeskriptor) {
	size_t packetLength = sizeof(HEADER) + ntohs(packet.header.length);

	if (send(socketDeskriptor, &packet, packetLength, 0) == -1) {
		errorPrint("Senden der Daten fehlgeschlagen!");
		//exit(0);

	} else {
		// Testweise ausgeben welcher Typ an welchen Socket versendet wurde
		infoPrint("Nachicht vom Type %i an die Socket-ID: %i gesendet",
				packet.header.type, socketDeskriptor);
	}
}
/*
 * Nachricht ueber Socket empfangen
 * param int socketDeskriptor Socketdeskriptor ueber den Nachricht empfangen wird *
 */
PACKET recvPacket(int socketDeskriptor) {

	PACKET packet;
	int recv_bytes = 0;
	int packetLength = 0;

	if ((recv_bytes = recv(socketDeskriptor, &packet.header, sizeof(HEADER), MSG_WAITALL))
			== -1) {
		errorPrint("Empfangen des Headers fehlgeschlagen!");
		// Fehlerpaket zurueckschicken
		//char error_message[MAX_MESSAGE_LENGTH] = "Fehlerhafte uebertragung der Daten, Verbindung unterbrochen!";
		packet.header.type = RFC_ERRORWARNING;
		packet.content.error.subtype = ERR_FATAL;
		strncpy(packet.content.error.errormessage, "Fehlerhafte uebertragung der Daten, Verbindung unterbrochen!", strlen("Fehlerhafte uebertragung der Daten, Verbindung unterbrochen!"));
		packet.header.length = htons(strlen("Fehlerhafte uebertragung der Daten, Verbindung unterbrochen!") + 1);
		return packet;
		exit(0);
		/*
	}else if(recv_bytes == 0){
		packet.header.type = RFC_ERRORWARNING;
		packet.content.error.subtype = ERR_FATAL;
		strncpy(packet.content.error.errormessage, "Client wurde beendet", strlen("Client wurde beendet"));
	*/
	}

	packetLength = ntohs(packet.header.length);
	infoPrint("packetLength: %i", packetLength);

	if(packetLength != 0)
	{
		if ((recv_bytes = recv(socketDeskriptor, &packet.content, packetLength, MSG_WAITALL))
				== -1) {
			errorPrint("Empfangen des Datenpakets fehlgeschlagen!");
			// Fehlerpaket zurueckschicken
			//char error_message[MAX_MESSAGE_LENGTH] = "Fehlerhaftes Datenpaket";
			packet.header.type = RFC_ERRORWARNING;
			packet.content.error.subtype = ERR_FATAL;
			strncpy(packet.content.error.errormessage, "Fehlerhaftes Datenpaket", strlen("Fehlerhaftes Datenpaket"));
			packet.header.length = htons(strlen("Fehlerhaftes Datenpaket") + 1);
			return packet;
			exit(0);
		}
	}

	return packet;
}
