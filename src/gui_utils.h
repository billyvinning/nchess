#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

bool is_white_square(int i, int j) { return i % 2 ^ j % 2; }

bool init_gui(void) {
    setlocale(LC_ALL, "");
    initscr();
    // if (has_colors() == FALSE) {
    //     endwin();
    //    printf("Your terminal does not support color!\n");
    //     return false;
    // }
    // start_color();
    cbreak(); /* Start curses mode              */
    noecho();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);
    curs_set(0);
    return true;
}

#endif
