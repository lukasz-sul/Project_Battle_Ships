#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "./game.h"
#include "./logic.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int fit_a_ship(enum board_state *board, int row, int column, int lenght, int vertical)
{
    int i, j, k;
    int success;

    success = 1;

    if(vertical == 1) {
        for(i = row; i < row + lenght; ++i) {
            if( board[i * N + column] != EMPTY) {
                success = 0;
                break;
            }
        }

        if(success == 1) {
            i = row - 1;
            j = column - 1;

            if(i < 0) {
                i++;
            }
            if(j < 0) {
                j++;
            }

            for(; i <= row + lenght && i < N; ++i) {
                for(k = j; k <= column + 1 && k < N; ++k) {
                    board[i * N + k] = EMPTY_SHOT;
                }
            }

            for(i = row; i < row + lenght; ++i) {
                board[i * N + column] = SHIP;
            }

            return 1;
        } else {
            return 0;
        }
    } else if(vertical == 0) {
        for(i = column; i < column + lenght; ++i) {
            if( board[row * N + i] != EMPTY) {
                success = 0;
                break;
            }
        }

        if(success == 1) {
            i = row - 1;
            j = column - 1;

            if(i < 0) {
                i++;
            }
            if(j < 0) {
                j++;
            }

            for(; i <= row + 1 && i < N; ++i) {
                for(k = j; k <= column + lenght && k < N; ++k) {
                    board[i * N + k] = EMPTY_SHOT;
                }
            }

            for(i = column; i < column + lenght; ++i) {
                board[row * N + i] = SHIP;
            }

            return 1;
        } else {
            return 0;
        }

    }
}

void computer_set_up_board(enum board_state *board)
{
    int i;
    int j;
    int row;
    int column;
    int vertical;
    int lenght;
    int success;

    srand(time(NULL));

    for(i = 0; i < 10; ++i) {
        switch(i) {
        case 0:
            lenght = 4;
            break;

        case 1:
            lenght = 3;
            break;

        case 3:
            lenght = 2;
            break;

        case 6:
            lenght = 1;
            break;

        default: {}
        }

        success = 0;

        while(!success) {
            vertical = rand()%2;
            if(vertical == 1) {
                row = rand()%(10 - lenght);
                column = rand()%10;
            } else if(vertical == 0) {
                row = rand()%10;
                column = rand()%(10 - lenght);
            }

            if(1 == fit_a_ship(board, row, column, lenght, vertical)) {
                success = 1;
            }
        }
    }

    for(i = 0; i < N * N; ++i) {
        if(board[i] == EMPTY_SHOT){
            board[i] = EMPTY;
        }
    }
}

void reset_game(game_t *game)
{
    int i;

    game->player = PLAYER;
    game->state = SET_UP_STATE;
    game->ships = 0;
    game->turn = 0;

    for(i = 0; i < N * N; ++i) {
        game->computer_board[i] = EMPTY;
        game->player_board[i] = EMPTY;
    }

    computer_set_up_board(game->computer_board);
}

int is_empty(enum board_state *board, int row, int column)
{
    int i, j, k, counter;

    i = row - 1;
    j = column - 1;
    counter = 0;

    if(i < 0) {
        i++;
    }
    if(j < 0) {
        j++;
    }
    for(i; i <= row + 1 && i < N; ++i) {
        for(k = j; k <= column + 1 && k < N; ++k) {
            if(board[i * N + k] != EMPTY) {
                counter++;
            }
        }
    }

    return counter;
}

void remove_block(enum board_state *board, int *blocks, int *where, int row, int column)
{
    int i, j;

    if(*blocks > 0 && (1 == is_empty(board, row, column) || 2 == is_empty(board, row, column))) {
        for(i = 0; i < *blocks; ++i) {
            if(where[2 * i] == row && where[2 * i + 1] == column) {
                for(j = i + 1; j < *blocks; ++j) {
                    where[2 * (j-1)] = where[2 * j];
                    where[2 * j - 1] = where[2 * j + 1];
                }
                board[row * N + column] = EMPTY;
                (*blocks)--;
                break;
            }
        }
    }
}

