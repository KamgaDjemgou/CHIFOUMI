#include "Interface.h"

//Fonction pour initialiser la SDL et le TTF(pour les polices)
void initSDLAndTTF()
{
     if (SDL_Init(SDL_INIT_VIDEO) != 0)
     {
        SDL_ExitWithError("Impossible d'initialiser la SDL");
     }

     if(TTF_Init() != 0){
        SDL_ExitWithError("Impossible d'initialiser la TTF");
     }
}

//Fonction pour initialiser la connection avec le serveur
void initConnexion()
{
    if(connexionServeur(&gameManager->socket) < 0)
    {
        SDL_ExitWithError("Impossible de connecter au serveur!.");
    }
}

//Initialiser le manager de l'interface
void initGameManager()
{

    gameManager = (GameManager*)malloc(sizeof(GameManager));

    // Création de la fenêtre du jeu
    gameManager->window = SDL_CreateWindow("Bienvenue sur Chifoumi", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gameManager->window == NULL)
    {
        SDL_ExitWithError("Impossible de créer la fenêtre.");
    }

    //Dessiner les composants de base sur la fenêtre
    clearAndDrawBasic()
    //On met à zéro les scores des joueur
    updateScore(0, 0);

}

//Pour la gestion des erreurs
void SDL_ExitWithError(const char* message)
{
    SDL_Log("ERREUR: %s > %s\n", message, SDL_GetError());
    destroyGame();
    exit(EXIT_FAILURE);
}

//Fonction pour dessiner les composants de base
//De notre fenêtre
void clearAndDrawBasic()
{
    //On vide le rendu
    SDL_RenderClear(gameManager->renderer);
    SDL_DestroyRenderer(gameManager->renderer);

    //Création du rendu
    gameManager->renderer = SDL_CreateRenderer(gameManager->window, -1, SDL_RENDERER_SOFTWARE);
    if(gameManager->renderer == NULL)
    {
        SDL_ExitWithError("Impossible de créer le rendu");
    }

    //Coloration de la surface de l'écran
    SDL_Surface *screenSurface = SDL_GetWindowSurface(gameManager->window);
    if(screenSurface == NULL)
    {
        SDL_ExitWithError("Impossible de recupérer la surface de l'écran");
    }
    SDL_FillRect(screenSurface, NULL, 
        SDL_MapRGB(screenSurface->format, 0, 0, 160));
    SDL_FreeSurface(screenSurface);

    drawAllText()
    drawAllButtons()
    
    
    //On actualise la page pour que les modifications 
    //puissent s'afficher
    SDL_RenderPresent(gameManager->renderer);

}

//Dessiner tous les textes de base de l'écran
void drawAllText(){
    SDL_Color color = {255, 201, 14};
    drawAndPut("CHIFOUMI", 40, color, (SCREEN_WIDTH-208)/2, 0);

    drawAndPut("MOI", 40, color, 10, 100);

    drawAndPut("ADV", 40, color, (SCREEN_WIDTH-100), 100);

    color.r = 255;
    color.g = 255;
    color.b = 255;
    drawAndPut("VS", 40, color, (SCREEN_WIDTH-60)/2, 100);
}

//Dessiner tous les boutons de l'écran
void drawAllButtons(){
    //Création des boutons
    SDL_Color color;
    color.r = 245;
    color.g = 239;
    color.b = 10;
    gameManager->ciseauxBouton = createBouton(CISEAUX, color, 485, 600, 
        gameManager->renderer);

    color.r = 192;
    color.g = 192;
    color.b = 192;
    gameManager->pierreBouton = createBouton(PIERRE, color, 124, 600, 
        gameManager->renderer);

    color.r = 0;
    color.g = 255;
    color.b = 0;
    gameManager->papierBouton = createBouton(PAPIER, color, 
        304, 600, gameManager->renderer);

    if(gameManager->ciseauxBouton == NULL ||
        gameManager->papierBouton == NULL || gameManager->pierreBouton == NULL)
    {
        SDL_ExitWithError("Impossible de créer le bouton");
    }
}

