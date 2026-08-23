#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"
#include "SDL2/SDL_image.h"

extern int ceataActiva;

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path)
{
    SDL_Texture *newTexture = IMG_LoadTexture(renderer, path);
    if (newTexture == NULL)
    {
        printf("Unable to load texture %s: %s\n", path, SDL_GetError());
    }
    return newTexture;
}

void initFog(SDL_Renderer *renderer, Fog fogs[], SDL_Texture *fogTextures[])
{
    // Verifica daca avem cel puțin o textura disponibila
    int hasTexture = 0;
    for (int i = 0; i < 3; i++)
    {
        if (fogTextures[i] != NULL)
        {
            hasTexture = 1;
            break;
        }
    }

    // Daca nu avem nicio textura, cream una de backup
    SDL_Texture *backupTexture = NULL;
    if (!hasTexture)
    {
        SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, 64, 64, 32, 0, 0, 0, 0);
        if (tempSurface)
        {
            SDL_FillRect(tempSurface, NULL, SDL_MapRGBA(tempSurface->format, 255, 255, 255, 100));
            backupTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface);

            if (backupTexture)
            {
                // Setam toate texturile la cea de backup
                for (int i = 0; i < 3; i++)
                {
                    fogTextures[i] = backupTexture;
                }
            }
        }
    }

    for (int i = 0; i < NUM_FOGS; i++)
    {
        // Distribuie ceata uniform pe ecran
        fogs[i].x = (rand() % (SCREEN_WIDTH + 200)) - 100;  
        fogs[i].y = (rand() % (SCREEN_HEIGHT + 200)) - 100; 

        float speed = ((float)(rand() % 5)) / 100.0f + 0.01f; 
        if (rand() % 2 == 0)
            speed *= -1; 

        fogs[i].dx = speed;
        fogs[i].dy = speed * (0.5f + ((float)(rand() % 10)) / 10.0f); 

        fogs[i].size = 100 + rand() % 200; 

        fogs[i].opacity = 20 + rand() % 40; 

        if (hasTexture || backupTexture)
        {
            int textureIndex = rand() % 3;
            fogs[i].texture = fogTextures[textureIndex];
        }
        else
        {
            fogs[i].texture = NULL;
        }
    }
}

void updateFog(Fog fogs[])
{
    for (int i = 0; i < NUM_FOGS; i++)
    {
        fogs[i].x += fogs[i].dx;
        fogs[i].y += fogs[i].dy;

        if (rand() % 30 == 0)
        {                                   
            int change = (rand() % 10) - 5; 
            fogs[i].opacity += change;

            if (fogs[i].opacity < 15)
                fogs[i].opacity = 15;
            if (fogs[i].opacity > 70)
                fogs[i].opacity = 70;
        }

        if (fogs[i].x < -fogs[i].size)
            fogs[i].x = SCREEN_WIDTH + 10;
        if (fogs[i].x > SCREEN_WIDTH + 10)
            fogs[i].x = -fogs[i].size;
        if (fogs[i].y < -fogs[i].size)
            fogs[i].y = SCREEN_HEIGHT + 10;
        if (fogs[i].y > SCREEN_HEIGHT + 10)
            fogs[i].y = -fogs[i].size;
    }
}

void drawFog(SDL_Renderer *renderer, Fog fogs[])
{
    for (int i = 0; i < NUM_FOGS; i++)
    {
        if (fogs[i].texture == NULL)
        {
            continue; 
        }

      
        SDL_SetTextureBlendMode(fogs[i].texture, SDL_BLENDMODE_BLEND);

   
        SDL_SetTextureColorMod(fogs[i].texture,
                               200 + rand() % 55, 
                               50 + rand() % 30,  
                               50 + rand() % 20); 

        SDL_SetTextureAlphaMod(fogs[i].texture, fogs[i].opacity);

        float pulseFactor = 1.0f + 0.2f * sinf((float)SDL_GetTicks() / 500.0f + i);
        int adjustedSize = (int)(fogs[i].size * pulseFactor);

        SDL_Rect dstRect = {
            (int)fogs[i].x - adjustedSize / 2,
            (int)fogs[i].y - adjustedSize / 2,
            adjustedSize,
            adjustedSize};

        SDL_RenderCopy(renderer, fogs[i].texture, NULL, &dstRect);

       SDL_SetTextureColorMod(fogs[i].texture, 255, 255, 255);
    }
}
