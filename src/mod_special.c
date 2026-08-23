#include "mod_special.h"
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "graphics.h"

// --- Structuri ---
typedef struct
{
    bool amestecaAlb;
    bool amestecaNegru;
    bool adaugaPereti;
    int nrPereti;
    bool efectCeata;
} OptiuniTemporare;

// --- Variabile statice interne ---
static OptiuniTemporare optiuniPopup = {false, false, false, 5, false};
static SetariModSpecial setari = {true, true, true, 5, false};
static bool popupActiv = false;
static bool setariAplicate = false;
static int timerSetariAplicate = 0;

// --- Ceata ---
Fog fogsGlobal[NUM_FOGS];
SDL_Texture *fogTexturesGlobal[3];
int ceataActiva = 0;

// --- Elemente UI ---
static SDL_Rect popupRect = {100, 80, 400, 350}; 
static SDL_Rect titleRect = {150, 90, 300, 30};  
static SDL_Rect checkboxAlb = {130, 130, 20, 20};
static SDL_Rect checkboxNegru = {130, 170, 20, 20};
static SDL_Rect checkboxPereti = {130, 210, 20, 20};
static SDL_Rect sliderBar = {160, 240, 200, 10}; 
static SDL_Rect sliderKnob = {160 + 5 * 20, 235, 10, 20}; 
static SDL_Rect checkboxCeata = {130, 270, 20, 20};       
static SDL_Rect butonAplica = {150, 330, 120, 40};
static SDL_Rect butonInchide = {300, 330, 120, 40};

static void deseneazaCheckbox(SDL_Renderer *renderer, SDL_Rect rect, bool checked, const char *label)
{
    // Checkbox background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    // Checkbox margini
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);

    if (checked)
    {
        // Deseneaza bifa
        int x1 = rect.x + 4;
        int y1 = rect.y + rect.h / 2;
        int x2 = rect.x + rect.w / 3;
        int y2 = rect.y + rect.h - 4;
        int x3 = rect.x + rect.w - 4;
        int y3 = rect.y + 4;

        SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2); // Desenam prima linie a bifei
        SDL_RenderDrawLine(renderer, x2, y2, x3, y3); // A doua
        
        // Facem liniile mai groase
        SDL_RenderDrawLine(renderer, x1, y1 - 1, x2, y2 - 1);
        SDL_RenderDrawLine(renderer, x2, y2 - 1, x3, y3 - 1);
        SDL_RenderDrawLine(renderer, x1, y1 + 1, x2, y2 + 1);
        SDL_RenderDrawLine(renderer, x2 + 1, y2, x3 + 1, y3);
    }

   
    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans.ttf", 16);
    if (font)
    {
        SDL_Color textColor = {234, 230, 202, 255}; 
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font, label, textColor);
        if (surface)
        {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect textRect = {rect.x + rect.w + 10, rect.y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, NULL, &textRect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        TTF_CloseFont(font);
    }
}

static void deseneazaButon(SDL_Renderer *renderer, SDL_Rect rect, const char *text, SDL_Color color)
{
    // Buton background
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);

    // Buton border
    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
    SDL_RenderDrawRect(renderer, &rect);

    // Buton text
    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans.ttf", 16);
    if (font)
    {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, textColor);
        if (surface)
        {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect textRect = {
                rect.x + (rect.w - surface->w) / 2,
                rect.y + (rect.h - surface->h) / 2,
                surface->w,
                surface->h};
            SDL_RenderCopy(renderer, texture, NULL, &textRect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        TTF_CloseFont(font);
    }
}

static void deseneazaSlider(SDL_Renderer *renderer, SDL_Rect barRect, SDL_Rect knobRect, int value)
{
    // Desenam bara de la slider
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &barRect);

    // Desenam bara verticala de setare a slider-ului
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &knobRect);

    // Desenarea numerelor pentru slider
    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans.ttf", 16);
    if (font)
    {
        char valueText[10];
        sprintf(valueText, "%d", value);

        SDL_Color textColor = {230, 230, 230, 255};
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font, valueText, textColor);
        if (surface)
        {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect textRect = {
                barRect.x + barRect.w + 10,
                barRect.y - 5,
                surface->w,
                surface->h};
            SDL_RenderCopy(renderer, texture, NULL, &textRect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        TTF_CloseFont(font);
    }
}