void add_block(enum board_state *board, int *blocks, int *ships, int *where, int row, int column, int lenght)
{
    int i;

    if(*blocks == 0 && 0 == is_empty(board, row, column)) {
        board[row * N + column] = SHIP;
        where[0] = row;
        where[1] = column;
        (*blocks)++;
    } else if(1 == is_empty(board, row, column)){
        for(i = 0; i < *blocks; ++i) {
            if((where[2 * i] == row - 1 && where[2 * i + 1] == column) || (where[2 * i] == row && where[2 * i + 1] == column + 1) || (where[2 * i] == row + 1 && where[2 * i + 1] == column) || (where[2 * i] == row && where[2 * i + 1] == column - 1)) {
                board[row * N + column] = SHIP;
                where[2 * (*blocks)] = row;
                where[2 * (*blocks) + 1] = column;
                (*blocks)++;
            }
        }
    }

    if(*blocks == lenght) {
        *blocks = 0;
        (*ships)++;
    }
}

void player_set_up_board(enum board_state *board, int *ships, int row, int column, enum game_state *state)
{
    static int blocks;
    static int where[8];
    int i, counter;

    if(*ships == 0) {
        blocks = 0;
        *ships = 1;
    }

    if(board[row * N + column] != EMPTY) {
        remove_block(board, &blocks, where, row, column);
    } else if(board[row * N + column] == EMPTY) {
        if(*ships == 1) {
            add_block(board, &blocks, ships, where, row, column, 4);
        } else if(*ships <= 3) {
            add_block(board, &blocks, ships, where, row, column, 3);
        } else if(*ships <= 6) {
            add_block(board, &blocks, ships, where, row, column, 2);
        } else if(*ships <= 10) {
            add_block(board, &blocks, ships, where, row, column, 1);
        }
    }

    if(*ships == 11) {
        *state = RUNNING_STATE;
    }
}

int is_last(enum board_state *board, int row, int column)
{
    int i, j;

    j = column;

    while(j >= 0 && board[row * N + j] != EMPTY && board[row * N + j] != EMPTY_SHOT) {
        if(board[row * N + j] == SHIP) {
            return 0;
        }
        j--;
    }

    i = row;

    while(i >= 0 && board[i * N + column] != EMPTY && board[i * N + column] != EMPTY_SHOT) {
        if(board[i * N + column] == SHIP) {
            return 0;
        }
        i--;
    }

    j = column;

    while(j < N && board[row * N + j] != EMPTY && board[row * N + j] != EMPTY_SHOT) {
        if(board[row * N + j] == SHIP) {
            return 0;
        }
        j++;
    }

    i = row;

    while(i < N && board[i * N + column] != EMPTY && board[i * N + column] != EMPTY_SHOT) {
        if(board[i * N + column] == SHIP) {
            return 0;
        }
        i++;
    }

    return 1;
}

void shoot_around(enum board_state *board, int row, int column)
{
    int top_left_row, top_left_column;
    int down_right_row, down_right_column;
    int i;

    top_left_row = row;
    down_right_row = row;
    top_left_column = column;
    down_right_column = column;

    while(top_left_row >= 0 && (board[top_left_row * N + column] == SHIP || board[top_left_row * N + column] == SHIP_SHOT)) {
        top_left_row--;
    }

    while(top_left_column >= 0 && (board[row * N + top_left_column] == SHIP || board[row * N + top_left_column] == SHIP_SHOT)) {
        top_left_column--;
    }

    while(down_right_row < N && (board[down_right_row * N + column] == SHIP || board[down_right_row * N + column] == SHIP_SHOT)) {
        down_right_row++;
    }

    while(down_right_column < N && (board[row * N + down_right_column] == SHIP || board[row * N + down_right_column] == SHIP_SHOT)) {
        down_right_column++;
    }

    if(top_left_column >= 0) {
        i = top_left_row;
        if(i < 0) {
            i++;
        }
        for(; i <= down_right_row; ++i) {
            if(i < N) {
                board[i * N + top_left_column] = EMPTY_SHOT;
            }
        }
    }
    if(down_right_column < N) {
        i = top_left_row;
        if(i < 0) {
            i++;
        }
        for(; i <= down_right_row; ++i) {
            if(i < N) {
                board[i * N + down_right_column] = EMPTY_SHOT;
            }
        }
    }
    if(top_left_row >= 0) {
        i = top_left_column + 1;

        for(; i < down_right_column; ++i) {
            if(i < N) {
                board[top_left_row * N + i] = EMPTY_SHOT;
            }
        }
    }
    if(down_right_row < N) {
        i = top_left_column + 1;

        for(; i < down_right_column; ++i) {
            if(i < N) {
                board[down_right_row * N + i] = EMPTY_SHOT;
            }
        }
    }

}

