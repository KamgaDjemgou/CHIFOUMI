#include "Interface.h"
const int SCREEN_WIDTH = 800;
const int SCREEEN_HEIGHT = 800;

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
    clearAndDrawBasic();
    updateScore(0, 0);

}

void initConnexion()
{
    if(connexionServeur(gameManager->socket) < 0)
    SDL_ExitWithError("Impossible de connecter au serveur!.");
}

void clearAndDrawBasic()
{
    SDL_RenderClear(gameManager->renderer);
    SDL_DestroyRenderer(gameManager->renderer);

    gameManager->renderer = SDL_CreateRenderer(gameManager->window, -1, SDL_RENDERER_SOFTWARE);
    if(gameManager->renderer == NULL)
    {
        SDL_ExitWithError("Impossible de créer le rendu");
    }

    SDL_Surface *screenSurface = SDL_GetWindowSurface(gameManager->window);
    if(screenSurface == NULL)
    {
        SDL_ExitWithError("Impossible de recupérer la surface de l'écran");
    }
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 160));


    SDL_Color color = {255, 201, 14};
    drawAndPut("CHIFOUMI", 40, color, (SCREEN_WIDTH-208)/2, 0);
    drawAndPut("MOI", 40, color, 10, 100);
    drawAndPut("ADV", 40, color, (SCREEN_WIDTH-100), 100);
    color.g = 255;
    color.b = 255;
    drawAndPut("VS", 40, color, (SCREEN_WIDTH-60)/2, 100);

    color.r = 245;
    color.g = 239;
    color.b = 10;
    gameManager->ciseauxBouton = createBouton("CISEAUX", color, 485, 600, gameManager->renderer);

    color.r = 192;
    color.g = 192;
    color.b = 192;
    gameManager->pierreBouton = createBouton("PIERRE", color, 124, 600, gameManager->renderer);

    color.r = 0;
    color.g = 255;
    color.b = 0;
    gameManager->papierBouton = createBouton("PAPIER", color, 304, 600, gameManager->renderer);

    if(gameManager->ciseauxBouton == NULL || gameManager->papierBouton == NULL || gameManager->pierreBouton == NULL)
    {
        SDL_ExitWithError("Impossible de créer le bouton");
    }
    gameManager->quitButton = createBoutonWithImage("./resources/stop.bmp", 690, 690, gameManager->renderer);
    gameManager->startButton = createBoutonWithImage("./resources/start.bmp", 10, 690, gameManager->renderer);
    //createBoutonWithImage("./resources/papier.bmp", SCREEN_WIDTH-210, 300, gameManager->renderer);
    SDL_RenderPresent(gameManager->renderer);

}

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

void drawAndPut(const char* text, int fontSize, SDL_Color color, int x, int y)
{
    TTF_Font* font = NULL;
    font = TTF_OpenFont("./resources/Symtext.ttf", fontSize);
    if(font == NULL)
    SDL_ExitWithError("Impossible de recupérer la surface de l'écran");

    Texture *texture = (Texture*)malloc(sizeof(Texture));
    texture->rect.x = x;
    texture->rect.y = y;
    if(!drawText(texture, text, color, font, gameManager->renderer))
    {
       SDL_ExitWithError("Impossible de créer la fenêtre.");
    }

    TTF_CloseFont(font);
    freeTexture(texture);

}

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
                        if(isClicked(gameManager->quitButton, event.motion.x, event.motion.y))
                        {
                          program_launched = SDL_FALSE;
                          break;
                        }
                        if(isClicked(gameManager->startButton, event.motion.x, event.motion.y))
                        {
                            clearAndDrawBasic();
                            updateScore(0, 0);
                            break;
                        }
                        if(isClicked(gameManager->pierreBouton, event.motion.x, event.motion.y))
                        {
                            clearAndDrawBasic();
                            createBoutonWithImage("./resources/pierre.bmp", 10, 300, gameManager->renderer);
                            updateScore(gameManager->scoreMe, gameManager->scoreAdv);
                            break;
                        }
                        if(isClicked(gameManager->papierBouton, event.motion.x, event.motion.y))
                        {
                            clearAndDrawBasic();
                            createBoutonWithImage("./resources/papier.bmp", 10, 300, gameManager->renderer);
                            updateScore(gameManager->scoreMe, gameManager->scoreAdv);
                            break;
                        }
                        if(isClicked(gameManager->ciseauxBouton, event.motion.x, event.motion.y))
                        {
                            clearAndDrawBasic();
                            createBoutonWithImage("./resources/ciseaux.bmp", 10, 300, gameManager->renderer);
                            updateScore(gameManager->scoreMe, gameManager->scoreAdv);
                            break;
                        }
                     }
                    break;
                case SDL_QUIT:
                    program_launched = SDL_FALSE;
                    break;
                default:
                    break;
            }
        }

    }

}

void startGame()
{
    initSDL();
    initGameManager();
    initConnexion();
    gameLoop();
}

void initSDL()
{
     if (SDL_Init(SDL_INIT_VIDEO) != 0)
        SDL_ExitWithError("Impossible d'initialiser la SDL");
    if(TTF_Init() != 0)
        SDL_ExitWithError("Impossible d'initialiser la TTF");
}

//Pour la gestion des erreurs
void SDL_ExitWithError(const char* message)
{
    SDL_Log("ERREUR: %s > %s\n", message, SDL_GetError());
    destroyGame();
    exit(EXIT_FAILURE);
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


