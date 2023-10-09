#pragma once

#include <SDL2/SDL.h>

class ScreenPainter {
    const int m_screen_width;
    const int m_screen_height;

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture{};
    SDL_Surface* m_surface;
public:
    ScreenPainter(int width, int height);
    int getScreenWidth() const;
    int getScreenHeight() const;
    void setPixel(int x, int y, float r, float g, float b);
    void display();
    void wait() const;
    void clear();
};
