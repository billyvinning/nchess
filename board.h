#ifndef BOARD_H
#define BOARD_H

#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include "pieces.h"
#include "players.h"

#define N_RANKS 8
#define N_FILES 8


typedef unsigned short int board[N_FILES][N_RANKS];


void init_board(board b) {
    int black_back_rank = 0;
    int white_back_rank = N_RANKS - 1;
    for (int i = 0; i < N_FILES; i++) {
        b[black_back_rank + 1][i] = BLACK_PAWN;
        b[white_back_rank - 1][i] = WHITE_PAWN;
    }
    b[black_back_rank][0] = b[black_back_rank][N_FILES - 1] = BLACK_ROOK;
    b[black_back_rank][1] = b[black_back_rank][N_FILES - 2] = BLACK_KNIGHT;
    b[black_back_rank][2] = b[black_back_rank][N_FILES - 3] = BLACK_BISHOP;
    b[black_back_rank][3] = BLACK_QUEEN;
    b[black_back_rank][4] = BLACK_KING;

    b[white_back_rank][0] = b[white_back_rank][N_FILES - 1] = WHITE_ROOK;
    b[white_back_rank][1] = b[white_back_rank][N_FILES - 2] = WHITE_KNIGHT;
    b[white_back_rank][2] = b[white_back_rank][N_FILES - 3] = WHITE_BISHOP;
    b[white_back_rank][3] = WHITE_QUEEN;
    b[white_back_rank][4] = WHITE_KING;
}


bool is_white_square(int i, int j) {
    return i % 2 ^ j % 2;
}

#endif
