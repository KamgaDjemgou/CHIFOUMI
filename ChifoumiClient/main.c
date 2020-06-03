#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Interface.h"

int main(int argc, char *argv[])
{
    //Commencer le jeu
    startGame();

    //Detruire les resources utilisées pour vider la mémoire
    destroyGame();

    return EXIT_SUCCESS;
}
