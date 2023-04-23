#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "game_gui.h"
#include "gui_utils.h"
#include "main_gui.h"

int main(void) {
    init_gui();
    int code = run_main_gui();
    switch (code) {
    case ONE_PLAYER:
        break;
    case TWO_PLAYER:
        clear();
        refresh();
        Game g = make_game();
        run_game_gui(&g);
        break;
    case QUIT:
        break;
    }

    endwin();
    printf("%d, %s", code, CHOICE_LABELS[code]);
    return EXIT_SUCCESS;
}
