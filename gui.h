#ifndef GUI_H
#define GUI_H

#include <ncurses.h>
#include "board.h"
#include "pieces.h"

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
    *x = (*x - WIN_PADDING) / SQUARE_WIDTH;
    *y = (*y - WIN_PADDING);
    return;
}

void update_cursor(WINDOW * win, int x, int y) {
    wmove(win, y, x);
    wattron(win, A_UNDERLINE);
    //wrefresh(win);
    wattroff(win, A_UNDERLINE);
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
        is_white_square(i, j), get_piece_owner(b[i][j]) == WHITE
    );
    wattron(win, COLOR_PAIR(colour_pair));

    int x = j;
    int y = i;
    transform_to_win(&x, &y);
    char piece_repr;
    if (b[i][j] == 0) {
        piece_repr = ' ';
    } else {
        int piece = get_piece_type(b[i][j]);
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
    int selected_x = -1;
    int selected_y = -1;
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
            case ' ':
                if (selected_x == -1){
                    selected_x = x;
                    selected_y = y;
                } else {
                    int board_x1 = selected_x;
                    int board_y1 = selected_y;
                    int board_x2 = x;
                    int board_y2 = y;
                    transform_to_board(&board_x1, &board_y1);
                    transform_to_board(&board_x2, &board_y2);
                    if (!make_move(b, board_x1, board_y1, board_x2, board_y2)) {
                        wmove(win, selected_y, selected_x);
                    }
                    else {
                    }
                    selected_x = -1;
                    selected_y = -1;
                }
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

