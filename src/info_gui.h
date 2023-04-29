#ifndef INFO_GUI_H
#define INFO_GUI_H

#include "board_gui.h"
#include "gui_geom.h"
#include "ncurses.h"
#include "notation.h"
WINDOW *win_info;

void new_win_info(void) {
    int starty = (LINES - WIN_HEIGHT) / 2;
    int startx = ((COLS - WIN_WIDTH) / 2) + WIN_WIDTH + 1;
    win_info = newwin(WIN_HEIGHT, 2 * WIN_WIDTH / 3, starty, startx);
    box(win_info, 0, 0);
    wrefresh(win_info);
}

void update_info_event(Game g) {
    static char **ptr = NULL;
    static int axis_0_size = 0;
    static int axis_1_size = 12;
    static int axis_1_cursor = 0;

    if (ptr == NULL)
        ptr = malloc(sizeof(char *));

    if (g.move_number % 2 == 0) {
        axis_0_size++;
        void *rtn = realloc(ptr, axis_0_size * sizeof(char *));
        if (rtn == NULL) {
            exit(1);
        }
        ptr = rtn;
        ptr[axis_0_size - 1] = malloc(axis_1_size * sizeof(char));
        char tmp[10];
        notate_move(g, last_x1, last_y1, last_x2, last_y2, last_move_type, tmp);
        sprintf(ptr[axis_0_size - 1], "%d. %s", g.move_number / 2, tmp);
    } else {
        char tmp[10];
        notate_move(g, last_x1, last_y1, last_x2, last_y2, last_move_type, tmp);
        sprintf(ptr[axis_0_size - 1], "%s %s", ptr[axis_0_size - 1], tmp);
    }

    int i = 0;
    int start_ix = 0;
    int max_y = getmaxy(win_info) - 2;
    if (axis_0_size > max_y)
        start_ix = axis_0_size - max_y;

    wclear(win_info);
    box(win_info, 0, 0);
    while (start_ix + i < axis_0_size && i < max_y) {
        mvwprintw(win_info, i + 1, 1, "%s", ptr[start_ix + i]);
        i++;
    }
    wrefresh(win_info);
}

#endif
