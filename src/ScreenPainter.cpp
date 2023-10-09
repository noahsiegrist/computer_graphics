#include "ScreenPainter.h"

int ScreenPainter::getScreenWidth() const {
    return m_screen_width;
}
int ScreenPainter::getScreenHeight() const {
    return m_screen_height;
}

ScreenPainter::ScreenPainter(int width, int height)
    : m_screen_width{width}
    , m_screen_height{height}
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing sdl2: %s\n", SDL_GetError());
        return;
    }

    m_window = SDL_CreateWindow("Gradient Nebula", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screen_width, m_screen_height, SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    m_surface = SDL_CreateRGBSurface(0, m_screen_width, m_screen_height, 32, 0, 0, 0, 0);
}

void ScreenPainter::setPixel(int x, int y, float r, float g, float b) {
    if(r >= 1) r = 1;
    if(g >= 1) g = 1;
    if(b >= 1) b = 1;
    if(r < 0) b = 0;
    if(g < 0) b = 0;
    if(b < 0) b = 0;
    r = pow(r, 1/2.2);
    g = pow(g, 1/2.2);
    b = pow(b, 1/2.2);
    Uint32 pixel = SDL_MapRGB(m_surface->format, (Uint8) (r*255), (Uint8) (g*255), (Uint8) (b*255));
    int bpp = m_surface->format->BytesPerPixel;
    Uint8* p = (Uint8 *)m_surface->pixels + y * m_surface->pitch + x * bpp;
    *(Uint32 *)p = pixel;
}

void ScreenPainter::display() {
    m_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
    SDL_FreeSurface(m_surface);
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
}

void ScreenPainter::wait() const {
    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_WaitEventTimeout(&e, 10) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }
}

void ScreenPainter::clear() {
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
