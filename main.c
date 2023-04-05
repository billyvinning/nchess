#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "game.h"
#include "gui.h"

int main(void) {
    board b = {};
    init_board(b);
    int game_meta = init_game();
    init_gui(b, &game_meta);
    return EXIT_SUCCESS;
}
