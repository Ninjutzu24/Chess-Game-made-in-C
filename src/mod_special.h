#ifndef MOD_SPECIAL_H
#define MOD_SPECIAL_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "graphics.h"
#include "board.h"

#define NUM_FOGS 50


void initializeazaSpecialTabla(Board *board, SDL_Renderer *renderer, SDL_Texture *textures[]);

typedef struct
{
    SDL_Rect rect;
    float alpha;
    float speed;
} FogEffect;

extern FogEffect fogsEffectGlobal[NUM_FOGS];

// Structura pentru setarile modului special
typedef struct
{
    bool amestecaAlb;
    bool amestecaNegru;
    bool adaugaPereti;
    int nrPereti; 
    bool efectCeata;
} SetariModSpecial;

// Variabile globale externe pentru efectul de ceata
extern Fog fogsGlobal[NUM_FOGS];
extern SDL_Texture *fogTexturesGlobal[3];
extern int ceataActiva;

// Funcții accesibile din exterior
SetariModSpecial getSetariModSpecial(void);
void deschideMeniuModSpecial(SDL_Renderer *renderer, Board *board, SDL_Texture *textures[]);
void activeazaEfectCeata(SDL_Renderer *renderer);

// Funcții noi pentru popup real-time
bool estePopupActiv(void);
void afiseazaMeniuModSpecial(SDL_Renderer *renderer);
void gestioneazaEvenimenteMeniuModSpecial(SDL_Event *e, Board *board, SDL_Renderer *renderer, SDL_Texture *textures[]);
void toggleMeniuModSpecial(void);
void aplicaSetari(Board *board, SDL_Renderer *renderer, SDL_Texture *textures[]);

#endif // MOD_SPECIAL_H
