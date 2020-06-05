#include "connexionTCP.h"

void initServeur()
{
    serveur = (Serveur*)malloc(sizeof(Serveur));
    serveur->tour = 0;

    struct sockaddr_in serverAddr;
    serveur->socketServer = socket(AF_INET, SOCK_STREAM, 0);

    if(serveur->socketServer < 0)
    {
        printf("[-]Erreur de création du socket du serveur!\n");
        destroyServeur();
    }
    printf("[+]Socket du serveur a été bien créé!\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(IP_SERVEUR);

    if(bind(serveur->socketServer, (struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0)
    {
        printf("[-]Erreur dans la liaison du socket avec l'adresse\n");
        destroyServeur();
    }

    if(listen(serveur->socketServer, 3) == 0)
    {
        printf("[+]A l'écoute ...\n");
    }

}

void connexionJoueurs()
{
    int newSocket;
    struct sockaddr_in newAddr;
    socklen_t addr_size;

    newSocket = accept(serveur->socketServer, (struct sockaddr*)&newAddr,&addr_size);
    if(newSocket < 0)
    {
        printf("[-]Impossible de connecter le premier joueur\n");
    }
    printf("[+]Le joueur à l'adresse IP: %s vient de se connecter.\n",
                inet_ntoa(newAddr.sin_addr));
    serveur->joueur1.socket = newSocket;

    newSocket = accept(serveur->socketServer, (struct sockaddr*)&newAddr,&addr_size);
    if(newSocket < 0)
    {
        printf("[-]Impossible de connecter le second joueur\n");
    }
    printf("[+]Le second joueur à l'adresse IP: %s vient de se connecter.\n",
                inet_ntoa(newAddr.sin_addr));
    serveur->joueur2.socket = newSocket;
}

void loopServeur()
{
    char buffer[TAILLE];
    int tour = 0;
    while(1){
            //On vide le buffer pour eviter des erreurs
            bzero(buffer,TAILLE);

            //On recoit le message venant du client
            recv(serveur->joueur1.socket, buffer, TAILLE, 0);

            if(strcmp(buffer, QUITTER) == 0)
            {
                printf("[+]Le joueur 1 vient de se déconnecter.\n");
                close(serveur->joueur1.socket);

                bzero(buffer, TAILLE);
                sprintf(buffer, "FORFAIT %s %d %d", serveur->joueur1.choix, serveur->joueur2.score, serveur->joueur1.score);
                envoyerMessage(serveur->joueur2.socket,buffer);
                destroyServeur();
            }else {

            if(strcmp(buffer, CISEAUX) == 0 ||
               strcmp(buffer, PIERRE) == 0 ||
               strcmp(buffer, PAPIER) == 0){

                printf("[+]Le joueur 1: %s.\n", buffer);
                bzero(serveur->joueur1.choix, TAILLE);
                strcpy(serveur->joueur1.choix, buffer);
                tour++;
              }
            }

            bzero(buffer,TAILLE);
            recv(serveur->joueur2.socket, buffer, TAILLE, 0);

            if(strcmp(buffer, QUITTER) == 0)
            {
                printf("[+]Le joueur 2 vient de se déconnecter.\n");
                close(serveur->joueur2.socket);

                bzero(buffer, TAILLE);
                sprintf(buffer, "FORFAIT %s %d %d", serveur->joueur2.choix, serveur->joueur1.score, serveur->joueur2.score);
                envoyerMessage(serveur->joueur1.socket,buffer);
                destroyServeur();
            }else {
                if(strcmp(buffer, CISEAUX) == 0 ||
                    strcmp(buffer, PIERRE) == 0 ||
                    strcmp(buffer, PAPIER) == 0){

                    printf("[+]Le joueur 2: %s.\n", buffer);
                    bzero(serveur->joueur2.choix, TAILLE);
                    strcpy(serveur->joueur2.choix, buffer);
                    tour++;
                }

            }

            //Fin d'un tour
            if(tour == 2){
                traiterFinTour();
                tour = 0;
            }
    }

}

void traiterFinTour(){
    char message[TAILLE];

    //Cas de match nul
    if(strcmp(serveur->joueur1.choix, serveur->joueur2.choix) == 0){


        //Message à envoyer au gagnant
        sprintf(message, "NUL %s %d %d", serveur->joueur2.choix, serveur->joueur1.score, serveur->joueur2.score);
        envoyerMessage(serveur->joueur1.socket, message);

        //On vide la chaine
        bzero(message, TAILLE);

        //Message à envoyer au perdant
        sprintf(message, "NUL %s %d %d", serveur->joueur1.choix, serveur->joueur2.score, serveur->joueur1.score);
        envoyerMessage(serveur->joueur2.socket, message);


    }else{
        //Si le premier joueur a gagné
        if((strcmp(serveur->joueur1.choix, PIERRE)==0 && strcmp(serveur->joueur2.choix, CISEAUX)==0) ||
        (strcmp(serveur->joueur1.choix, CISEAUX)==0 && strcmp(serveur->joueur2.choix, PAPIER)==0) ||
        (strcmp(serveur->joueur1.choix, PAPIER)==0 && strcmp(serveur->joueur2.choix, PIERRE)==0)){

            serveur->joueur1.score++;
            if(serveur->joueur1.score == 10){
                //Message à envoyer au gagnant
            sprintf(message, "MATCH_GAGNE %s %d %d", serveur->joueur2.choix, serveur->joueur1.score, serveur->joueur2.score);
            envoyerMessage(serveur->joueur1.socket, message);


            //On vide la chaine
            bzero(message, TAILLE);

            //Message à envoyer au perdant
            sprintf(message, "MATCH_PERDU %s %d %d", serveur->joueur1.choix, serveur->joueur2.score, serveur->joueur1.score);
            envoyerMessage(serveur->joueur2.socket, message);

            destroyServeur();
            }else{
            //Message à envoyer au gagnant
            sprintf(message, "GAGNE %s %d %d", serveur->joueur2.choix, serveur->joueur1.score, serveur->joueur2.score);
            envoyerMessage(serveur->joueur1.socket, message);


            //On vide la chaine
            bzero(message, TAILLE);

            //Message à envoyer au perdant
            sprintf(message, "PERDU %s %d %d", serveur->joueur1.choix, serveur->joueur2.score, serveur->joueur1.score);
            envoyerMessage(serveur->joueur2.socket, message);


            }

        }else{
            serveur->joueur2.score++;

            if(serveur->joueur2.score == 10)
            {
                //Message à envoyer au gagnant
            sprintf(message, "MATCH_PERDU %s %d %d", serveur->joueur2.choix, serveur->joueur1.score, serveur->joueur2.score);
            envoyerMessage(serveur->joueur1.socket, message);

            //On vide la chaine
            bzero(message, TAILLE);

            //Message à envoyer au perdant
            sprintf(message, "MATCH_GAGNE %s %d %d", serveur->joueur1.choix, serveur->joueur2.score, serveur->joueur1.score);
            envoyerMessage(serveur->joueur2.socket, message);

            destroyServeur();

            }else{
                //Message à envoyer au gagnant
            sprintf(message, "PERDU %s %d %d", serveur->joueur2.choix, serveur->joueur1.score, serveur->joueur2.score);
            envoyerMessage(serveur->joueur1.socket, message);

            //On vide la chaine
            bzero(message, TAILLE);

            //Message à envoyer au perdant
            sprintf(message, "GAGNE %s %d %d", serveur->joueur1.choix, serveur->joueur2.score, serveur->joueur1.score);
            envoyerMessage(serveur->joueur2.socket, message);

            }



        }

    }
}

void startServeur(){
    initServeur();
    connexionJoueurs();
    loopServeur();
}

//Fonction pour envoyer un message
int envoyerMessage(int socket, const char* message)
{
    return send(socket, message, strlen(message), 0);
}

//Détruire le serveur pour libérer la mémoire
void destroyServeur()
{
    serveur = NULL;
    exit(1);
}

