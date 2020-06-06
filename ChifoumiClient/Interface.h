#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL.h>
#include "Texture.h"
#include "Bouton.h"
#include "ConnexionTCP.h"
#include <unistd.h>

//Définition des macros
#define SCREEN_WIDTH  800
#define SCREEEN_HEIGHT 800

#define PIERRE "PIERRE"
#define PAPIER "PAPIER"
#define CISEAUX "CISEAUX"

#define TTF "./resources/Symtext.ttf"
#define IMAGE_CISEAUX "./resources/ciseaux.bmp"
#define IMAGE_PAPIER "./resources/papier.bmp"
#define IMAGE_PIERRE "./resources/pierre.bmp"

#define QUITTER ":exit"
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

    //Les boutons pour le choix du joueur
    Bouton* pierreBouton;
    Bouton* papierBouton;
    Bouton* ciseauxBouton;
    
    //Score du joueur courant
    int scoreMe;
    //Score de son adversaire
    int scoreAdv;

    //Socket du joueur
    int socket;

}GameManager;

GameManager* gameManager;

void initSDLAndTTF();
void initGameManager();
void initConnexion();
void drawAllText();
void drawAllButtons();
void gameLoop();
void startGame();
void destroyGame();
void clearAndDrawBasic();
void setAllDisable();
void setAllAble();
void updateResult(const char* monChoixImage, const char* choixAdvImage, const char* result);
void drawAndPut(const char* text, int fontSize, SDL_Color color, int x, int y);
void traiterClic(const char* choix, const char* imagePath);
void traiterReceptionServeur(const char* monChoixImage);
void SDL_ExitWithError(const char* textError);
void updateScore(int monScore, int advScore);
void printResultOnScreen(const char* result);


#endif // INTERFACE_BEGIN_H

