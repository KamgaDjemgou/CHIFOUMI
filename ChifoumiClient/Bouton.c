#include "Bouton.h"

//Méthode pour créer un bouton avec texte
Bouton* createBouton(const char* texte, SDL_Color color, int x, int y, SDL_Renderer* rendu)
{
    Bouton *bouton = (Bouton*)malloc(sizeof(Bouton));

    bouton->texture = (Texture*)malloc(sizeof(Texture));

    TTF_Font* font = NULL;
    font = TTF_OpenFont("./resources/Symtext.ttf", 40);
    if(font == NULL)
    {
        freeBouton(bouton);
        return NULL;
    }
    bouton->texture->rect.x = x;
    bouton->texture->rect.y = y;
    if(!drawText(bouton->texture, texte, color, font, rendu)) return NULL;


    bouton->isActive = SDL_TRUE;

    TTF_CloseFont(font);

    return bouton;
}

//Méthode pour créer un bouton avec image
Bouton* createBoutonWithImage(const char* imagePath, int x, int y, SDL_Renderer* rendu)
{
    Bouton *bouton = (Bouton*)malloc(sizeof(Bouton));

    bouton->texture = (Texture*)malloc(sizeof(Texture));

    bouton->texture->rect.x = x;
    bouton->texture->rect.y = y;
    if(!drawImage(bouton->texture,imagePath, rendu)) return NULL;

    bouton->isActive = SDL_TRUE;

}

//Méthode qui permet de vérifier si un bouton est touché ou pas
SDL_bool isClicked(Bouton* bouton, int x, int y)
{
    if(bouton->isActive){
        SDL_Rect rect = bouton->texture->rect;
        return (x >= rect.x) && (x <= rect.x + rect.w) && (y >= rect.y) && (y <= rect.y + rect.h);
    }
    return SDL_FALSE;
}

//Libérer la mémoire
void freeBouton(Bouton* bouton)
{
    freeTexture(bouton->texture);
    bouton = NULL;
}
