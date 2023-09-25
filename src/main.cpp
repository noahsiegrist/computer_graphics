#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void SetPixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32 *)p = pixel;

}

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing sdl2: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Gradient Nebula", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_WIDTH, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* gradient = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float factor = (float)x / SCREEN_WIDTH;
        Uint8 r = 255 * pow(factor, 1/2.2);
        Uint8 g = 255 * pow(1 - factor, 1/2.2);
        Uint32 color = SDL_MapRGB(gradient->format, r, g, 0);

        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            SetPixel(gradient, x, y, color);
        }
    }

    // Convert the surface to texture
    texture = SDL_CreateTextureFromSurface(renderer, gradient);
    SDL_FreeSurface(gradient);

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
