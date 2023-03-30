#include <stdlib.h>
#include <stdio.h>

#include "gui.h"
#include "board.h"

int main(void) {
    board b = {};
    init_board(b);
    init_gui(b); 
    return EXIT_SUCCESS;
}
