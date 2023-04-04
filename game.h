#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include "geom.h"


typedef enum {
    NULL_META = 0x00,
    WHITES_TURN = 0x01,
    BLACKS_TURN = 0x02,
    WHITE_IN_CHECK = 0x04,
    BLACK_IN_CHECK = 0x08,
    WHITE_CAN_CASTLE = 0x10,
    BLACK_CAN_CASTLE = 0x20,
    WHITE_CAN_ENPASSENT = 0x40,
    BLACK_CAN_ENPASSENT = 0x80,
    CAN_ENPASSENT_APAWN = 0x100,
    CAN_ENPASSENT_BPAWN = 0x200,
    CAN_ENPASSENT_CPAWN = 0x400,
    CAN_ENPASSENT_DPAWN = 0x800,
    CAN_ENPASSENT_EPAWN = 0x1000,
    CAN_ENPASSENT_FPAWN = 0x2000,
    CAN_ENPASSENT_GPAWN = 0x4000,
    CAN_ENPASSENT_HPAWN = 0x8000
} GameMeta; 


static const int ENPASSENT_FILES[] = {
    CAN_ENPASSENT_APAWN,
    CAN_ENPASSENT_BPAWN,
    CAN_ENPASSENT_CPAWN,
    CAN_ENPASSENT_DPAWN,
    CAN_ENPASSENT_EPAWN,
    CAN_ENPASSENT_FPAWN,
    CAN_ENPASSENT_GPAWN,
    CAN_ENPASSENT_HPAWN
};


int get_enpassent_flag(int file) {
    return ENPASSENT_FILES[file];
}

int get_enpassent_file(GameMeta flag) {
    for (int i = 0; i < N_FILES; i++) {
        if (flag & ENPASSENT_FILES[i])
            return i;
    }
    return -1;
}


int init_game(void) {
    return WHITES_TURN | WHITE_CAN_CASTLE | BLACK_CAN_CASTLE;
}


void position_to_string(int x, int y, char * out, int buffersize) {
    static const char rank_labels[] = "abcdefgh";
    if (buffersize != 3)
        return;

    sprintf(out, "%c%d", rank_labels[x], y + 1);
    out[buffersize - 1] = '\0';
}




#endif
