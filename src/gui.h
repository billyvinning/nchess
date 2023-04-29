#ifndef GUI_H
#define GUI_H

#include "board_gui.h"
#include "dialog_gui.h"
#include "info_gui.h"
#include <ncurses.h>
#include <stdbool.h>

WINDOW *win_body, *win_info, *win_dialog;
bool is_board_focused;

void new_game_window(Game g) {
    // win_body = new_win_body();
    new_win_board(g);
    new_win_info();
    is_board_focused = true;
    // win_dialog = new_win_dialog();
}

void del_game_window(void) {
    delwin(win_body);
    delwin(win_board);
    delwin(win_info);
    delwin(win_dialog);
}

void popup_dialog_event(void) {}

void run_game_gui(Game *g) {
    new_game_window(*g);
    while (1) {
        if (is_board_focused) {
            int ch = wgetch(win_board);
            if (ch == 27) { // Esc or Alt pressed.
                is_board_focused = false;
                popup_dialog_event();
            } else {
                bool board_state_is_updated = board_driver(ch, g);
                if (board_state_is_updated)
                    update_info_event(*g);
            }
        } else {
            dialog_driver(wgetch(win_dialog), g);
        }
    }

    del_game_window();
    endwin();
}

#endif