//Mettre à jour les scores des joueurs
void updateScore(int monScore, int advScore)
{
    SDL_Color color = {255, 255, 255};
    char scoreText[20] = "";

    //Mon score
    sprintf(scoreText, "%d", monScore);
    drawAndPut(scoreText, 40, color, 10, 150);
    gameManager->scoreMe = monScore;

    //Score de l'adversaire
    sprintf(scoreText, "%d", advScore);
    drawAndPut(scoreText, 40, color, (SCREEN_WIDTH-100), 150);
    gameManager->scoreAdv = advScore;

    SDL_RenderPresent(gameManager->renderer);
}

//Dessiner le texte et le placer sur le rendu
void drawAndPut(const char* text, int fontSize, SDL_Color color, int x, int y)
{
    TTF_Font* font = NULL;
    font = TTF_OpenFont(TTF, fontSize);
    if(font == NULL)
    SDL_ExitWithError("Impossible de charger le font");

    Texture *texture = (Texture*)malloc(sizeof(Texture));
    texture->rect.x = x;
    texture->rect.y = y;
    if(!drawText(texture, text, color, font, gameManager->renderer))
    {
       SDL_ExitWithError("Impossible de dessiner sur la texture un texte");
    }

    TTF_CloseFont(font);
    freeTexture(texture);

}

//Boucle du jeu et gestion des événements
void gameLoop(){
    SDL_bool program_launched = SDL_TRUE;
    while(program_launched){
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.clicks >= 1)
                    {
                        //Clic sur pierre
                        if(isClicked(gameManager->pierreBouton, 
                            event.motion.x, event.motion.y))
                        {
                            traiterClic(PIERRE, IMAGE_PIERRE);
                            break;
                        }
                        //Clic sur papier
                        if(isClicked(gameManager->papierBouton, 
                            event.motion.x, event.motion.y))
                        {
                            traiterClic(PAPIER, IMAGE_PAPIER);
                            break;
                        }

                        //Clic sur ciseaux
                        if(isClicked(gameManager->ciseauxBouton, 
                            event.motion.x, event.motion.y))
                        {
                            traiterClic(CISEAUX, IMAGE_CISEAUX);
                            break;
                        }
                     }
                    break;
                case SDL_QUIT:
                    //Lorsque le jouer ferme la fenêtre
                    envoyerMessage(gameManager->socket, QUITTER);
                    program_launched = SDL_FALSE;
                    break;
                default:
                    break;
            }

        }

    }
}

//Traitement lorsque le joueur fait son choix
void traiterClic(const char* choix, const char* imagePath)
{
    //On efface l'écran pour actualiser les 
    //informations en fonction du choix du joueur
    clearAndDrawBasic();
    createBoutonWithImage(imagePath, 10, 300, gameManager->renderer);
    updateScore(gameManager->scoreMe, gameManager->scoreAdv);

    //On met tous les boutons inactifs pour éviter
    //qu'on y appuie encore
    setAllDisable();

    //On envoie au serveur le choix
    envoyerMessage(gameManager->socket,choix);

    //Attendre la reponse du serveur et la traiter en fonction
    traiterReceptionServeur(imagePath);

}