void deseneazaPopupModSpecial(SDL_Renderer *renderer)
{
    // Semi-transparent background 
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect fullScreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &fullScreen);

    // Popup background
    SDL_SetRenderDrawColor(renderer, TROLLEY_GREY_R, TROLLEY_GREY_G, TROLLEY_GREY_B, 255);
    SDL_RenderFillRect(renderer, &popupRect);

    // Popup border 
    SDL_SetRenderDrawColor(renderer, LIGHT_CREAM_R, LIGHT_CREAM_G, LIGHT_CREAM_B, 255);
    SDL_RenderDrawRect(renderer, &popupRect);

    // Titlu
    TTF_Font *fontTitle = TTF_OpenFont("assets/fonts/OpenSans.ttf", 20);
    if (fontTitle)
    {
        SDL_Color color = {LIGHT_CREAM_R, LIGHT_CREAM_G, LIGHT_CREAM_B, 255};
        SDL_Surface *surface = TTF_RenderUTF8_Blended(fontTitle, "Setări Mod Teroare Nocturnă", color);
        if (surface)
        {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_Rect textRect = {
                titleRect.x + (titleRect.w - surface->w) / 2,
                titleRect.y,
                surface->w,
                surface->h};
            SDL_RenderCopy(renderer, texture, NULL, &textRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
        TTF_CloseFont(fontTitle);
    }

    // Setarile
    deseneazaCheckbox(renderer, checkboxAlb, optiuniPopup.amestecaAlb, "Amestecă piesele albe");
    deseneazaCheckbox(renderer, checkboxNegru, optiuniPopup.amestecaNegru, "Amestecă piesele negre");
    deseneazaCheckbox(renderer, checkboxPereti, optiuniPopup.adaugaPereti, "Adaugă pereți pe tablă");

    // Slider pentru numarul de pereti care e vizibil doar cand e bifat 
    if (optiuniPopup.adaugaPereti)
    {
        deseneazaSlider(renderer, sliderBar, sliderKnob, optiuniPopup.nrPereti);
    }

    deseneazaCheckbox(renderer, checkboxCeata, optiuniPopup.efectCeata, "Activeaza efectul de ceata");

    // Afisam butonul ce afiseaza daca s-au pus setarile 
    if (setariAplicate && timerSetariAplicate > 0)
    {
        deseneazaButon(renderer, butonAplica, "Aplicat!", (SDL_Color){0, 220, 0, 255});
        timerSetariAplicate--;
        if (timerSetariAplicate <= 0)
        {
            setariAplicate = false;
        }
    }
    else
    {
        deseneazaButon(renderer, butonAplica, "Aplica", (SDL_Color){0, 180, 80, 255});
    }
    deseneazaButon(renderer, butonInchide, "Inchide", (SDL_Color){180, 60, 60, 255});
}

// Aplicam noile setari
void aplicaSetari(Board *board, SDL_Renderer *renderer, SDL_Texture *textures[])
{
    // Updatam setarile 
    setari = (SetariModSpecial){
        optiuniPopup.amestecaAlb,
        optiuniPopup.amestecaNegru,
        optiuniPopup.adaugaPereti,
        optiuniPopup.nrPereti,
        optiuniPopup.efectCeata};

    // Reinitializam tabla cu noile setari
    initializeazaSpecialTabla(board, renderer, textures);

    
    if (setari.efectCeata)
    {
        activeazaEfectCeata(renderer);
    }
    else
    {
        ceataActiva = 0; // Dezactivam efectul de ceata
    }

    
    setariAplicate = true;
    timerSetariAplicate = 30; //Afisam ca s-au aplicat setarile pentru 30 de secunde
}


void proceseazaSlider(int x)
{
    if (x >= sliderBar.x && x <= sliderBar.x + sliderBar.w)
    {
        optiuniPopup.nrPereti = ((x - sliderBar.x) * 10) / sliderBar.w;
        //Ne asiguram ca e minim un perete
        if (optiuniPopup.adaugaPereti && optiuniPopup.nrPereti < 1)
            optiuniPopup.nrPereti = 1;
        sliderKnob.x = sliderBar.x + (optiuniPopup.nrPereti * sliderBar.w) / 10 - sliderKnob.w / 2;
    }
}

void proceseazaEvenimentePopup(SDL_Event *e, bool *deschis, Board *board, SDL_Renderer *renderer, SDL_Texture *textures[])
{
    if (e->type == SDL_QUIT)
    {
        *deschis = false;
    }
    else if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX = e->button.x;
        int mouseY = e->button.y;
        SDL_Point p = {mouseX, mouseY};

        //Verificam daca s-a apasat in afara popului
        if (!SDL_PointInRect(&p, &popupRect))
        {
            *deschis = false;
            return;
        }

        // Verifica coliziuni cu checkboxurile
        if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &checkboxAlb))
        {
            optiuniPopup.amestecaAlb = !optiuniPopup.amestecaAlb;
        }
        else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &checkboxNegru))
        {
            optiuniPopup.amestecaNegru = !optiuniPopup.amestecaNegru;
        }
        else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &checkboxPereti))
        {
            optiuniPopup.adaugaPereti = !optiuniPopup.adaugaPereti;
            // Actualizeaza pozitia slider-ului când este activat/dezactivat
        }
        else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &checkboxCeata))
        {
            optiuniPopup.efectCeata = !optiuniPopup.efectCeata;
        }
        // Verifica coliziune cu slider
        else if (optiuniPopup.adaugaPereti && mouseY >= sliderBar.y - 10 && mouseY <= sliderBar.y + sliderBar.h + 10 && mouseX >= sliderBar.x && mouseX <= sliderBar.x + sliderBar.w)
        {
            proceseazaSlider(mouseX);
        }
        // Verifica coliziune cu butonul de aplicare
        else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &butonAplica))
        {
            aplicaSetari(board, renderer, textures);
        }
        // Verifica coliziune cu butonul de inchidere
        else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &butonInchide))
        {
            *deschis = false;
        }
    }
    else if (e->type == SDL_MOUSEMOTION && e->motion.state & SDL_BUTTON_LMASK)
    {
        // Actualizeaza pozitia slider-ului în timpul glisarii
        int mouseX = e->motion.x;
        int mouseY = e->motion.y;

        if (optiuniPopup.adaugaPereti && mouseY >= sliderBar.y - 10 && mouseY <= sliderBar.y + sliderBar.h + 10 && mouseX >= sliderBar.x - 10 && mouseX <= sliderBar.x + sliderBar.w + 10)
        {
            proceseazaSlider(mouseX);
        }
    }
    else if (e->type == SDL_KEYDOWN)
    {
        if (e->key.keysym.sym == SDLK_ESCAPE)
        {
            *deschis = false;
        }
    }
}


