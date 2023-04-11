#ifndef GUI_H
#define GUI_H

#include "board.h"
#include "pieces.h"
#include <math.h>
#include <ncurses.h>
#include <unistd.h>

#define WIN_PADDING 1
#define SQUARE_WIDTH 3
#define WIN_HEIGHT N_RANKS + 2 * WIN_PADDING
#define WIN_WIDTH SQUARE_WIDTH *N_FILES + 2 * WIN_PADDING

#define BLACK_PIECE_COLOUR COLOR_RED
#define WHITE_PIECE_COLOUR COLOR_BLUE
#define BLACK_SQUARE_COLOUR COLOR_BLACK
#define WHITE_SQUARE_COLOUR COLOR_WHITE
#define WHITE_HIGHLIGHTED_SQUARE_COLOUR COLOR_CYAN
#define BLACK_HIGHLIGHTED_SQUARE_COLOUR COLOR_MAGENTA
typedef enum {
    WHITE_SQUARE = 0x4,
    BLACK_SQUARE = 0x8,
    WHITE_HIGHLIGHTED_SQUARE = 0x10,
    BLACK_HIGHLIGHTED_SQUARE = 0x20
} SquareBackgroundType;

void print_game_meta_debug(WINDOW *win, int flags) {
    const char *flag_names[] = {
        "WHITES_TURN",         "BLACKS_TURN",         "WHITE_IN_CHECK",
        "BLACK_IN_CHECK",      "WHITE_CAN_CASTLE",    "BLACK_CAN_CASTLE",
        "WHITE_CAN_ENPASSANT", "BLACK_CAN_ENPASSANT", "CAN_ENPASSANT_APAWN",
        "CAN_ENPASSANT_BPAWN", "CAN_ENPASSANT_CPAWN", "CAN_ENPASSANT_DPAWN",
        "CAN_ENPASSANT_EPAWN", "CAN_ENPASSANT_FPAWN", "CAN_ENPASSANT_GPAWN",
        "CAN_ENPASSANT_HPAWN",
    };

    const int flag_values[] = {
        WHITES_TURN,         BLACKS_TURN,         WHITE_IN_CHECK,
        BLACK_IN_CHECK,      WHITE_CAN_CASTLE,    BLACK_CAN_CASTLE,
        WHITE_CAN_ENPASSANT, BLACK_CAN_ENPASSANT, CAN_ENPASSANT_APAWN,
        CAN_ENPASSANT_BPAWN, CAN_ENPASSANT_CPAWN, CAN_ENPASSANT_DPAWN,
        CAN_ENPASSANT_EPAWN, CAN_ENPASSANT_FPAWN, CAN_ENPASSANT_GPAWN,
        CAN_ENPASSANT_HPAWN,
    };
    for (int i = 0; i < (sizeof(flag_values) / sizeof(int)); i++) {
        mvprintw(i, 0, "                    ");
        mvprintw(i, 0, "%s %d", flag_names[i],
                 (flags & flag_values[i]) == flag_values[i]);
    }
}

bool is_white_square(int i, int j) { return i % 2 ^ j % 2; }

void transform_to_win(int *x, int *y) {
    *x = (SQUARE_WIDTH * (*x)) + WIN_PADDING;
    *y = (*y + WIN_PADDING);
    return;
}

void transform_to_board(int *x, int *y) {
    *x = (*x - WIN_PADDING) / SQUARE_WIDTH;
    *y = (*y - WIN_PADDING);
    return;
}

WINDOW *new_win() {
    WINDOW *win;
    int starty = (LINES - WIN_HEIGHT) / 2;
    int startx = (COLS - WIN_WIDTH) / 2;
    win = newwin(WIN_HEIGHT, WIN_WIDTH, starty, startx);
    keypad(win, TRUE);
    box(win, 0, 0);
    refresh();
    int x, y;
    getbegyx(win, y, x);
    refresh();
    return win;
}

bool init_colours(void) {
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color!\n");
        return false;
    }
    start_color();

    const int square_background_types[] = {WHITE_SQUARE, BLACK_SQUARE,
                                           WHITE_HIGHLIGHTED_SQUARE,
                                           BLACK_HIGHLIGHTED_SQUARE};
    const int square_background_colours[] = {
        WHITE_SQUARE_COLOUR, BLACK_SQUARE_COLOUR,
        WHITE_HIGHLIGHTED_SQUARE_COLOUR, BLACK_HIGHLIGHTED_SQUARE_COLOUR};

    for (int i = 0; i < sizeof(square_background_types) / sizeof(int); i++) {
        init_pair(WHITE | square_background_types[i], WHITE_PIECE_COLOUR,
                  square_background_colours[i]);
        init_pair(BLACK | square_background_types[i], BLACK_PIECE_COLOUR,
                  square_background_colours[i]);
    }
    return true;
}

int get_colour_pair(Game g, int x1, int y1, int x2, int y2) {
    int background_type;
    bool is_highlighted =
        x1 != -1 && y1 != -1 && is_valid_move(g.b, g.meta, x1, y1, x2, y2);

    if (is_highlighted) {
        if (get_piece_owner(g.b[y1][x1]) == WHITE) {
            background_type = WHITE_HIGHLIGHTED_SQUARE;
        } else {
            background_type = BLACK_HIGHLIGHTED_SQUARE;
        }
    } else {
        if (is_white_square(x2, y2)) {
            background_type = WHITE_SQUARE;
        } else {
            background_type = BLACK_SQUARE;
        }
    }

    int square_owner = get_piece_owner(g.b[y2][x2]);
    if (square_owner == NOOWNER)
        square_owner = WHITE;

    return COLOR_PAIR(square_owner | background_type);
}