//Traiter la reception du serveur
void traiterReceptionServeur(const char* monChoixImage)
{
    //Reception de la reponse du serveur
    char buffer[TAILLE];

    if(recevoirMessage(gameManager->socket, buffer) < 0){
        printf("[-]Erreur dans la reception du message");
    }else{
        char jeuAdv[TAILLE];
        char result[TAILLE];

        bzero(jeuAdv, TAILLE);
        bzero(result, TAILLE);

        sscanf(buffer, "%s %s %d %d", result, jeuAdv, 
            &gameManager->scoreMe, 
            &gameManager->scoreAdv);

        //Si votre adversaire a quitté le jeu
        if(strcmp(result, FORFAIT) == 0)
        {
            SDL_Color color = {0, 255, 0};
            drawAndPut("MATCH GAGNE", 40, color, (SCREEN_WIDTH-190)/2, 700);
            drawAndPut(result, 40, color, (SCREEN_WIDTH-100)/2, 400);

            updateScore(gameManager->scoreMe, gameManager->scoreAdv);
            close(gameManager->socket);
            return;
        }

        if(strcmp(jeuAdv, CISEAUX) == 0)
        {
            updateResult(monChoixImage, IMAGE_CISEAUX, result);
            
        } else if(strcmp(jeuAdv, PIERRE) == 0){

           updateResult(monChoixImage, IMAGE_PIERRE, result);

        }else if(strcmp(jeuAdv, PAPIER) == 0){

            updateResult(monChoixImage, IMAGE_PAPIER, result);

        }
    }

}

//Mettre à jour les resultats sur notre fenêtre 
//en fonction des choix des joueurs et du resultat
//envoyé par le serveur
void updateResult(const char* monChoixImage, 
    const char* choixAdvImage, const char* result)
{
    clearAndDrawBasic();
    createBoutonWithImage(monChoix, 10, 300, gameManager->renderer);
    createBoutonWithImage(choixAdv, SCREEN_WIDTH-210, 300, gameManager->renderer);
    printResultOnScreen(result);
    updateScore(gameManager->scoreMe, gameManager->scoreAdv);
    setAllAble();
}



//Mettre tous les boutons inactifs donc non cliquables
void setAllDisable()
{
   gameManager->pierreBouton->isActive = SDL_FALSE;
   gameManager->papierBouton->isActive = SDL_FALSE;
   gameManager->ciseauxBouton->isActive = SDL_FALSE;
}

//Remettre les boutons cliquables
void setAllAble()
{
   gameManager->pierreBouton->isActive = SDL_TRUE;
   gameManager->papierBouton->isActive = SDL_TRUE;
   gameManager->ciseauxBouton->isActive = SDL_TRUE;
}

//Dessiner le resultat(GAGNE, PERDU, NUL ou FORFAIT) 
void printResultOnScreen(const char* result)
{
    SDL_Color color;
    if(strcmp(result, GAGNE) == 0 || (strcmp(result, MATCH_GAGNE) == 0))
    {
       color.r = 0;
       color.g = 255;
       color.b = 0;

    }else if(strcmp(result, PERDU) == 0 || (strcmp(result,MATCH_PERDU) == 0)){
       color.r = 255;
       color.g = 0;
       color.b = 0;
    }else{
       color.r = 255;
       color.g = 255;
       color.b = 0;
    }

    if((strcmp(result, MATCH_GAGNE) == 0)){
        drawAndPut("MATCH GAGNE", 40, color, 274, 700);
        drawAndPut(GAGNE, 40, color, (SCREEN_WIDTH-100)/2, 400);
    }else if((strcmp(result,MATCH_PERDU) == 0)){
        drawAndPut("MATCH PERDU", 40, color, 274, 700);
        drawAndPut(PERDU, 40, color, (SCREEN_WIDTH-100)/2, 400);
    }else{
        drawAndPut(result, 40, color, (SCREEN_WIDTH-100)/2, 400);
    }
}

void startGame()
{
    initSDLAndTTF();
    initGameManager();
    initConnexion();
    gameLoop();
}

//Libérer l'espace mémoire
void destroyGame()
{
   SDL_DestroyRenderer(gameManager->renderer);
   SDL_DestroyWindow(gameManager->window);
   freeBouton(gameManager->ciseauxBouton);
   freeBouton(gameManager->papierBouton);
   freeBouton(gameManager->pierreBouton);
   TTF_Quit();
   SDL_Quit();
}


