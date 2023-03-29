#ifndef PIECES_H
#define PIECES_H
#define _WHITE_PIECE_REGISTER 1
#define _BLACK_PIECE_REGISTER 10

#include <stdbool.h>
#include "players.h"


typedef enum {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5
} PieceID;



typedef enum {
    WHITE_PAWN = _WHITE_PIECE_REGISTER + PAWN,
    WHITE_KNIGHT = _WHITE_PIECE_REGISTER + KNIGHT,
    WHITE_BISHOP = _WHITE_PIECE_REGISTER + BISHOP,
    WHITE_ROOK = _WHITE_PIECE_REGISTER + ROOK,
    WHITE_QUEEN = _WHITE_PIECE_REGISTER + QUEEN,
    WHITE_KING = _WHITE_PIECE_REGISTER + KING,
    BLACK_PAWN = _BLACK_PIECE_REGISTER + PAWN,
    BLACK_KNIGHT = _BLACK_PIECE_REGISTER + KNIGHT,
    BLACK_BISHOP = _BLACK_PIECE_REGISTER + BISHOP,
    BLACK_ROOK = _BLACK_PIECE_REGISTER + ROOK,
    BLACK_QUEEN = _BLACK_PIECE_REGISTER + QUEEN,
    BLACK_KING = _BLACK_PIECE_REGISTER + KING,
} BoardPieceID;


bool is_white_piece(BoardPieceID p) {
    return p < _BLACK_PIECE_REGISTER && p >= _WHITE_PIECE_REGISTER;
}


int piece_to_board(Player p, PieceID i) {
    int offset;
    switch (p) {
        case WHITE:
            offset = _WHITE_PIECE_REGISTER;
            break;
        case BLACK:
            offset = _BLACK_PIECE_REGISTER;
            break;
    }
    return i + offset;
} 


int board_to_piece(BoardPieceID i) {
    int offset;
    if (is_white_piece(i)) {
        offset = _WHITE_PIECE_REGISTER;
    } else {
        offset = _BLACK_PIECE_REGISTER;
    }
    return i - offset;
}


char get_piece_repr(PieceID p) {
    switch (p) {
        case PAWN:
            return 'p';
        case KNIGHT:
            return 'k';
        case BISHOP:
            return 'B';
        case ROOK:
            return 'R';
        case QUEEN:
            return 'Q';
        case KING:
            return 'K';
    }
    return ' ';
}

//
//typedef struct {
//    PieceID pieceid; 
//    char label;
//}
//Piece;
//
//
//const Piece Pawn = {PAWN, "p"};
//
//const Piece Knight = {KNIGHT, "h"};
//
//const Piece Bishop = {BISHOP, "b"};
//
//const Piece Rook = {ROOK, "r"};
//
//const Piece Queen = {QUEEN, "q"};
//
//static const Piece King = {KING, "k"};
//

void get_valid_pawn_moves();

void get_valid_knight_moves();

void get_valid_bishop_moves();

void get_valid_rook_moves();

void get_valid_queen_moves();

void get_valid_king_moves();

#endif
