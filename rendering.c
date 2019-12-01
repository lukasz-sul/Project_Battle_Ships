#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "./game.h"
#include "./rendering.h"

SDL_Color GRID_COLOR;
SDL_Color SHIP_COLOR;
SDL_Color SHIP_SHOT_COLOR;

void set_colors()
{
    GRID_COLOR.r = 255;
    GRID_COLOR.g = 255;
    GRID_COLOR.b = 255;

    SHIP_COLOR.r = 50;
    SHIP_COLOR.g = 255;
    SHIP_COLOR.b = 50;

    SHIP_SHOT_COLOR.r = 255;
    SHIP_SHOT_COLOR.g = 50;
    SHIP_SHOT_COLOR.b = 50;
}

double mymin(double a, double b)
{
    if(a < b) {
        return a;
    } else if(a >= b) {
        return b;
    }
    return 0;
}

void render_grid(SDL_Renderer *renderer, const SDL_Color *color)
{
    int i;

    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);
    for(i = 1; i <= 2 * N + 2; ++i) {
            SDL_RenderDrawLine(renderer, i * CELL_WIDTH, CELL_HEIGHT, i*CELL_WIDTH, CELL_HEIGHT + GRID_HEIGHT);
    }
    for(i = 1; i <= N + 1; ++i) {
        SDL_RenderDrawLine(renderer, CELL_WIDTH, i * CELL_HEIGHT, (N + 1) * CELL_WIDTH, i * CELL_HEIGHT);
        SDL_RenderDrawLine(renderer, (N + 2) * CELL_WIDTH, i * CELL_HEIGHT, (2 * N + 2) * CELL_WIDTH, i * CELL_HEIGHT);
    }
}

void render_empty_shot(SDL_Renderer *renderer, int row, int column, int player)
{
    float half_box_side, center_x, center_y;

    half_box_side = mymin(CELL_WIDTH, CELL_HEIGHT) * 0.25;
    if(player == PLAYER) {
        center_x = CELL_WIDTH * 0.5 + (column + 1) * CELL_WIDTH;
        center_y = CELL_HEIGHT * 0.5 + (row + 1) * CELL_HEIGHT;
    } else if(player == COMPUTER){
        center_x = CELL_WIDTH * 0.5 + (column + 12) * CELL_WIDTH;
        center_y = CELL_HEIGHT * 0.5 + (row + 1) * CELL_HEIGHT;
    }

    filledCircleRGBA(renderer, center_x, center_y, half_box_side, 100, 100, 100, 255);
}

void render_ship(SDL_Renderer *renderer, int row, int column, int player, const SDL_Color *color)
{
    float center_x, center_y;

    center_x = CELL_WIDTH * 0.5 + (column + 1) * CELL_WIDTH;
    center_y = CELL_HEIGHT * 0.5 + (row + 1) * CELL_HEIGHT;

    boxRGBA(renderer, center_x - (CELL_WIDTH * 0.5), center_y - (CELL_HEIGHT * 0.5), center_x + (CELL_WIDTH * 0.5), center_y + (CELL_HEIGHT * 0.5), color->r, color->g, color->b, 100);

    thickLineRGBA(renderer, center_x - (CELL_WIDTH * 0.5), center_y - (CELL_HEIGHT * 0.5), center_x + (CELL_WIDTH * 0.5), center_y - (CELL_HEIGHT * 0.5), 2, color->r, color->g, color->b, 255);
    thickLineRGBA(renderer, center_x - (CELL_WIDTH * 0.5), center_y + (CELL_HEIGHT * 0.5), center_x + (CELL_WIDTH * 0.5), center_y + (CELL_HEIGHT * 0.5), 2, color->r, color->g, color->b, 255);
    thickLineRGBA(renderer, center_x - (CELL_WIDTH * 0.5), center_y - (CELL_HEIGHT * 0.5), center_x - (CELL_WIDTH * 0.5), center_y + (CELL_HEIGHT * 0.5), 2, color->r, color->g, color->b, 255);
    thickLineRGBA(renderer, center_x + (CELL_WIDTH * 0.5), center_y - (CELL_HEIGHT * 0.5), center_x + (CELL_WIDTH * 0.5), center_y + (CELL_HEIGHT * 0.5), 2, color->r, color->g, color->b, 255);
}

