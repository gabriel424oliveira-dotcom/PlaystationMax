/*
 * PlaystationMax v0.3 — SDL2 UI prototype.
 *
 * Dependencies: SDL2 and SDL_ttf, built for the same target environment.
 * This is source code, not a verified PS4 binary. Use the SDL-PS4 port and
 * the build configuration from a working OpenOrbis sample.
 *
 * Controls:
 *   Up/Down/Left/Right or D-pad: move selection
 *   Enter / controller A: select category/card
 *   Escape / controller B: back to category navigation
 *   Q: quit (desktop testing)
 */
#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>
#include <vector>

struct MediaItem {
    std::string title;
    std::string type;
    std::string year;
    SDL_Color color;
};

static const SDL_Color BG      = { 8, 13, 27, 255 };
static const SDL_Color PANEL   = { 17, 27, 49, 255 };
static const SDL_Color WHITE   = { 238, 244, 255, 255 };
static const SDL_Color MUTED   = { 145, 164, 194, 255 };
static const SDL_Color ACCENT  = { 43, 139, 255, 255 };
static const SDL_Color CARD_A  = { 31, 82, 145, 255 };
static const SDL_Color CARD_B  = { 94, 48, 128, 255 };
static const SDL_Color CARD_C  = { 24, 112, 105, 255 };
static const SDL_Color CARD_D  = { 151, 75, 42, 255 };

static void fill(SDL_Renderer* r, SDL_Rect rect, SDL_Color c) {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(r, &rect);
}
static void outline(SDL_Renderer* r, SDL_Rect rect, SDL_Color c, int width=3) {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    for (int i=0; i<width; ++i) {
        SDL_Rect q{rect.x+i, rect.y+i, rect.w-2*i, rect.h-2*i};
        SDL_RenderDrawRect(r, &q);
    }
}
static void text(SDL_Renderer* r, TTF_Font* font, const std::string& s,
                 int x, int y, SDL_Color color) {
    if (!font) return;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, s.c_str(), color);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    if (tex) {
        SDL_Rect dst{x, y, surf->w, surf->h};
        SDL_RenderCopy(r, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }
    SDL_FreeSurface(surf);
}
static void card(SDL_Renderer* r, TTF_Font* font, const MediaItem& item,
                 int x, int y, int w, int h, bool selected) {
    SDL_Rect shadow{x+5, y+7, w, h};
    fill(r, shadow, {3, 5, 12, 255});
    SDL_Rect body{x, y, w, h};
    fill(r, body, item.color);
    SDL_Rect band{x, y+h-58, w, 58};
    fill(r, band, {10, 17, 31, 235});
    // Decorative poster artwork; replace with licensed cover images later.
    SDL_SetRenderDrawColor(r, 255, 255, 255, 40);
    for (int i=0; i<5; ++i) {
        SDL_Rect stripe{x+18+i*22, y+22+i*8, 8, h/2};
        SDL_RenderFillRect(r, &stripe);
    }
    text(r, font, item.title, x+10, y+h-49, WHITE);
    text(r, font, item.year + "  •  " + item.type, x+10, y+h-25, MUTED);
    if (selected) outline(r, {x-4, y-4, w+8, h+8}, ACCENT, 4);
}