bool estePopupActiv()
{
    return popupActiv;
}

void deschideMeniuModSpecial(SDL_Renderer *renderer, Board *board, SDL_Texture *textures[])
{
    popupActiv = true;

    optiuniPopup = (OptiuniTemporare){
        setari.amestecaAlb,
        setari.amestecaNegru,
        setari.adaugaPereti,
        setari.nrPereti,
        setari.efectCeata};

    sliderKnob.x = sliderBar.x + (optiuniPopup.nrPereti * sliderBar.w) / 10 - sliderKnob.w / 2;
}


void afiseazaMeniuModSpecial(SDL_Renderer *renderer)
{
    if (popupActiv)
    {
        deseneazaPopupModSpecial(renderer);
    }
}

void gestioneazaEvenimenteMeniuModSpecial(SDL_Event *e, Board *board, SDL_Renderer *renderer, SDL_Texture *textures[])
{
    if (popupActiv)
    {
        bool deschis = true;
        proceseazaEvenimentePopup(e, &deschis, board, renderer, textures);
        if (!deschis)
        {
            popupActiv = false;
        }
    }
}

SetariModSpecial getSetariModSpecial()
{
    return setari;
}

void activeazaEfectCeata(SDL_Renderer *renderer)
{
    initFog(renderer, fogsGlobal, fogTexturesGlobal);
    ceataActiva = 1;
}

void toggleMeniuModSpecial()
{
    popupActiv = !popupActiv;
}
