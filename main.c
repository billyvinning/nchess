#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "game.h"
#include "gui.h"

int main(void) {
    Game game = make_game();
    run_gui(&game);
    return EXIT_SUCCESS;
}