void render_ship_shot(SDL_Renderer *renderer, int row, int column, int player, const SDL_Color *color)
{
    float center_x, center_y;

    if(player == PLAYER) {
        center_x = CELL_WIDTH * 0.5 + (column + 1) * CELL_WIDTH;
        center_y = CELL_HEIGHT * 0.5 + (row + 1) * CELL_HEIGHT;
    } else if(player == COMPUTER){
        center_x = CELL_WIDTH * 0.5 + (column + 12) * CELL_WIDTH;
        center_y = CELL_HEIGHT * 0.5 + (row + 1) * CELL_HEIGHT;
    }
    boxRGBA(renderer, center_x - (CELL_WIDTH * 0.5), center_y - (CELL_HEIGHT * 0.5), center_x + (CELL_WIDTH * 0.5), center_y + (CELL_HEIGHT * 0.5), color->r, color->g, color->b, 100);

    thickLineRGBA(renderer, center_x - (CELL_WIDTH * 0.5), center_y - (CELL_HEIGHT * 0.5), center_x + (CELL_WIDTH * 0.5), center_y - (CELL_HEIGHT * 0.5), 2, color->r, color->g, color->b, 255);
    thickLineRGBA(renderer, center_x - (CELL_WIDTH * 0.5), center_y + (CELL_HEIGHT * 0.5), center_x + (CELL_WIDTH * 0.5), center_y + (CELL_HEIGHT * 0.5), 2, color->r, color->g, color->b, 255);
    thickLineRGBA(renderer, center_x - (CELL_WIDTH * 0.5), center_y - (CELL_HEIGHT * 0.5), center_x - (CELL_WIDTH * 0.5), center_y + (CELL_HEIGHT * 0.5), 2, color->r, color->g, color->b, 255);
    thickLineRGBA(renderer, center_x + (CELL_WIDTH * 0.5), center_y - (CELL_HEIGHT * 0.5), center_x + (CELL_WIDTH * 0.5), center_y + (CELL_HEIGHT * 0.5), 2, color->r, color->g, color->b, 255);

    thickLineRGBA(renderer, center_x - (CELL_WIDTH * 0.5), center_y - (CELL_HEIGHT * 0.5), center_x + (CELL_WIDTH * 0.5), center_y + (CELL_HEIGHT * 0.5), 2, color->r, color->g, color->b, 255);
    thickLineRGBA(renderer, center_x + (CELL_WIDTH * 0.5), center_y - (CELL_HEIGHT * 0.5), center_x - (CELL_WIDTH * 0.5), center_y + (CELL_HEIGHT * 0.5), 2, color->r, color->g, color->b, 255);
}

void render_player_board(SDL_Renderer *renderer, const enum board_state *board)
{
    int i, j;
    for(i = 0; i < N; ++i) {
        for(j = 0; j < N; ++j) {
            switch(board[i * N + j]) {
            case EMPTY_SHOT:
                render_empty_shot(renderer, i, j, PLAYER);
                break;

            case SHIP:
                render_ship(renderer, i, j, PLAYER, &SHIP_COLOR);
                break;

            default: {}
            }
        }
    }
    for(i = 0; i < N; ++i) {
        for(j = 0; j < N; ++j) {
            if(board[i * N + j] == SHIP_SHOT) {
                render_ship_shot(renderer, i, j, PLAYER, &SHIP_SHOT_COLOR);
            }
        }
    }
}

void render_computer_board(SDL_Renderer *renderer, const enum board_state *board)
{
    int i, j;
    for(i = 0; i < N; ++i) {
        for(j = 0; j < N; ++j) {
            switch(board[i * N + j]) {
            case EMPTY_SHOT:
                render_empty_shot(renderer, i, j, COMPUTER);
                break;

            case SHIP_SHOT:
                render_ship_shot(renderer, i, j, COMPUTER, &SHIP_SHOT_COLOR);
                break;

            default: {}
            }
        }
    }
}

void render_grid_2(SDL_Renderer *renderer, const SDL_Color *color)
{
    int i;

    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);
    for(i = 1; i <= N + 1; ++i) {
            SDL_RenderDrawLine(renderer, i * CELL_WIDTH, CELL_HEIGHT, i*CELL_WIDTH, CELL_HEIGHT + GRID_HEIGHT);
    }
    for(i = 1; i <= N + 1; ++i) {
        SDL_RenderDrawLine(renderer, CELL_WIDTH, i * CELL_HEIGHT, (N + 1) * CELL_WIDTH, i * CELL_HEIGHT);
    }
}

void render_choose_ship(SDL_Renderer *renderer, int ships){
    int i;
    if(ships < 2){
        for(i = 0; i < 4; i++) {
            render_ship(renderer, 4, 14+i, PLAYER, &SHIP_COLOR);
        }
    }
    else if(ships < 4){
        for(i = 0; i < 3; i++) {
            render_ship(renderer, 4, 14+i, PLAYER, &SHIP_COLOR);
        }
    }
    else if(ships < 7){
        for(i = 0; i < 2; i++) {
            render_ship(renderer, 4, 15+i, PLAYER, &SHIP_COLOR);
        }
    }
    else{
        render_ship(renderer, 4, 15, PLAYER, &SHIP_COLOR);
    }
}

void render_set_up_state(SDL_Renderer *renderer, const game_t *game)
{
    render_grid_2(renderer, &GRID_COLOR);
    render_player_board(renderer, game->player_board);
    render_choose_ship(renderer, game->ships);
}

void render_running_state(SDL_Renderer *renderer, const game_t *game)
{
    render_grid(renderer, &GRID_COLOR);
    render_player_board(renderer, game->player_board);
    render_computer_board(renderer, game->computer_board);
}

void render_game_over_state(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color)
{
    render_grid(renderer, color);
    render_player_board(renderer, game->player_board);
    render_computer_board(renderer, game->computer_board);
}

void render_game(SDL_Renderer *renderer, const game_t *game)
{
    switch (game->state) {
    case SET_UP_STATE:
        render_set_up_state(renderer, game);
        break;

    case RUNNING_STATE:
        render_running_state(renderer, game);
        break;

    case PLAYER_WON_STATE:
        render_game_over_state(renderer, game, &SHIP_COLOR);
        break;

    case COMPUTER_WON_STATE:
        render_game_over_state(renderer, game, &SHIP_SHOT_COLOR);
        break;

    default: {}
    }

}

void render_on_screen(SDL_Renderer *renderer, const game_t *game)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    render_game(renderer, game);
    SDL_RenderPresent(renderer);
}
