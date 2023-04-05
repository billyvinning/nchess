#ifndef GAME_H
#define GAME_H

#include "geom.h"
#include <stdio.h>

typedef enum {
    NULL_META = 0x00,
    WHITES_TURN = 0x01,
    BLACKS_TURN = 0x02,
    WHITE_IN_CHECK = 0x04,
    BLACK_IN_CHECK = 0x08,
    WHITE_CAN_CASTLE = 0x10,
    BLACK_CAN_CASTLE = 0x20,
    WHITE_CAN_ENPASSANT = 0x40,
    BLACK_CAN_ENPASSANT = 0x80,
    CAN_ENPASSANT_APAWN = 0x100,
    CAN_ENPASSANT_BPAWN = 0x200,
    CAN_ENPASSANT_CPAWN = 0x400,
    CAN_ENPASSANT_DPAWN = 0x800,
    CAN_ENPASSANT_EPAWN = 0x1000,
    CAN_ENPASSANT_FPAWN = 0x2000,
    CAN_ENPASSANT_GPAWN = 0x4000,
    CAN_ENPASSANT_HPAWN = 0x8000
} GameMeta;

static const int ENPASSANT_FILES[] = {CAN_ENPASSANT_APAWN, CAN_ENPASSANT_BPAWN,
                                      CAN_ENPASSANT_CPAWN, CAN_ENPASSANT_DPAWN,
                                      CAN_ENPASSANT_EPAWN, CAN_ENPASSANT_FPAWN,
                                      CAN_ENPASSANT_GPAWN, CAN_ENPASSANT_HPAWN};

int get_enpassant_flag(int file) { return ENPASSANT_FILES[file]; }

int get_enpassant_file(GameMeta flag) {
    for (int i = 0; i < N_FILES; i++) {
        if (flag & ENPASSANT_FILES[i])
            return i;
    }
    return -1;
}

int init_game(void) {
    return WHITES_TURN | WHITE_CAN_CASTLE | BLACK_CAN_CASTLE;
}

void position_to_string(int x, int y, char *out, int buffersize) {
    static const char rank_labels[] = "abcdefgh";
    if (buffersize != 3)
        return;

    sprintf(out, "%c%d", rank_labels[x], y + 1);
    out[buffersize - 1] = '\0';
}

#endif
