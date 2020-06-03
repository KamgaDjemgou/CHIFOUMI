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
    while(1)
    {
        if(limit != NBRE_JOUEURS)
        {
            newSocket = accept(serveur->socketServer, (struct sockaddr*)&newAddr,&addr_size);
            if(newSocket < 0)
            {
                printf("[-]Impossible d'accepter une nouvelle connection\n");
                destroyServeur();
            }

            printf("[+]Le joueur à l'adresse IP: %s vient de se connecter.\n", inet_ntoa(newAddr.sin_addr));
            Joueur joueur = serveur->joueurs[limit];
            joueur.socket = newSocket;
            joueur.score = 0;
            limit++;

            //Création d'un processus fils pour gérer la connection
            //Avec le nouveau joueur
            fils = fork();
            if(fils == 0){
                //serveur->socketServer = -1;
                while(1)
                {
                    bzero(buffer,TAILLE);
                    if(recv(newSocket, buffer, TAILLE, 0) > 0)
                    printf("Le mot recu: %s \n", buffer);

                    if(strcmp(buffer, QUITTER) == 0)
                    {
                      printf("[+]Le joueur à l'adresse IP: %s vient de se déconnecter.\n", inet_ntoa(newAddr.sin_addr));
                      limit--;
                      break;
                    }

                    if(strcmp(buffer, CISEAUX) == 0)
                    {
                      printf("[+]Le joueur à l'adresse IP: %s .\n", CISEAUX);
                      continue;
                    }

                    if(strcmp(buffer, CISEAUX) == 0)
                    {
                      printf("[+]Le joueur à l'adresse IP: %s vient de se déconnecter.\n", inet_ntoa(newAddr.sin_addr));
                      continue;
                    }

                    if(strcmp(buffer, CISEAUX) == 0)
                    {
                      printf("[+]Le joueur à l'adresse IP: %s vient de se déconnecter.\n", inet_ntoa(newAddr.sin_addr));
                      continue;
                    }
                }
            }
        }

    }
}

void startServeur()
{
    initServeur();
    loopServeur();
}

//Détruire le serveur pour libérer la mémoire
void destroyServeur()
{
    serveur = NULL;
    exit(1);
}

