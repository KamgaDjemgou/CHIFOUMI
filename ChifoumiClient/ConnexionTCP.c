#include "ConnexionTCP.h"

//Fonction pour se connecter au serveur
int connexionServeur(int* clientSocket)
{
    struct sockaddr_in serverAddr;

    *clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0)return -1;

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(IP_SERVEUR);

    if(connect(*clientSocket, (struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0) return -1;

    return 0;
}

//Fonction pour envoyer un message
int envoyerMessage(int socket, const char* message)
{
	return send(socket, message, strlen(message), 0);
}

//Fonction pour recevoir un message
int recevoirMessage(int socket, char* message)
{
    char buffer[TAILLE];
    if(recv(socket, buffer, TAILLE, 0) < 0) return -1;
    strcpy(buffer, message);

    return 0;
}