void player_shoot(enum board_state *board, int row, int column, int *player)
{
    if(board[row * N + column] == EMPTY) {
        board[row * N + column] = EMPTY_SHOT;
        *player = COMPUTER;
    } else if(board[row * N + column] == SHIP) {
        board[row * N + column] = SHIP_SHOT;

        if(1 == is_last(board, row, column)) {
            shoot_around(board, row, column);
        } else {
            if(row + 1 < N) {
                if(column + 1 < N) {
                    board[(row + 1) * N + (column + 1)] = EMPTY_SHOT;
                }
                if(column - 1 >= 0) {
                    board[(row + 1) * N + (column - 1)] = EMPTY_SHOT;
                }
            }
            if(row - 1 >= 0) {
                if(column + 1 < N) {
                    board[(row - 1) * N + (column + 1)] = EMPTY_SHOT;
                }
                if(column - 1 >= 0) {
                    board[(row - 1) * N + (column - 1)] = EMPTY_SHOT;
                }
            }
        }
    }
}

void computer_shoot(enum board_state *board, int *player, int *turn, enum game_state *state, SDL_Renderer *renderer, game_t *game)
{
    static int hit, row, column, row_2, column_2, vertical;
    int i, j, direction;
    srand(time(NULL));
    SDL_Event e;

    if(*turn == 0) {
        hit = 0;
        *turn = 1;
    }
    #ifdef _WIN32
    Sleep(500);
    #else
    usleep(500*1000);
    #endif
    while(*player == COMPUTER){
        if(hit == 0) {
            do {
                i = rand()%N;
                j = rand()%N;
            } while(board[i * N + j] != EMPTY && board[i * N + j] != SHIP);

            if(board[i * N + j] == SHIP) {
                board[i * N + j] = SHIP_SHOT;

                if(0 == is_last(board, i, j)) {
                    hit = 1;
                    row = i;
                    column = j;
                    if(i + 1 < N) {
                        if(j + 1 < N) {
                            board[(i + 1) * N + (j + 1)] = EMPTY_SHOT;
                        }
                        if(j - 1 >= 0) {
                            board[(i + 1) * N + (j - 1)] = EMPTY_SHOT;
                        }
                    }
                    if(i - 1 >= 0) {
                        if(j + 1 < N) {
                            board[(i - 1) * N + (j + 1)] = EMPTY_SHOT;
                        }
                        if(j - 1 >= 0) {
                            board[(i - 1) * N + (j - 1)] = EMPTY_SHOT;
                        }
                    }
                } else {
                    shoot_around(board, i, j);
                }
                #ifdef _WIN32
                Sleep(500);
                #else
                usleep(500*1000);
                #endif
                render_on_screen(renderer, game);

            } else if(board[i * N + j] == EMPTY) {
                board[i * N + j] = EMPTY_SHOT;
                *player = PLAYER;
                #ifdef _WIN32
                Sleep(500);
                #else
                usleep(500*1000);
                #endif
                render_on_screen(renderer, game);
            }

        } else if(hit == 1) {
            i = -1;
            j = -1;

            while(i < 0 || board[i * N + j] == EMPTY_SHOT) {
                direction = rand()%4;

                switch(direction) {
                case 0:
                    if(row - 1 >= 0) {
                        i = row - 1;
                        j = column;
                    }
                    break;
                case 1:
                    if(column + 1 < N) {
                        i = row;
                        j = column +1;
                    }
                    break;
                case 2:
                    if(row + 1 < N) {
                        i = row + 1;
                        j = column;
                    }
                    break;
                case 3:
                    if(column - 1 >= 0) {
                        i = row;
                        j = column - 1;
                    }
                    break;
                default: {}
                }
            }

            if(board[i * N + j] == SHIP) {
                board[i * N + j] = SHIP_SHOT;

                if(0 == is_last(board, i, j)) {
                    hit = 2;
                    row_2 = i;
                    column_2 = j;

                    if(row_2 != row) {
                        vertical = 1;
                    } else if(column_2 != column) {
                        vertical = 0;
                    }

                    if(i + 1 < N) {
                        if(j + 1 < N) {
                            board[(i + 1) * N + (j + 1)] = EMPTY_SHOT;
                        }
                        if(j - 1 >= 0) {
                            board[(i + 1) * N + (j - 1)] = EMPTY_SHOT;
                        }
                    }
                    if(i - 1 >= 0) {
                        if(j + 1 < N) {
                            board[(i - 1) * N + (j + 1)] = EMPTY_SHOT;
                        }
                        if(j - 1 >= 0) {
                            board[(i - 1) * N + (j - 1)] = EMPTY_SHOT;
                        }
                    }
                } else {
                    shoot_around(board, i, j);
                    hit = 0;
                }

                #ifdef _WIN32
                Sleep(500);
                #else
                usleep(500*1000);
                #endif
                render_on_screen(renderer, game);

            } else if(board[i * N + j] == EMPTY) {
                board[i * N + j] = EMPTY_SHOT;
                *player = PLAYER;
                #ifdef _WIN32
                Sleep(500);
                #else
                usleep(500*1000);
                #endif
                render_on_screen(renderer, game);
            }
        } else if(hit == 2){
            i = -1;
            j = -1;


            if(vertical == 1) {
                while(i < 0 || board[i * N + j] == EMPTY_SHOT) {
                    direction = rand()%2;

                    switch(direction) {
                    case 0:
                        if(row < row_2 && row - 1 >= 0) {
                            i = row - 1;
                            j = column;
                        } else if(row_2 < row && row_2 - 1 >= 0) {
                            i = row_2 - 1;
                            j = column_2;
                        }
                        break;
                    case 1:
                        if(row > row_2 && row + 1 < N) {
                            i = row + 1;
                            j = column;
                        } else if(row_2 > row && row_2 + 1 < N) {
                            i = row_2 + 1;
                            j = column_2;
                        }
                        break;
                    }
                }

                if(board[i * N + j] == SHIP) {
                    board[i * N + j] = SHIP_SHOT;

                    if(0 == is_last(board, i, j)) {
                        if(direction == 0 && i == row - 1) {
                            row = i;
                            column = j;
                        } else if(direction == 0 && i == row_2 - 1) {
                            row_2 = i;
                            column_2 = j;
                        } else if(direction == 1 && i == row + 1) {
                            row = i;
                            column = j;
                        } else if(direction == 1 && i == row_2 + 1) {
                            row_2 = i;
                            column_2 = j;
                        }

                        if(i + 1 < N) {
                            if(j + 1 < N) {
                                board[(i + 1) * N + (j + 1)] = EMPTY_SHOT;
                            }
                            if(j - 1 >= 0) {
                                board[(i + 1) * N + (j - 1)] = EMPTY_SHOT;
                            }
                        }
                        if(i - 1 >= 0) {
                            if(j + 1 < N) {
                                board[(i - 1) * N + (j + 1)] = EMPTY_SHOT;
                            }
                            if(j - 1 >= 0) {
                                board[(i - 1) * N + (j - 1)] = EMPTY_SHOT;
                            }
                        }
                    } else {
                        shoot_around(board, i, j);
                        hit = 0;
                    }

                    #ifdef _WIN32
                    Sleep(500);
                    #else
                    usleep(500*1000);
                    #endif
                    render_on_screen(renderer, game);

                } else if(board[i * N + j] == EMPTY) {
                    board[i * N + j] = EMPTY_SHOT;
                    *player = PLAYER;
                    #ifdef _WIN32
                    Sleep(500);
                    #else
                    usleep(500*1000);
                    #endif
                    render_on_screen(renderer, game);
                }

            } else if(vertical == 0) {
                while(i < 0 || board[i * N + j] == EMPTY_SHOT) {
                    direction = rand()%2;

                    switch(direction) {
                    case 0:
                        if(column < column_2 && column - 1 >= 0) {
                            i = row;
                            j = column - 1;
                        } else if(column_2 < column && column_2 - 1 >= 0) {
                            i = row_2;
                            j = column_2 - 1;
                        }
                        break;
                    case 1:
                        if(column > column_2 && column + 1 < N) {
                            i = row;
                            j = column + 1;
                        } else if(column_2 > column && column_2 + 1 < N) {
                            i = row_2;
                            j = column_2 + 1;
                        }
                        break;
                    }
                }
                if(board[i * N + j] == SHIP) {
                    board[i * N + j] = SHIP_SHOT;

                    if(0 == is_last(board, i, j)) {
                        if(direction == 0 && j == column - 1) {
                            row = i;
                            column = j;
                        } else if(direction == 0 && j == column_2 - 1) {
                            row_2 = i;
                            column_2 = j;
                        } else if(direction == 1 && j == column + 1) {
                            row = i;
                            column = j;
                        } else if(direction == 1 && j == column_2 + 1) {
                            row_2 = i;
                            column_2 = j;
                        }

                        if(i + 1 < N) {
                            if(j + 1 < N) {
                                board[(i + 1) * N + (j + 1)] = EMPTY_SHOT;
                            }
                            if(j - 1 >= 0) {
                                board[(i + 1) * N + (j - 1)] = EMPTY_SHOT;
                            }
                        }
                        if(i - 1 >= 0) {
                            if(j + 1 < N) {
                                board[(i - 1) * N + (j + 1)] = EMPTY_SHOT;
                            }
                            if(j - 1 >= 0) {
                                board[(i - 1) * N + (j - 1)] = EMPTY_SHOT;
                            }
                        }
                    } else {
                        shoot_around(board, i, j);
                        hit = 0;
                    }

                    #ifdef _WIN32
                    Sleep(500);
                    #else
                    usleep(500*1000);
                    #endif
                    render_on_screen(renderer, game);

                } else if(board[i * N + j] == EMPTY) {
                    board[i * N + j] = EMPTY_SHOT;
                    *player = PLAYER;
                    #ifdef _WIN32
                    Sleep(500);
                    #else
                    usleep(500*1000);
                    #endif
                    render_on_screen(renderer, game);
                }
            }
        }
        if(1 == check_end_game(board, state)) {
            *state = COMPUTER_WON_STATE;
            *player = PLAYER;
        }
        while(SDL_PollEvent(&e));
    }
}

int check_end_game(enum board_state *board, enum game_state *state)
{
    int i;

    for(i = 0; i < N * N; ++i) {
        if(board[i] == SHIP) {
            return 0;
            break;
        }
    }

    return 1;
}

void click_on_cell(game_t *game, int row, int column, SDL_Renderer *renderer)
{
    switch (game->state) {
    case SET_UP_STATE:

        if(row >= 0 && row < N && column >= 0 && column < N) {
            player_set_up_board(game->player_board, &(game->ships), row, column, &(game->state));
        }
        break;

    case RUNNING_STATE:
        if(row >= 0 && row < N && column >= N + 1 & column <= 2 * N && game->player == PLAYER) {
            player_shoot(game->computer_board, row, column - N - 1, &(game->player));
        }
        render_on_screen(renderer, game);
        if(1 == check_end_game(game->computer_board, &(game->state))) {
            game->state = PLAYER_WON_STATE;
        } else if(game->player == COMPUTER) {
            computer_shoot(game->player_board, &(game->player), &(game->turn), &(game->state), renderer, game);
        }
        break;

    case PLAYER_WON_STATE:

    case COMPUTER_WON_STATE:

        reset_game(game);
        break;

    default: {}
    }
}
