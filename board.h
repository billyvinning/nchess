#ifndef BOARD_H
#define BOARD_H

#include <string.h>
#include <stdio.h>
#include <ncurses.h>

#include "pieces.h"
#include "players.h"
#include "rules.h"
#include "geom.h"


void init_board(board b) {
    for (int i = 0; i < N_FILES; i++) {
        b[BLACK_PAWN_ORIGIN][i] = BLACK | PAWN;
        b[WHITE_PAWN_ORIGIN][i] = WHITE | PAWN;
    }
    b[BLACK_BACK_RANK][0] = b[BLACK_BACK_RANK][N_FILES - 1] = BLACK | ROOK;
    b[BLACK_BACK_RANK][1] = b[BLACK_BACK_RANK][N_FILES - 2] = BLACK | KNIGHT;
    b[BLACK_BACK_RANK][2] = b[BLACK_BACK_RANK][N_FILES - 3] = BLACK | BISHOP;
    b[BLACK_BACK_RANK][3] = BLACK | QUEEN;
    b[BLACK_BACK_RANK][4] = BLACK | KING;

    b[WHITE_BACK_RANK][0] = b[WHITE_BACK_RANK][N_FILES - 1] = WHITE | ROOK;
    b[WHITE_BACK_RANK][1] = b[WHITE_BACK_RANK][N_FILES - 2] = WHITE | KNIGHT;
    b[WHITE_BACK_RANK][2] = b[WHITE_BACK_RANK][N_FILES - 3] = WHITE | BISHOP;
    b[WHITE_BACK_RANK][3] = WHITE | QUEEN;
    b[WHITE_BACK_RANK][4] = WHITE | KING;
}


bool is_white_square(int i, int j) {
    return i % 2 ^ j % 2;
}


bool make_move(board b, int x1, int y1, int x2, int y2) {
    if (!is_valid_move(b, x1, y1, x2, y2)) {
        return false;
    }
    b[y2][x2] = b[y1][x1];
    b[y1][x1] = EMPTY_SQUARE;
    return true;
}



#endif
