#ifndef BOARD_H
#define BOARD_H

#include <string.h>
#include <stdio.h>
#include "pieces.h"
#include "players.h"

#define N_RANKS 8
#define N_FILES 8

#define ANSI_WHITE_ESC "\33[38;5;0;48;5;255m"
#define ANSI_BLACK_ESC "\33[38;5;255;48;5;0m"
#define ANSI_RESET  "\33[m"


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

void print_square(int boardpiece, int i, int j) {

     int piece = board_to_piece(boardpiece);
     char repr = get_piece_repr(piece);

     if (
            (boardpiece == 0 && is_white_square(i, j)) ||
            (boardpiece != 0 && is_white_piece(boardpiece))
        ) { 
            printf(ANSI_WHITE_ESC);
     } else {
        printf(ANSI_BLACK_ESC);
    }
    printf("%c", repr);
    printf(" ");
    printf(ANSI_RESET);

}

void print_board(board b) {
    for (int i = 0; i < N_RANKS + 2; i++){
        printf("--");
    }
    printf("\n");
    for (int i = 0; i < N_RANKS; i++) {
        printf("| ");
        for (int j = 0; j < N_FILES; j++) {
            //printf("%d ", b[i][j]);
            print_square(b[i][j], i, j);
        }
        printf(" |\n");
    }
    for (int i = 0; i < N_RANKS + 2; i++){
        printf("--");
    }
    printf("\n");
}

#endif