int main(int, char**) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        std::fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        std::fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "PlaystationMax", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        TTF_Quit(); SDL_Quit(); return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        std::fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window); TTF_Quit(); SDL_Quit(); return 1;
    }

    // Replace these paths with a font packaged with the app and licensed for use.
    TTF_Font* font = TTF_OpenFont("assets/DejaVuSans.ttf", 20);
    TTF_Font* small = TTF_OpenFont("assets/DejaVuSans.ttf", 15);
    if (!font) std::fprintf(stderr, "Font not loaded: %s\n", TTF_GetError());

    const std::vector<std::string> categories = {
        "Inicio", "Filmes", "Series", "Desenhos", "USB", "Rede local", "Internet", "Favoritos"
    };
    const std::vector<MediaItem> items = {
        {"Aventura Azul", "Filme", "2025", CARD_A},
        {"Noite de Misterio", "Serie", "2024", CARD_B},
        {"Mundo Animado", "Desenho", "2023", CARD_C},
        {"Espaco Distante", "Anime", "2025", CARD_D}
    };
    int categoryIndex = 0;
    int itemIndex = 0;
    bool running = true;
    SDL_GameController* controller = nullptr;
    for (int i=0; i<SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) { controller = SDL_GameControllerOpen(i); if (controller) break; }
    }

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: categoryIndex = 0; break;
                    case SDLK_q: running = false; break;
                    case SDLK_LEFT:  if (itemIndex > 0) --itemIndex; else if (categoryIndex > 0) --categoryIndex; break;
                    case SDLK_RIGHT: if (itemIndex < (int)items.size()-1) ++itemIndex; else if (categoryIndex < (int)categories.size()-1) ++categoryIndex; break;
                    case SDLK_UP: if (categoryIndex > 0) --categoryIndex; break;
                    case SDLK_DOWN: if (categoryIndex < (int)categories.size()-1) ++categoryIndex; break;
                    case SDLK_RETURN: std::printf("Selected: %s / %s\n", categories[categoryIndex].c_str(), items[itemIndex].title.c_str()); break;
                    default: break;
                }
            }
            if (e.type == SDL_CONTROLLERBUTTONDOWN) {
                switch (e.cbutton.button) {
                    case SDL_CONTROLLER_BUTTON_DPAD_LEFT: if (itemIndex > 0) --itemIndex; else if (categoryIndex > 0) --categoryIndex; break;
                    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: if (itemIndex < (int)items.size()-1) ++itemIndex; else if (categoryIndex < (int)categories.size()-1) ++categoryIndex; break;
                    case SDL_CONTROLLER_BUTTON_DPAD_UP: if (categoryIndex > 0) --categoryIndex; break;
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN: if (categoryIndex < (int)categories.size()-1) ++categoryIndex; break;
                    case SDL_CONTROLLER_BUTTON_A: std::printf("Selected: %s / %s\n", categories[categoryIndex].c_str(), items[itemIndex].title.c_str()); break;
                    case SDL_CONTROLLER_BUTTON_B: categoryIndex = 0; break;
                    default: break;
                }
            }
        }

        int ww=1280, hh=720;
        SDL_GetRendererOutputSize(renderer, &ww, &hh);
        SDL_SetRenderDrawColor(renderer, BG.r, BG.g, BG.b, BG.a);
        SDL_RenderClear(renderer);
        fill(renderer, {0,0,ww,96}, PANEL);
        text(renderer, font, "PlaystationMax", 42, 23, WHITE);
        text(renderer, small, "FILMES  •  SERIES  •  DESENHOS  •  ANIMES", 42, 61, MUTED);
        text(renderer, small, "HOME MEDIA", ww-180, 38, ACCENT);

        // Category rail
        int railY = 122;
        for (int i=0; i<(int)categories.size(); ++i) {
            SDL_Rect b{36, railY+i*54, 210, 42};
            if (i == categoryIndex) fill(renderer, b, ACCENT);
            else fill(renderer, b, PANEL);
            text(renderer, small, categories[i], b.x+14, b.y+11, i==categoryIndex ? WHITE : MUTED);
        }

        text(renderer, font, categoryIndex==0 ? "Continue explorando" : categories[categoryIndex], 285, 130, WHITE);
        text(renderer, small, "Use as setas ou o direcional do controle", 285, 165, MUTED);

        const int startX=286, startY=205, cw=190, ch=270, gap=22;
        for (int i=0; i<(int)items.size(); ++i)
            card(renderer, small, items[i], startX+i*(cw+gap), startY, cw, ch, i==itemIndex);

        fill(renderer, {282, 510, ww-320, 150}, PANEL);
        text(renderer, font, items[itemIndex].title, 310, 530, WHITE);
        text(renderer, small, items[itemIndex].type + "  •  " + items[itemIndex].year, 310, 565, ACCENT);
        text(renderer, small, "Exemplo visual. Adicione seu catalogo autorizado em data/catalogue.json.", 310, 602, MUTED);
        text(renderer, small, "A  Selecionar      B  Voltar      Q  Sair (teste desktop)", 310, 628, MUTED);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    if (controller) SDL_GameControllerClose(controller);
    if (small) TTF_CloseFont(small);
    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
