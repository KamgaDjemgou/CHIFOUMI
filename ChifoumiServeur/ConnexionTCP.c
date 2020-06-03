#include "connexionTCP.h"

void initServeur()
{
    serveur = (Serveur*)malloc(sizeof(Serveur));

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
    //On limite la connection à deux: pour les deux joueurs
    if(listen(serveur->socketServer, 2) == 0)
    {
        printf("[+]A l'écoute ...\n");
    }

}

void loopServeur()
{
    int newSocket;
    struct sockaddr_in newAddr;
    socklen_t addr_size;
    char buffer[TAILLE];
    pid_t fils;

    int limit = 0;
    int tour = 0;
    while(1)
    {
        if(limit >= 0)
        {
            newSocket = accept(serveur->socketServer, (struct sockaddr*)&newAddr,&addr_size);
            if(newSocket < 0)
            {
                printf("[-]Impossible d'accepter une nouvelle connection\n");
                destroyServeur();
            }

            printf("[+]Le joueur à l'adresse IP: %s vient de se connecter.\n", 
                inet_ntoa(newAddr.sin_addr));
            Joueur joueur = serveur->joueurs[limit];
            joueur.socket = newSocket;
            joueur.score = 0;
            limit++;

            //Création d'un processus fils pour gérer la connection
            //Avec le nouveau joueur
            fils = fork();
            if(fils == 0){
                close(serveur->socketServer);
                while(1)
                {
                    //On vide le buffer pour eviter des erreurs
                    bzero(buffer,TAILLE);

                    //On recoit le message venant du client
                    recv(newSocket, buffer, TAILLE, 0);
                    
                    //Lorsque un client se déconnecte
                    if(strcmp(buffer, QUITTER) == 0)
                    {
                      printf("[+]Le joueur à l'adresse IP: %s vient de se déconnecter.\n", 
                        inet_ntoa(newAddr.sin_addr));
                      limit = 0;
                      //finJeu();
                      break;
                    }

                    if(strcmp(buffer, CISEAUX) == 0 || 
                        strcmp(buffer, PIERRE) == 0 || 
                        strcmp(buffer, PAPIER) == 0){

                      printf("[+]Le joueur à l'adresse IP: %s.\n", buffer);
                      joueur.choix = buffer;
                      tour++;

                      //Lorsque les deux joueurs ont envoyés leur choix
                      if(tour == NBRE_JOUEURS){
                        traiterFinTour();
                        tour = 0;
                      }

                      continue;
                    }
                }
            }
        }

    }
    close(newSocket);
}

void traiterFinTour(){
    char message[TAILLE];

    //On vide la chaine
    bzero(message, TAILLE);

    //Cas de match nul
    if(strcmp(serveur->joueurs[0].choix, serveur->joueurs[1].choix) == 0){

        sprintf(message, "NUL|%s", serveur->joueurs[0].choix);
        envoyerMessage(serveur->joueurs[0].socket, message);
        envoyerMessage(serveur->joueurs[1].socket, message);
    }else{
        //Si le premier joueur a gagné
        if((strcmp(serveur->joueurs[0].choix, PIERRE)==0 && strcmp(serveur->joueurs[1].choix, CISEAUX)==0) ||
        (strcmp(serveur->joueurs[0].choix, CISEAUX)==0 && strcmp(serveur->joueurs[1].choix, PAPIER)==0) ||
        (strcmp(serveur->joueurs[0].choix, PAPIER)==0 && strcmp(serveur->joueurs[1].choix, PIERRE)==0)){
            serveur->joueurs[0].score++;

            //Message à envoyer au gagnant
            sprintf(message, "GAGNE|%s|%d", serveur->joueurs[1].choix, serveur->joueurs[0].score);
            envoyerMessage(serveur->joueurs[0].socket, message);

            //On vide la chaine
            bzero(message, TAILLE);

            //Message à envoyer au perdant
            sprintf(message, "PERDU|%s|%d", serveur->joueurs[0].choix, serveur->joueurs[0].score);
            envoyerMessage(serveur->joueurs[1].socket, message);
        }else{
            serveur->joueurs[1].score++;

            //Message à envoyer au gagnant
            sprintf(message, "GAGNE|%s|%d", serveur->joueurs[0].choix, serveur->joueurs[1].score);
            envoyerMessage(serveur->joueurs[1].socket, message);

            //On vide la chaine
            bzero(message, TAILLE);

            //Message à envoyer au perdant
            sprintf(message, "PERDU|%s|%d", serveur->joueurs[1].choix, serveur->joueurs[1].score);
            envoyerMessage(serveur->joueurs[0].socket, message);

        }

    }
}

void startServeur(){
    initServeur();
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

