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
    //Création du rendu
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

    gameManager->scoreMe = drawAndPut("5", 40, color, 10, 150);
    gameManager->scoreAdv = drawAndPut("1", 40, color, (SCREEN_WIDTH-100), 150);
    freeTexture(gameManager->scoreMe);
    freeTexture(gameManager->scoreAdv);
    SDL_RenderPresent(gameManager->renderer);
    gameManager->scoreMe = drawAndPut("10", 40, color, 10, 150);
    gameManager->scoreAdv = drawAndPut("312", 40, color, (SCREEN_WIDTH-100), 150);




    SDL_RenderPresent(gameManager->renderer);
    SDL_UpdateWindowSurface(gameManager->window);

}

Texture* drawAndPut(const char* text, int fontSize, SDL_Color color, int x, int y)
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

    printf("%d %d %d %d\n", texture->rect.x, texture->rect.y,texture->rect.h, texture->rect.w);
    return texture;
}

void gameLoop(){
    SDL_bool program_launched = SDL_TRUE;
    while(program_launched){
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
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


