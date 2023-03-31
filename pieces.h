#ifndef PIECES_H
#define PIECES_H

#include <stdbool.h>
#include <math.h>
#include "players.h"


typedef enum {
    EMPTY_SQUARE = 0,
    PAWN = 4,
    KNIGHT = 8,
    BISHOP = 16,
    ROOK = 32,
    QUEEN = 64,
    KING = 128
} PieceType;


int get_piece_owner(int piece) {
    if (piece == EMPTY_SQUARE) {
        return NOOWNER;
    }
    else {
        if ((piece & WHITE) == WHITE) {
            return WHITE;
        }
        else {
            return BLACK;
        }
    }
}


int get_piece_type(int piece) {
    if (piece == EMPTY_SQUARE) {
        return EMPTY_SQUARE;
    }
    return piece ^ get_piece_owner(piece);
}


int get_piece(PieceType type, Player player) {
    if (type == EMPTY_SQUARE || player == NOOWNER) {
        return EMPTY_SQUARE;
    }
    return player & type;
}


char get_piece_repr(PieceType type) {
    switch (type) {
        case EMPTY_SQUARE:
            return ' ';
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
        default:
            return '!';
    }
}


bool is_valid_pawn_move(
    int x1, int y1, int x2, int y2, bool is_white_piece, bool is_taking_move
) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    int origin;
    if (is_white_piece) {
        origin = 6;
        dy = -1 * dy;
    } else {
        origin = 1;
    }
    // Must march one or two squares.
    if (y1 == origin && ((dy < 1) || (dy > 2))) {
        return false;
    } else if (y1 != origin && dy != 1) {
        return false;
    }

    if (is_taking_move && dx == 0) {
        return false;
    } else if (!is_taking_move && dx != 0) {
        return false;
    }

    return true;
}

bool is_valid_knight_move(
    int x1, int y1, int x2, int y2 
) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    return (dx == 2 && dy == 1) ^ (dx == 1 && dy == 2);
}


bool is_valid_bishop_move(
    int x1, int y1, int x2, int y2
) {
    return abs(x2 - x1) == abs(y2 - y1);
}


bool is_valid_rook_move(
    int x1, int y1, int x2, int y2
) {
    return (abs(x2 - x1) > 0) ^ (abs(y2 - y1) > 0);
}


bool is_valid_queen_move(
    int x1, int y1, int x2, int y2
) {
    return is_valid_bishop_move(x1, y1, x2, y2) || is_valid_rook_move(x1, y1, x2, y2);
}


bool is_valid_king_move(
    int x1, int y1, int x2, int y2
) {
    return abs(x2 - x1) == 1 || abs(y2 - y1) == 1;
}


bool is_valid_move(int piece_1, int piece_2, int x1, int y1, int x2, int y2) {
    // Cannot move empty square.
    if (piece_1 == EMPTY_SQUARE) {
        return false;
    }

    int piece_1_owner = get_piece_owner(piece_1);
    int piece_2_owner = get_piece_owner(piece_2);
    // Cannot move piece onto own piece.
    if (piece_1_owner == piece_2_owner) {
        return false;
    }

    bool is_taking_move = piece_2 != EMPTY_SQUARE;
    // Cannot take a king.
    if (is_taking_move && get_piece_type(piece_2) == KING) {
        return false;
    }
    int piece_1_type = get_piece_type(piece_1);
    switch (piece_1_type) {
        case PAWN:
            return is_valid_pawn_move(x1, y1, x2, y2, piece_1_owner == WHITE, is_taking_move);
        case KNIGHT:
            return is_valid_knight_move(x1, y1, x2, y2);
        case BISHOP:
            return is_valid_bishop_move(x1, y1, x2, y2);
        case ROOK:
            return is_valid_rook_move(x1, y1, x2, y2);
        case QUEEN:
            return is_valid_queen_move(x1, y1, x2, y2);
        case KING:
            return is_valid_king_move(x1, y1, x2, y2);
        default: 
            return false;
    }
    return true;
}

#endif
