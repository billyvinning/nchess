#ifndef DIALOG_GUI_H
#define DIALOG_GUI_H

#include "board.h"
#include "gui_geom.h"
#include "ncurses.h"
#include <string.h>

WINDOW *win_dialog;

enum DIALOG_MODES { DIALOG_START_MODE, DIALOG_MID_MODE, DIALOG_END_MODE };

enum START_MODE_CHOICES {
    STARTMODE_START_GAME,
    STARTMODE_VIEW_CREDITS,
    STARTMODE_EXIT
};

int start_mode_choices[] = {STARTMODE_START_GAME, STARTMODE_EXIT};
char *start_mode_labels[] = {"New Game", "Exit"};
int n_start_mode_choices = sizeof(start_mode_choices) / sizeof(int);

enum MID_MODE_CHOICES { MIDMODE_RESUME, MIDMODE_RESTART, MIDMODE_EXIT };

int mid_mode_choices[] = {MIDMODE_RESUME, MIDMODE_RESTART, MIDMODE_EXIT};
char *mid_mode_labels[] = {"Resume", "Restart", "Exit"};
int n_mid_mode_choices = sizeof(mid_mode_choices) / sizeof(int);

enum END_MODE_CHOICES { ENDMODE_RESTART, ENDMODE_EXIT };

int end_mode_choices[] = {ENDMODE_RESTART, ENDMODE_EXIT};
char *end_mode_labels[] = {"Restart", "Exit"};
int n_end_mode_choices = sizeof(end_mode_choices) / sizeof(int);

int dialog_cursor_i = 0;

int n_curr_mode_choices;
int *curr_mode_choices;
char **curr_mode_labels;
int curr_mode;

enum DIALOG_EVENT_TYPES {
    DIALOG_NULL_EVENT,
    DIALOG_START_EVENT,
    DIALOG_RESUME_EVENT,
    DIALOG_RESTART_EVENT,
    DIALOG_EXIT_EVENT,
};

void switch_dialog_mode(int mode) {
    curr_mode = mode;
    switch (mode) {
    case DIALOG_START_MODE:
        n_curr_mode_choices = n_start_mode_choices;
        curr_mode_choices = start_mode_choices;
        curr_mode_labels = start_mode_labels;
        break;
    case DIALOG_MID_MODE:
        n_curr_mode_choices = n_mid_mode_choices;
        curr_mode_choices = mid_mode_choices;
        curr_mode_labels = mid_mode_labels;
        break;
    case DIALOG_END_MODE:
        n_curr_mode_choices = n_end_mode_choices;
        curr_mode_choices = end_mode_choices;
        curr_mode_labels = end_mode_labels;
        break;
    }
}

void update_win_dialog(void) {
    int y_offset = 1;
    if (curr_mode == DIALOG_START_MODE) {
        char welcome_msg[] = "nchess 0.1";
        wattron(win_dialog, A_BOLD);
        mvwprintw(win_dialog, y_offset, (WIN_WIDTH - strlen(welcome_msg)) / 2,
                  "%s", welcome_msg);
        wattroff(win_dialog, A_BOLD);
        y_offset++;
    }
    for (int i = 0; i < n_curr_mode_choices; i++) {
        if (i == dialog_cursor_i)
            wattron(win_dialog, A_REVERSE);
        mvwprintw(win_dialog, i + y_offset,
                  (WIN_WIDTH - strlen(curr_mode_labels[i])) / 2, "%s",
                  curr_mode_labels[i]);
        if (i == dialog_cursor_i)
            wattroff(win_dialog, A_REVERSE);
    }
    wrefresh(win_dialog);
}

void new_win_dialog(void) {
    int starty = (LINES - WIN_HEIGHT) / 2 + WIN_HEIGHT;
    int startx = ((COLS - WIN_WIDTH) / 2);
    int height_offset = curr_mode == DIALOG_START_MODE ? 1 : 0;
    win_dialog = newwin(n_curr_mode_choices + height_offset + 2, WIN_WIDTH,
                        starty, startx);
    keypad(win_dialog, TRUE);
    box(win_dialog, 0, 0);
    update_win_dialog();
}

int dialog_selection_update_event(void) {
    update_win_dialog();
    return DIALOG_NULL_EVENT;
}

int dialog_startmode_selection_event(void) {
    switch (curr_mode_choices[dialog_cursor_i]) {
    case STARTMODE_START_GAME:
        return DIALOG_START_EVENT;
    case STARTMODE_EXIT:
        return DIALOG_EXIT_EVENT;
    default:
        return DIALOG_NULL_EVENT;
    }
}

int dialog_midmode_selection_event(void) {
    switch (curr_mode_choices[dialog_cursor_i]) {
    case MIDMODE_RESUME:
        return DIALOG_RESUME_EVENT;
    case MIDMODE_RESTART:
        return DIALOG_RESTART_EVENT;
    case MIDMODE_EXIT:
        return DIALOG_EXIT_EVENT;
    default:
        return DIALOG_NULL_EVENT;
    }
}
int dialog_endmode_selection_event(void) {
    switch (curr_mode_choices[dialog_cursor_i]) {
    case ENDMODE_RESTART:
        return DIALOG_START_EVENT;
    case ENDMODE_EXIT:
        return DIALOG_EXIT_EVENT;
    default:
        return DIALOG_NULL_EVENT;
    }
}

int dialog_selection_event(void) {
    switch (curr_mode) {
    case DIALOG_START_MODE:
        return dialog_startmode_selection_event();
    case DIALOG_MID_MODE:
        return dialog_midmode_selection_event();
    case DIALOG_END_MODE:
        return dialog_endmode_selection_event();
    }
}

int dialog_driver(int ch, Game *g) {
    switch (ch) {
    case KEY_UP:
        if (dialog_cursor_i-- <= 0)
            dialog_cursor_i = n_curr_mode_choices - 1;
        return dialog_selection_update_event();
    case KEY_DOWN:
        if (dialog_cursor_i++ >= (n_curr_mode_choices - 1))
            dialog_cursor_i = 0;
        return dialog_selection_update_event();
    case '\n':
    case ' ':
        return dialog_selection_event();
    }
}

#endif
