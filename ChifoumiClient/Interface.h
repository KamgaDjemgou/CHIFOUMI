#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL.h>
#include "Texture.h"
#include "Bouton.h"
#include "ConnexionTCP.h"
#include <unistd.h>

#define IMAGE_CISEAUX "./resources/ciseaux.bmp"
#define IMAGE_PAPIER "./resources/papier.bmp"
#define IMAGE_PIERRE "./resources/pierre.bmp"

#define GAGNE "GAGNE"
#define PERDU "PERDU"
#define NUL "NUL"
#define MATCH_PERDU "MATCH_PERDU"
#define MATCH_GAGNE "MATCH_GAGNE"
#define FORFAIT "FORFAIT"

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

    char choix[TAILLE];

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
void traiterClic(const char* choix, const char* imagePath);
void setAllDisable();
void setAllAble();
void printResultOnScreen(const char* result);

#endif // INTERFACE_BEGIN_H

