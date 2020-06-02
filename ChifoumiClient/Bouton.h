#ifndef BOUTON_H
#define BOUTON_H

#include <SDL.h>
#include "Texture.h"

typedef struct _BOUTON
{
    Texture* texture;
    SDL_bool isActive;
    int x;
    int y;

}Bouton;
Bouton* createBouton(const char* texte, SDL_Color color, int x, int y, SDL_Renderer* rendu);
SDL_bool isClicked(Bouton* bouton, int x, int y);
void freeBouton(Bouton* bouton);

#endif
