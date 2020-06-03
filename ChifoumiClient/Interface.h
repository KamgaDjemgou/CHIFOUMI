#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL.h>
#include "Texture.h"
#include "Bouton.h"
#include "ConnexionTCP.h"

typedef struct _GameManager
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    Bouton* pierreBouton;
    Bouton* papierBouton;
    Bouton* ciseauxBouton;
    Bouton* startButton;
    Bouton* quitButton;
    //Score du joueur courant
    int scoreMe;
    //Score de son adversaire
    int scoreAdv;

    //Socket du joueur
    int socket;

}GameManager;

GameManager* gameManager;

void initSDL();
void initGameManager();
void initConnexion();
void gameLoop();
void startGame();
void SDL_ExitWithError(const char*);
void destroyGame();
void drawAndPut(const char* text, int fontSize, SDL_Color color, int x, int y);
void updateScore(int monScore, int advScore);
void clearAndDrawBasic();

#endif // INTERFACE_BEGIN_H

