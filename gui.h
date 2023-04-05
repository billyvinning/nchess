#ifndef GUI_H
#define GUI_H

#include "board.h"
#include "pieces.h"
#include <math.h>
#include <ncurses.h>

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

void update_cursor(WINDOW *win, int x, int y) {
    wmove(win, y, x);
    wattron(win, A_UNDERLINE);
    wrefresh(win);
    wattroff(win, A_UNDERLINE);
}

WINDOW *new_win() {
    WINDOW *win;
    int starty = (LINES - WIN_HEIGHT) / 2;
    int startx = (COLS - WIN_WIDTH) / 2;
    win = newwin(WIN_HEIGHT, WIN_WIDTH, starty, startx);
    keypad(win, TRUE);
    box(win, 0, 0);
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

int get_colour_pair(board b, int x1, int y1, int x2, int y2, int game_meta) {
    int background_type;
    bool is_highlighted =
        x1 != -1 && y1 != -1 && is_valid_move(b, x1, y1, x2, y2, game_meta);

    if (is_highlighted) {
        if (get_piece_owner(b[y1][x1]) == WHITE) {
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

    int square_owner = get_piece_owner(b[y2][x2]);
    if (square_owner == NOOWNER)
        square_owner = WHITE;

    return COLOR_PAIR(square_owner | background_type);
}

void print_square(WINDOW *win, board b, int x1, int y1, int x2, int y2,
                  int game_meta) {
    int colour_pair = get_colour_pair(b, x1, y1, x2, y2, game_meta);
    int x_t = x2;
    int y_t = y2;
    transform_to_win(&x_t, &y_t);
    char piece_repr;
    if (b[y2][x2] == 0) {
        piece_repr = ' ';
    } else {
        int piece = get_piece_type(b[y2][x2]);
        piece_repr = get_piece_repr(piece);
    }

    wattron(win, colour_pair);
    mvwprintw(win, y_t, x_t, " %c ", piece_repr);
    wattroff(win, colour_pair);
    refresh();
    return;
}

void print_board(WINDOW *win, board b, int x1, int y1, int game_meta) {

    if (x1 != -1 && y1 != -1) {
        transform_to_board(&x1, &y1);
    }
    for (int y2 = 0; y2 < N_FILES; y2++) {
        for (int x2 = 0; x2 < N_RANKS; x2++) {
            print_square(win, b, x1, y1, x2, y2, game_meta);
        }
    }
    wrefresh(win);
}

int init_gui(board b, int *game_meta) {
    initscr();
    if (!init_colours())
        return EXIT_FAILURE;
    cbreak(); /* Start curses mode              */
    noecho();
    int x = WIN_PADDING;
    int y = WIN_HEIGHT - 1 - WIN_PADDING;
    int selected_x = -1;
    int selected_y = -1;
    WINDOW *win = new_win();
    print_board(win, b, selected_x, selected_y, *game_meta);
    print_game_meta_debug(win, *game_meta);
    update_cursor(win, x, y);
    while (1) {
        int ch = wgetch(win);
        switch (ch) {
        case KEY_UP:
            if (y == WIN_PADDING)
                y = WIN_HEIGHT - WIN_PADDING - 1;
            else
                y--;
            break;
        case KEY_DOWN:
            if (y == WIN_HEIGHT - WIN_PADDING - 1)
                y = WIN_PADDING;
            else
                y++;
            break;
        case KEY_LEFT:
            if (x == 1)
                x = WIN_WIDTH - WIN_PADDING - SQUARE_WIDTH;
            else
                x -= SQUARE_WIDTH;
            break;
        case KEY_RIGHT:
            if (x >= WIN_WIDTH - WIN_PADDING - SQUARE_WIDTH)
                x = WIN_PADDING;
            else
                x += SQUARE_WIDTH;
            break;
        case ' ':
            if (selected_x == -1) {
                selected_x = x;
                selected_y = y;
            } else {
                int board_x1 = selected_x;
                int board_y1 = selected_y;
                int board_x2 = x;
                int board_y2 = y;
                transform_to_board(&board_x1, &board_y1);
                transform_to_board(&board_x2, &board_y2);
                if (!make_move(b, board_x1, board_y1, board_x2, board_y2,
                               game_meta)) {
                    wmove(win, selected_y, selected_x);
                } else {
                }
                selected_x = -1;
                selected_y = -1;
            }
            break;
        default:
            // refresh();
            break;
        }
        win = new_win();
        print_board(win, b, selected_x, selected_y, *game_meta);
        print_game_meta_debug(win, *game_meta);
        update_cursor(win, x, y);
    }
}
#endif
