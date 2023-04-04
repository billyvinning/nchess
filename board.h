#ifndef BOARD_H
#define BOARD_H

#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include <math.h>

#include "pieces.h"
#include "players.h"
#include "rules.h"
#include "geom.h"
#include "game.h"


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


void reset_enpassent(int piece, int * flags) {
    
    if (*flags & CAN_ENPASSENT_APAWN) *flags ^= CAN_ENPASSENT_APAWN;
    if (*flags & CAN_ENPASSENT_BPAWN) *flags ^= CAN_ENPASSENT_BPAWN;
    if (*flags & CAN_ENPASSENT_CPAWN) *flags ^= CAN_ENPASSENT_CPAWN;
    if (*flags & CAN_ENPASSENT_DPAWN) *flags ^= CAN_ENPASSENT_DPAWN;
    if (*flags & CAN_ENPASSENT_EPAWN) *flags ^= CAN_ENPASSENT_EPAWN;
    if (*flags & CAN_ENPASSENT_FPAWN) *flags ^= CAN_ENPASSENT_FPAWN;
    if (*flags & CAN_ENPASSENT_GPAWN) *flags ^= CAN_ENPASSENT_GPAWN;
    if (*flags & CAN_ENPASSENT_HPAWN) *flags ^= CAN_ENPASSENT_HPAWN;

    int p = get_piece_owner(piece); 
    if (p == WHITE && (*flags & WHITE_CAN_ENPASSENT)) {
        *flags = *flags ^ WHITE_CAN_ENPASSENT;
    } else if (
        p == BLACK && (*flags & BLACK_CAN_ENPASSENT)
    ) {
        *flags = *flags ^ BLACK_CAN_ENPASSENT;
    }
}


void make_regular_move(board b, int x1, int y1, int x2, int y2, int * game_meta) { 
    int piece_owner = get_piece_owner(b[y1][x1]);
    if ( // Handle pawn promotion.
        get_piece_type(b[y1][x1]) == PAWN && (
            (piece_owner == WHITE && y2 == BLACK_BACK_RANK) ||
            (piece_owner == BLACK && y2 == WHITE_BACK_RANK)
        )
    ) 
        b[y2][x2] = QUEEN | piece_owner;
    else
        b[y2][x2] = b[y1][x1];
    
    reset_enpassent(b[y1][x1], game_meta);
    if (get_piece_type(b[y1][x1]) == PAWN && abs(y2 - y1) == 2) {
        int file_flag = get_enpassent_flag(x1);
        int player_flag;
        if (piece_owner == WHITE)
            player_flag = BLACK_CAN_ENPASSENT;
        else
            player_flag = WHITE_CAN_ENPASSENT;
        *game_meta |= file_flag | player_flag;
    }
    
    b[y1][x1] = EMPTY_SQUARE;
}


void make_enpassent_move(board b, int x1, int y1, int x2, int y2, int * game_meta) {
    reset_enpassent(b[y1][x1], game_meta);
    b[y2][x2] = b[y1][x1];
    b[y1][x1] = EMPTY_SQUARE;
    b[y1][x2] = EMPTY_SQUARE; // Remove passed pawn.
}


void make_castling_move(board b, int x1, int y1, int x2, int y2, int * game_meta) {
    reset_enpassent(b[y1][x1], game_meta);
    b[y2][x2] = b[y1][x1];
    if (x2 == N_FILES - 2) { // Short castle.
        b[y2][x2 - 1] = b[y2][N_FILES - 1];
        b[y2][N_FILES - 1] = EMPTY_SQUARE;
    }
    else if (x2 == 2) { // Long castle.
        b[y2][x2 + 1] = b[y2][0];
        b[y2][0] = EMPTY_SQUARE;
    }
}



bool make_move(board b, int x1, int y1, int x2, int y2, int * game_meta) {
    int piece = b[y1][x1];
    switch(is_valid_move(b, x1, y1, x2, y2, game_meta)) {
        case INVALID_MOVE:
            return false;
        case REGULAR_MOVE:
            make_regular_move(b, x1, y1, x2, y2, game_meta);
            break;
        case CASTLING:
            make_castling_move(b, x1, y1, x2, y2, game_meta);
            break;
        case ENPASSENT:
            make_enpassent_move(b, x1, y1, x2, y2, game_meta);
            break;
        default:
            return false;
    }
    return true;
}





#endif
