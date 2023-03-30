#ifndef GUI_H
#define GUI_H

#include <ncurses.h>
#include "board.h"

#define WIN_PADDING 1
#define SQUARE_WIDTH 3
#define WIN_HEIGHT N_RANKS + 2 * WIN_PADDING
#define WIN_WIDTH SQUARE_WIDTH * N_FILES + 2 * WIN_PADDING



void transform_to_win(int * x, int * y) {
    *x = (SQUARE_WIDTH * (*x)) + WIN_PADDING;
    *y = (*y + WIN_PADDING);
    return;
}


void transform_to_board(int * x, int * y) {
    *x = (*x - 2) / SQUARE_WIDTH;
    *y = (*y - 2) / SQUARE_WIDTH;
    return;
}

void update_cursor(WINDOW * win, int x, int y) {

    char v[SQUARE_WIDTH];
    
    for (int i = 0; i < SQUARE_WIDTH; i++) {
        v[i] = mvwinch(win, y, x + i);
    }

    wattron(win, A_UNDERLINE);
    mvwprintw(win, y, x, "%s", v);
    wattroff(win, A_UNDERLINE);
    wrefresh(win);
}


WINDOW * new_win() {
    WINDOW * win;
    int starty = (LINES - WIN_HEIGHT) / 2;
    int startx = (COLS - WIN_WIDTH) / 2;
    win = newwin(WIN_HEIGHT, WIN_WIDTH, starty, startx);
    keypad(win, TRUE);
    box(win, 0, 0);
    return win;
}

bool init_colours(void) {
    if(has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color!\n");
        return false;
    }
    start_color();
    static const int BLACK_PIECE_COLOUR = COLOR_RED;
    static const int WHITE_PIECE_COLOUR = COLOR_BLUE;
    static const int BLACK_SQUARE_COLOUR = COLOR_BLACK;
    static const int WHITE_SQUARE_COLOUR = COLOR_WHITE;
    init_pair(1, WHITE_PIECE_COLOUR, WHITE_SQUARE_COLOUR);
    init_pair(2, BLACK_PIECE_COLOUR, WHITE_SQUARE_COLOUR);
    init_pair(3, WHITE_PIECE_COLOUR, BLACK_SQUARE_COLOUR);
    init_pair(4, BLACK_PIECE_COLOUR, BLACK_SQUARE_COLOUR);
    return true;
}


int get_colour_pair(bool is_white_square, bool is_white_piece) {
    if (is_white_square && is_white_piece) {
        return 1;
    } else if (is_white_square && !is_white_piece) {
        return 2;
    } else if (!is_white_square && is_white_piece) {
        return 3;
    }
    else {
        return 4;
    }
}

void print_square(WINDOW * win, board b, int i, int j) {
    int colour_pair = get_colour_pair(
        is_white_square(i, j), is_white_piece(b[i][j])
    );
    wattron(win, COLOR_PAIR(colour_pair));

    int x = j;
    int y = i;
    transform_to_win(&x, &y);
    char piece_repr;
    if (b[i][j] == 0) {
        piece_repr = ' ';
    } else {
        int piece = board_to_piece(b[i][j]);
        piece_repr = get_piece_repr(piece);
    }
    

    mvwprintw(win, y, x, " %c ", piece_repr);
    wattroff(win, COLOR_PAIR(colour_pair));    
    refresh();
    return;
}


void print_board(WINDOW * win, board b) {
    for (int i = 0; i < N_FILES; i++) {
        for (int j = 0; j < N_RANKS; j++) {
            print_square(win, b, i, j);
        }
    }
    wrefresh(win);
}


int init_gui(board b) {
    initscr();
    if (!init_colours())
        return EXIT_FAILURE;
    cbreak();/* Start curses mode              */
    noecho();
    int x = WIN_PADDING;
    int y = WIN_HEIGHT - 1 - WIN_PADDING;
    WINDOW * win = new_win();
    print_board(win, b);
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
            default:
                //refresh();
                break;
        }
        win = new_win();
        print_board(win, b);
        update_cursor(win, x, y);
    }
}
#endif

