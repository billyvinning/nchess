#ifndef GUI_H
#define GUI_H

#include <ncurses.h>
#include "board.h"
#include "pieces.h"

#define WIN_PADDING 1
#define SQUARE_WIDTH 3
#define WIN_HEIGHT N_RANKS + 2 * WIN_PADDING
#define WIN_WIDTH SQUARE_WIDTH * N_FILES + 2 * WIN_PADDING

#define BLACK_PIECE_COLOUR COLOR_RED
#define WHITE_PIECE_COLOUR COLOR_BLUE
#define BLACK_SQUARE_COLOUR COLOR_BLACK
#define WHITE_SQUARE_COLOUR COLOR_WHITE
#define HIGHLIGHTED_SQUARE_COLOUR COLOR_CYAN

typedef enum {WHITE_SQUARE=0x4, BLACK_SQUARE=0x8, HIGHLIGHTED_SQUARE=0x10} SquareBackgroundType;

void print_game_meta_debug(WINDOW * win, int flags) {
    const char * flag_names[] = {
       "WHITES_TURN",
       "BLACKS_TURN",
       "WHITE_IN_CHECK",
       "BLACK_IN_CHECK",
       "WHITE_CAN_ENPASSANT",
       "BLACK_CAN_ENPASSANT",
       "CAN_ENPASSANT_APAWN",
       "CAN_ENPASSANT_BPAWN",
       "CAN_ENPASSANT_CPAWN",
       "CAN_ENPASSANT_DPAWN",
       "CAN_ENPASSANT_EPAWN",
       "CAN_ENPASSANT_FPAWN",
       "CAN_ENPASSANT_GPAWN",
       "CAN_ENPASSANT_HPAWN",
    };

    const int flag_values[] = {
        WHITES_TURN,
        BLACKS_TURN,
        WHITE_IN_CHECK,
        BLACK_IN_CHECK,
        WHITE_CAN_ENPASSANT,
        BLACK_CAN_ENPASSANT,
        CAN_ENPASSANT_APAWN,
        CAN_ENPASSANT_BPAWN,
        CAN_ENPASSANT_CPAWN,
        CAN_ENPASSANT_DPAWN,
        CAN_ENPASSANT_EPAWN,
        CAN_ENPASSANT_FPAWN,
        CAN_ENPASSANT_GPAWN,
        CAN_ENPASSANT_HPAWN,
    };
    for (int i = 0; i < (sizeof(flag_values) / sizeof(int) ); i++) {
        mvprintw(i, 0, "                    ");
        mvprintw(i, 0, "%s %d", flag_names[i], (flags & flag_values[i]) == flag_values[i]);
    }
}

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
    wrefresh(win);
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
    
    const int square_background_types[] = {WHITE_SQUARE, BLACK_SQUARE, HIGHLIGHTED_SQUARE};
    const int square_background_colours[] = {WHITE_SQUARE_COLOUR, BLACK_SQUARE_COLOUR, HIGHLIGHTED_SQUARE_COLOUR};

    for (int i = 0; i < sizeof(square_background_types) / sizeof(int); i++) {
        init_pair(
            WHITE | square_background_types[i],
            WHITE_PIECE_COLOUR,
            square_background_colours[i]
        );
        init_pair(
            BLACK | square_background_types[i],
            BLACK_PIECE_COLOUR,
            square_background_colours[i]
        );
    }
    return true;
}


void print_square(WINDOW * win, board b, int x, int y, bool is_highlighted) {
    int piece_owner = get_piece_owner(b[y][x]);
    int background_type;
    if (is_highlighted) {
        background_type = HIGHLIGHTED_SQUARE; 
    } else if (is_white_square(x, y)) {
        background_type = WHITE_SQUARE;
    }
    else {
        background_type = BLACK_SQUARE;
    }  
    if (piece_owner == NOOWNER)
        piece_owner = WHITE;

    int colour_pair = COLOR_PAIR(piece_owner | background_type);
    int x_t = x;
    int y_t = y;
    transform_to_win(&x_t, &y_t);
    char piece_repr;
    if (b[y][x] == 0) {
        piece_repr = ' ';
    } else {
        int piece = get_piece_type(b[y][x]);
        piece_repr = get_piece_repr(piece);
    }

    wattron(win, colour_pair);
    mvwprintw(win, y_t, x_t, " %c ", piece_repr);
    wattroff(win, colour_pair);    
    refresh();
    return;
}


void print_board(WINDOW * win, board b, int x_selected, int y_selected, int game_meta) {
    
    bool is_highlighted;
    if (x_selected != -1 && y_selected != -1) {
        transform_to_board(&x_selected, &y_selected); 
    }
    for (int y = 0; y < N_FILES; y++) {
        for (int x = 0; x < N_RANKS; x++) {
            if (x_selected == -1) {
                is_highlighted = false;
            } else {
                is_highlighted = is_valid_move(b, x_selected, y_selected, x, y, game_meta);
            }
            print_square(win, b, x, y, is_highlighted);
        }
    }
    wrefresh(win);
}


int init_gui(board b, int * game_meta) {
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
    print_board(win, b, selected_x, selected_y, *game_meta);
    // print_game_meta_debug(win, *game_meta);
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
                    if (!make_move(b, board_x1, board_y1, board_x2, board_y2, game_meta)) {
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
        print_board(win, b, selected_x, selected_y, *game_meta);
        //print_game_meta_debug(win, *game_meta);
        update_cursor(win, x, y);
    }
}
#endif

