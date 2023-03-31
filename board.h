#ifndef BOARD_H
#define BOARD_H

#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include "pieces.h"
#include "players.h"

#define N_RANKS 8
#define N_FILES 8


typedef short int board[N_FILES][N_RANKS];


void init_board(board b) {
    int black_back_rank = 0;
    int white_back_rank = N_RANKS - 1;
    for (int i = 0; i < N_FILES; i++) {
        b[black_back_rank + 1][i] = BLACK | PAWN;
        b[white_back_rank - 1][i] = WHITE | PAWN;
    }
    b[black_back_rank][0] = b[black_back_rank][N_FILES - 1] = BLACK | ROOK;
    b[black_back_rank][1] = b[black_back_rank][N_FILES - 2] = BLACK | KNIGHT;
    b[black_back_rank][2] = b[black_back_rank][N_FILES - 3] = BLACK | BISHOP;
    b[black_back_rank][3] = BLACK | QUEEN;
    b[black_back_rank][4] = BLACK | KING;

    b[white_back_rank][0] = b[white_back_rank][N_FILES - 1] = WHITE | ROOK;
    b[white_back_rank][1] = b[white_back_rank][N_FILES - 2] = WHITE | KNIGHT;
    b[white_back_rank][2] = b[white_back_rank][N_FILES - 3] = WHITE | BISHOP;
    b[white_back_rank][3] = WHITE | QUEEN;
    b[white_back_rank][4] = WHITE | KING;
}


bool is_white_square(int i, int j) {
    return i % 2 ^ j % 2;
}


bool make_move(board b, int x1, int y1, int x2, int y2) {
    if (!is_valid_move(b[y1][x1], b[y2][x2], x1, y1, x2, y2)) {
        return false;
    }
    b[y2][x2] = b[y1][x1];
    b[y1][x1] = EMPTY_SQUARE;
    return true;
}



#endif
