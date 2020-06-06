#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_ttf.h>

#define TTF "./resources/Symtext.ttf"

typedef struct _texture
{
    SDL_Texture* texture;
    SDL_Rect rect;
} Texture;

SDL_bool drawImage(Texture*, const char*, SDL_Renderer*);
SDL_bool drawText(Texture*, const char*, SDL_Color, TTF_Font*, SDL_Renderer*);
void freeTexture(Texture*);

#endif // TEXTURE_H
