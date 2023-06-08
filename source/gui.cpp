#include <cstdio>
#include <SDL2/SDL_image.h>

#include "gui.hpp"
#include "SDL_FontCache.h"

SDL_Texture *banner = nullptr, *drive = nullptr, *menu_icons[8] = { 0 };

namespace GUI {
    static SDL_Window *g_window = nullptr;
    static SDL_Renderer *g_renderer = nullptr;
    static FC_Font *g_font = nullptr; 

    static void LoadImage(SDL_Texture **texture, const char *path) {
        SDL_Surface *image = nullptr;
        image = IMG_Load(path);
        
        if (!image)
            return;
            
        SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);
        *texture = SDL_CreateTextureFromSurface(g_renderer, image);
        SDL_FreeSurface(image);
    }

    int Init(void) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            return -1;
            
        g_window = SDL_CreateWindow("SwitchIdent", 0, 0, 1280, 720, SDL_WINDOW_FULLSCREEN);
        if (g_window == nullptr)
            return -1;
            
        g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        
        int flags = IMG_INIT_PNG;
        if ((IMG_Init(flags) & flags) != flags)
            return -1;
            
        GUI::LoadImage(&banner, "romfs:/banner.png");
        GUI::LoadImage(&drive, "romfs:/drive.png");
        GUI::LoadImage(&menu_icons[0], "romfs:/kernel.png");
        GUI::LoadImage(&menu_icons[1], "romfs:/system.png");
        GUI::LoadImage(&menu_icons[2], "romfs:/power.png");
        menu_icons[3] = menu_icons[2];
        GUI::LoadImage(&menu_icons[4], "romfs:/storage.png");
        GUI::LoadImage(&menu_icons[5], "romfs:/joycon.png");
        GUI::LoadImage(&menu_icons[6], "romfs:/misc.png");
        GUI::LoadImage(&menu_icons[7], "romfs:/exit.png");
        
        g_font = FC_CreateFont();
        FC_LoadFont(g_font, g_renderer, "romfs:/Ubuntu-Regular.ttf", 25, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);
        return 0;
    }

    void Exit(void) {
        FC_FreeFont(g_font);
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
        FC_DrawColor(g_font, g_renderer, x, y, colour, text);
    }
    
    void DrawTextf(int x, int y, int size, SDL_Color colour, const char* text, ...) {
        char buffer[256];
        va_list args;
        va_start(args, text);
        std::vsnprintf(buffer, 256, text, args);
        GUI::DrawText(x, y, size, colour, buffer);
        va_end(args);
    }
    
    void GetTextDimensions(int size, const char *text, u32 *width, u32 *height) {
        if (width != nullptr) 
            *width = FC_GetWidth(g_font, text);
        if (height != nullptr) 
            *height = FC_GetHeight(g_font, text);
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
