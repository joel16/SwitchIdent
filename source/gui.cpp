#include <cstdio>
#include <SDL2/SDL_image.h>
#include <SDL_ttf.h>

#include "gui.hpp"

SDL_Texture *banner = nullptr, *drive = nullptr, *menu_icons[7] = { 0 };

namespace GUI {
    static SDL_Window *g_window = nullptr;
    static SDL_Renderer *g_renderer = nullptr;
    static TTF_Font *g_font = nullptr; 

    static void LoadImage(SDL_Texture **texture, const char *path) {
        SDL_Surface *image = nullptr;
        image = IMG_Load(path);
        
        if (!image) {
            SDL_Log("IMG_Load failed: %s\n", IMG_GetError());
            return;
        }
            
        SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);
        *texture = SDL_CreateTextureFromSurface(g_renderer, image);
        SDL_FreeSurface(image);
    }

    int Init(void) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            SDL_Log("SDL_Init failed: %s\n", SDL_GetError());
            return -1;
        }
            
        g_window = SDL_CreateWindow("SwitchIdent", 0, 0, 1920, 1080, SDL_WINDOW_FULLSCREEN);
        if (g_window == nullptr) {
            SDL_Log("SDL_CreateWindow failed: %s\n", SDL_GetError());
            return -1;
        }
            
        g_renderer = SDL_CreateRenderer(g_window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (g_renderer == nullptr) {
            SDL_Log("SDL_CreateRenderer failed: %s\n", SDL_GetError());
            return -1;
        }
        
        int flags = IMG_INIT_PNG;
        if ((IMG_Init(flags) & flags) != flags) {
            SDL_Log("IMG_Init failed: %s\n", IMG_GetError());
            return -1;
        }
            
        GUI::LoadImage(&banner, "romfs:/banner.png");
        GUI::LoadImage(&drive, "romfs:/drive.png");
        GUI::LoadImage(&menu_icons[0], "romfs:/kernel.png");
        GUI::LoadImage(&menu_icons[1], "romfs:/system.png");
        GUI::LoadImage(&menu_icons[2], "romfs:/power.png");
        GUI::LoadImage(&menu_icons[3], "romfs:/storage.png");
        GUI::LoadImage(&menu_icons[4], "romfs:/joycon.png");
        GUI::LoadImage(&menu_icons[5], "romfs:/misc.png");
        GUI::LoadImage(&menu_icons[6], "romfs:/exit.png");

        TTF_Init();
        g_font = TTF_OpenFont("romfs:/Ubuntu-Regular.ttf", 36);
        return 0;
    }

    void Exit(void) {
        TTF_CloseFont(g_font);
        SDL_DestroyTexture(menu_icons[6]);
        SDL_DestroyTexture(menu_icons[5]);
        SDL_DestroyTexture(menu_icons[4]);
        SDL_DestroyTexture(menu_icons[3]);
        SDL_DestroyTexture(menu_icons[2]);
        SDL_DestroyTexture(menu_icons[1]);
        SDL_DestroyTexture(menu_icons[0]);
        SDL_DestroyTexture(banner);
        SDL_DestroyTexture(drive);
        TTF_Quit();
        IMG_Quit();
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);
        SDL_Quit();
    }

    void ClearScreen(SDL_Color colour) {
        SDL_SetRenderDrawColor(g_renderer, colour.r, colour.g, colour.b, colour.a);
        SDL_RenderClear(g_renderer);
    }
    
    void DrawRect(int x, int y, int w, int h, SDL_Color colour) {
        SDL_Rect rect;
        rect.x = x; rect.y = y; rect.w = w; rect.h = h;
        SDL_SetRenderDrawColor(g_renderer, colour.r, colour.g, colour.b, colour.a);
        SDL_RenderFillRect(g_renderer, &rect);
    }
    
    void DrawText(int x, int y, int size, SDL_Color colour, const char *text) {
        SDL_Surface *surface = nullptr;
        SDL_Texture *texture = nullptr;
        
        surface = TTF_RenderUTF8_Blended(g_font, text, colour);
        texture = SDL_CreateTextureFromSurface(g_renderer, surface);
        SDL_Rect position = {x, y, surface->w, surface->h};

        SDL_RenderCopy(g_renderer, texture, nullptr, &position);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    void DrawTextf(int x, int y, int size, SDL_Color colour, const char* text, ...) {
        char buffer[256];
        va_list args;
        va_start(args, text);
        std::vsnprintf(buffer, 256, text, args);
        GUI::DrawText(x, y, size, colour, buffer);
        va_end(args);
    }
    
    void GetTextDimensions(int size, const char *text, int *width, int *height) {
        TTF_SizeText(g_font, text, width, height);
    }
    
    void DrawImage(SDL_Texture *texture, int x, int y) {
        SDL_Rect position;
        position.x = x; position.y = y;
        SDL_QueryTexture(texture, nullptr, nullptr, &position.w, &position.h);
        SDL_RenderCopy(g_renderer, texture, nullptr, &position);
    }
    
    void Render(void) {
        SDL_RenderPresent(g_renderer);
    }
}
