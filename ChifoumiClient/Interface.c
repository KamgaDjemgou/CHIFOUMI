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

void clearAndDrawBasic()
{
    SDL_RenderClear(gameManager->renderer);
    SDL_DestroyRenderer(gameManager->renderer);

    gameManager->renderer = SDL_CreateRenderer(gameManager->window, -1, SDL_RENDERER_SOFTWARE);
    if(gameManager->renderer == NULL)
    {
        destroyGame(gameManager);
        SDL_ExitWithError("Impossible de créer le rendu");
    }

    SDL_Surface *screenSurface = SDL_GetWindowSurface(gameManager->window);
    if(screenSurface == NULL)
    {
        destroyGame(gameManager);
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
    gameManager->ciseauxBouton = createBouton("CISEAUX", color, 124, 600, gameManager->renderer);

    color.r = 192;
    color.g = 192;
    color.b = 192;
    gameManager->pierreBouton = createBouton("PIERRE", color, 334, 600, gameManager->renderer);

    color.r = 0;
    color.g = 255;
    color.b = 0;
    gameManager->papierBouton = createBouton("PAPIER", color, 514, 600, gameManager->renderer);

    if(gameManager->ciseauxBouton == NULL || gameManager->papierBouton == NULL || gameManager->pierreBouton == NULL)
    {
        destroyGame(gameManager);
        SDL_ExitWithError("Impossible de créer le bouton");
    }

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
    printf("%d %d %d %d\n", texture->rect.x, texture->rect.y,texture->rect.h, texture->rect.w);
    freeTexture(texture);

}

void gameLoop(){
    SDL_bool program_launched = SDL_TRUE;
    int test = 15;
    while(program_launched){
        SDL_Event event;
        test++;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    program_launched = SDL_FALSE;
                    break;
                default:
                    //clearAndDrawBasic();
                    //updateScore(test%10, test%15);
                    break;
            }
        }

    }

}

void startGame()
{
    initSDL();
    initGameManager();
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
    SDL_Quit();
    exit(EXIT_FAILURE);
}

//Libérer l'espace mémoire
void destroyGame(GameManager* gameManager)
{
   SDL_DestroyRenderer(gameManager->renderer);
   SDL_DestroyWindow(gameManager->window);
}


