#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL.h>
#include "Texture.h"

typedef struct _GameManager
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    //Score du joueur courant
    Texture* scoreMe;
    //Score de son adversaire
    Texture* scoreAdv;

}GameManager;

GameManager* gameManager;

void initSDL();
void initGameManager();
void gameLoop();
void startGame();
void SDL_ExitWithError(const char*);
void destroyGame(GameManager*);
Texture* drawAndPut(const char* text, int fontSize, SDL_Color color, int x, int y);

#endif // INTERFACE_BEGIN_H

