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

    printf("Bouton: %d %d %d %d\n", bouton->texture->rect.x, bouton->texture->rect.y,bouton->texture->rect.h, bouton->texture->rect.w);
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

    printf("Bouton: %d %d %d %d\n", bouton->texture->rect.x, bouton->texture->rect.y,bouton->texture->rect.h, bouton->texture->rect.w);
    bouton->isActive = SDL_TRUE;

}
//Libérer la mémoire
void freeBouton(Bouton* bouton)
{
    freeTexture(bouton->texture);
    bouton = NULL;
}
