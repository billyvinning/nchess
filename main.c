#include <stdlib.h>
#include <stdio.h>

#include "board.h"
//#include "pieces.h"


int  main(void) {
    board b = {};
    init_board(b);
    print_board(b);
    return EXIT_SUCCESS;
}
