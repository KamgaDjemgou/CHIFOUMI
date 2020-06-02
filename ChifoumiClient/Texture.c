#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "Texture.h"

SDL_bool drawText(Texture* texture, const char* texte, SDL_Color couleur, TTF_Font* police, SDL_Renderer* rendu)
{
   if((texture != NULL) && (police != NULL) && (rendu != NULL))
   {
        SDL_Surface* surfaceTexte = NULL;
        surfaceTexte = TTF_RenderText_Solid(police, texte, couleur);

        if(surfaceTexte == NULL) return SDL_FALSE;

        texture->texture = SDL_CreateTextureFromSurface(rendu,surfaceTexte);
        if(texture == NULL) return SDL_FALSE;

        texture->rect.h = surfaceTexte->h;
        texture->rect.w = surfaceTexte->w;
        SDL_RenderCopy(rendu, texture->texture, NULL, &texture->rect);

        SDL_FreeSurface(surfaceTexte);

        return SDL_TRUE;
   }

   return SDL_FALSE;
}
//Libérer la mémoire
void freeTexture(Texture* texture)
{
    if (texture->texture != NULL)
        SDL_DestroyTexture(texture->texture);

    free(texture);
    texture = NULL;
}


