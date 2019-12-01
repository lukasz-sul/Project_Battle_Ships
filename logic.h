#ifndef LOGIC_H_
#define LOGIC_H_

int fit_a_ship(enum board_state *board, int row, int column, int lenght, int vertical);
void computer_set_up_board(enum board_state *board);
void reset_game(game_t *game);
int is_empty(enum board_state *board, int row, int column);
void remove_block(enum board_state *board, int *blocks, int *where, int row, int column);
void add_block(enum board_state *board, int *blocks, int *ships, int *where, int row, int column, int lenght);
void player_set_up_board(enum board_state *board, int *ships, int row, int column, enum game_state *state);
int is_last(enum board_state *board, int row, int column);
void shoot_around(enum board_state *board, int row, int column);
void player_shoot(enum board_state *board, int row, int column, int *player);
void computer_shoot(enum board_state *board, int *player, int *turn, enum game_state *state, SDL_Renderer *renderer, game_t *game);
int check_end_game(enum board_state *board, enum game_state *state);
void click_on_cell(game_t *game, int row, int column, SDL_Renderer *renderer);


#endif /* LOGIC_H_ */
