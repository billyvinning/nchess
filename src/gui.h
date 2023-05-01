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
}

void del_game_window(void) {
    delwin(win_body);
    delwin(win_board);
    delwin(win_info);
    delwin(win_dialog);
}

void run_game_gui(Game *g) {
    is_board_focused = false;
    new_game_window(*g);
    switch_dialog_mode(DIALOG_START_MODE);
    new_win_dialog();
    while (1) {
        if (is_board_focused) {
            int ch = wgetch(win_board);
            if (ch == 27) { // Esc or Alt pressed.
                is_board_focused = false;
                switch_dialog_mode(DIALOG_MID_MODE);
                new_win_dialog();
            } else {
                bool board_state_is_updated = board_driver(ch, g);
                if (board_state_is_updated)
                    update_info_event(*g);
            }
        } else {
            int dialog_event_type = dialog_driver(wgetch(win_dialog), g);
            switch (dialog_event_type) {
            case DIALOG_START_EVENT:
                is_board_focused = true;
                switch_dialog_mode(DIALOG_MID_MODE);
                refresh();
                new_game_window(*g);
                break;
            case DIALOG_RESTART_EVENT:
                init_game(g);
            case DIALOG_RESUME_EVENT:
                is_board_focused = true;
                refresh();
                new_game_window(*g);
                break;
            case DIALOG_EXIT_EVENT:
                goto teardown;
            }
        }
    }

teardown:
    del_game_window();
    endwin();
}

#endif
