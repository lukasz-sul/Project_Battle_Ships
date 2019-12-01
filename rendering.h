#ifndef RENDERING_H_
#define RENDERING_H_

void set_colors();
double mymin(double a, double b);
void render_grid(SDL_Renderer *renderer, const SDL_Color *color);
void render_empty_shot(SDL_Renderer *renderer, int row, int column, int player);
void render_ship(SDL_Renderer *renderer, int row, int column, int player, const SDL_Color *color);
void render_ship_shot(SDL_Renderer *renderer, int row, int column, int player, const SDL_Color *color);
void render_player_board(SDL_Renderer *renderer, const enum board_state *board);
void render_computer_board(SDL_Renderer *renderer, const enum board_state *board);
void render_set_up_state(SDL_Renderer *renderer, const game_t *game);
void render_running_state(SDL_Renderer *renderer, const game_t *game);
void render_game_over_state(SDL_Renderer *rednerer, const game_t *game, const SDL_Color *color);
void render_game(SDL_Renderer *renderer, const game_t *game);
void render_on_screen(SDL_Renderer *renderer, const game_t *game);

#endif /* RENDERING_H_ */
