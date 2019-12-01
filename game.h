#ifndef GAME_H_
#define GAME_H_

#define N 10
#define SCREEN_WIDTH 1380 /*1920*/
#define SCREEN_HEIGHT 720 /*960*/
#define CELL_WIDTH (SCREEN_WIDTH / ((2 * N) + 3))
#define CELL_HEIGHT (SCREEN_HEIGHT / (N + 2))
#define GRID_HEIGHT (CELL_HEIGHT * N)
#define PLAYER 1
#define COMPUTER 2

enum game_state {QUIT_STATE, SET_UP_STATE, RUNNING_STATE, PLAYER_WON_STATE, COMPUTER_WON_STATE};

enum board_state {EMPTY, EMPTY_SHOT, SHIP, SHIP_SHOT};

enum ship_type {CRUISER, DESTROYER, BATTLESHIP, CARRIER};

typedef struct {
    enum board_state player_board[N * N];
    enum board_state computer_board[N * N];
    int ships;
    int player;
    enum game_state state;
    int turn;
} game_t;

#endif /* GAME_H_ */
