#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "Texture.h"

//Méthode pour dessiner un texte
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

//Dessiner une image
SDL_bool drawImage(Texture* texture, const char* imagePath, SDL_Renderer* renderer)
{
    if((texture != NULL) && (renderer != NULL))
    {
        SDL_Surface* surface = NULL;
        surface = SDL_LoadBMP(imagePath);
        if(surface == NULL) return SDL_FALSE;

        texture->texture = SDL_CreateTextureFromSurface(renderer,surface);
        if(texture == NULL) return SDL_FALSE;

        texture->rect.h = surface->h;
        texture->rect.w = surface->w;
        SDL_RenderCopy(renderer, texture->texture, NULL, &texture->rect);

        SDL_FreeSurface(surface);

        return SDL_TRUE;
    }
    return SDL_FALSE;
}

void renderTexture(Texture* texture, int x, int y, SDL_Renderer* renderer)
{
	SDL_Rect* renderRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	renderRect->x = x;
    renderRect->y = y;
	renderRect->w = texture->rect.w;
	renderRect->h = texture->rect.h;

	SDL_RenderCopyEx(renderer, texture->texture, NULL, renderRect, 0.0, NULL, SDL_FLIP_NONE);
}
//Libérer la mémoire
void freeTexture(Texture* texture)
{
    if (texture->texture != NULL)
        SDL_DestroyTexture(texture->texture);

    free(texture);
    texture = NULL;
}


