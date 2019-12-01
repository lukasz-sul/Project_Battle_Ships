#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "./game.h"
#include "./logic.h"
#include "./rendering.h"

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event e;
    game_t game;
    int i, cell_height, cell_width;

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Inicjalizacja sdl2 nie powiodla sie: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow(  "Okrety",
                                100,
                                100,
                                SCREEN_WIDTH,
                                SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);
    if(window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(  window,
                                    -1,
                                    SDL_RENDERER_ACCELERATED |
                                    SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    reset_game(&game);
    cell_height = CELL_HEIGHT;
    cell_width = CELL_WIDTH;
    set_colors();

    while(game.state != QUIT_STATE) {
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_QUIT:
                game.state = QUIT_STATE;
                break;

            case SDL_MOUSEBUTTONDOWN:
                click_on_cell(&game, (e.button.y / cell_height) - 1, (e.button.x / cell_width) - 1, renderer);
                break;

            default: {}
            }
        }

        render_on_screen(renderer, &game);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