int get_square_attr(Game g, int x1, int y1, int x2, int y2, int x_cur,
                    int y_cur) {
    int attr = get_colour_pair(g, x1, y1, x2, y2);

    if (x2 == x_cur && y2 == y_cur) { // Is cursor square
        attr |= A_REVERSE;
    } else if (x1 != -1 && y1 != -1 && (x1 == x2 && y1 == y2)) {
        attr |= A_REVERSE | A_BOLD;
    }
    return attr;
}

void print_square(WINDOW *win, Game g, int x1, int y1, int x2, int y2,
                  int x_cur, int y_cur) {

    int attr = get_square_attr(g, x1, y1, x2, y2, x_cur, y_cur);

    char piece_repr;
    if (g.b[y2][x2] == 0) {
        piece_repr = ' ';
    } else {
        int piece = get_piece_type(g.b[y2][x2]);
        piece_repr = get_piece_repr(piece);
    }

    wattron(win, attr);
    transform_to_win(&x2, &y2);
    mvwprintw(win, y2, x2, " %c ", piece_repr);
    wattroff(win, attr);
    return;
}

void print_board(WINDOW *win, Game g, int x1, int y1, int x_cursor,
                 int y_cursor) {
    if (x1 != -1 && y1 != -1) {
        transform_to_board(&x1, &y1);
    }
    transform_to_board(&x_cursor, &y_cursor);
    for (int y2 = 0; y2 < N_FILES; y2++) {
        for (int x2 = 0; x2 < N_RANKS; x2++) {
            print_square(win, g, x1, y1, x2, y2, x_cursor, y_cursor);
        }
    }
}

void print_board_labels(WINDOW *win) {
    int x, y;
    getmaxyx(win, y, x);
    char labels[] = "abcdefgh";
    for (int i = 0; i < (sizeof(labels) / sizeof(char) - 1); i++) {
        mvwprintw(win, y - 1, 2 + (i * SQUARE_WIDTH), "%c", labels[i]);
        mvwprintw(win, i + 1, x - 1, "%d", (8 - i));
    }
}

void update_gui(WINDOW *win, Game g, int x1, int y1, int x2, int y2,
                bool debug) {
    if (debug) {
        print_game_meta_debug(win, g.meta);
    }
    print_board(win, g, x1, y1, x2, y2);
    // print_board_labels(win);
    wmove(win, y2, x2);
    wrefresh(win);
    refresh();
}

bool init_gui() {
    initscr();
    if (!init_colours())
        return false;
    cbreak(); /* Start curses mode              */
    noecho();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);
    curs_set(0);
    return true;
}

void attempt_move(WINDOW *win, Game *g, int *x1, int *y1, int x2, int y2) {
    int x1_t = *x1;
    int y1_t = *y1;
    transform_to_board(&x1_t, &y1_t);
    transform_to_board(&x2, &y2);
    if (!make_move(g, x1_t, y1_t, x2, y2)) {
        wmove(win, *x1, *y1);
    }
    *x1 = -1;
    *y1 = -1;
}

int run_gui(Game *game) {
    bool debug = true;
    if (!init_gui())
        return EXIT_FAILURE;

    WINDOW *win = new_win();
    MEVENT event;
    MEVENT event_tmp;

    int x1 = -1;
    int y1 = -1;
    int x2 = WIN_PADDING;
    int y2 = WIN_HEIGHT - 1 - WIN_PADDING;
    int x_click, y_click;
    update_gui(win, *game, x1, y1, x2, y2, debug);
    while (1) {
        switch (wgetch(win)) {
        case KEY_UP:

            if (y2 == WIN_PADDING)
                y2 = WIN_HEIGHT - WIN_PADDING - 1;
            else
                y2--;
            break;
        case KEY_DOWN:
            if (y2 == WIN_HEIGHT - WIN_PADDING - 1)
                y2 = WIN_PADDING;
            else
                y2++;
            break;
        case KEY_LEFT:
            if (x2 == 1)
                x2 = WIN_WIDTH - WIN_PADDING - SQUARE_WIDTH;
            else
                x2 -= SQUARE_WIDTH;
            break;
        case KEY_RIGHT:
            if (x2 >= WIN_WIDTH - WIN_PADDING - SQUARE_WIDTH)
                x2 = WIN_PADDING;
            else
                x2 += SQUARE_WIDTH;
            break;
        case KEY_MOUSE:
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_PRESSED) {
                    if (!wmouse_trafo(win, &event.y, &event.x, false))
                        break;

                    if (x1 == -1 && y1 == -1) {
                        x1 = event.x;
                        y1 = event.y;
                    } else {
                        x2 = event.x;
                        y2 = event.y;
                        attempt_move(win, game, &x1, &y1, x2, y2);
                    }

                } else if (event.bstate & BUTTON3_PRESSED) {
                    x1 = -1;
                    y1 = -1;
                }
            }
            break;
        case ' ':
            if (x1 == -1 && y1 == -1) {
                x1 = x2;
                y1 = y2;
            } else {
                attempt_move(win, game, &x1, &y1, x2, y2);
            }
            break;
        default:
            break;
        }
        update_gui(win, *game, x1, y1, x2, y2, debug);
    }
}
#endif
