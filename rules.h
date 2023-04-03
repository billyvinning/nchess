#ifndef RULES_H
#define RULES_H

#include <math.h>
#include "pieces.h"
#include "geom.h"


#define ADJ_M_WIDTH N_RANKS * N_FILES


int transform_to_adj(int i, int j) {
    return i + (N_RANKS * j);
}


void transform_from_adj(int v, int * x, int * y) {
    int i = v % N_RANKS;
    int j = (v - i) / N_RANKS;
    *x = i;
    *y = j;
    return;
}


void add_directional_edges(bool m[][ADJ_M_WIDTH], board b, int x0, int y0, int dx, int dy, int max_steps) {
    int x, y, curr_node, prev_node, curr_node_owner, n_steps;
    int src_node_owner = get_piece_owner(b[y0][x0]);
    int src_node = transform_to_adj(x0, y0);

    n_steps = 1;
    prev_node = src_node;
    while (1) {
        x = x0 + (dx * n_steps);
        y = y0 + (dy * n_steps);
        if (x < 0 || x >= N_FILES || y < 0 || y >= N_RANKS)
            break;
        curr_node = transform_to_adj(x, y);
        curr_node_owner = get_piece_owner(b[y][x]);
            
        if (curr_node_owner == NOOWNER || curr_node_owner != src_node_owner)
            m[prev_node][curr_node] = true;       
        if (curr_node_owner != NOOWNER)
            break;
    
        prev_node = curr_node;
        n_steps += 1;
        if (max_steps != -1 && n_steps > max_steps)
            break;
    }
}

void add_diag_edges(bool m[][ADJ_M_WIDTH], board b, int x0, int y0, int max_steps) {
    const int dx[] = {1, -1, -1, 1};
    const int dy[] = {1, 1, -1, -1};
    for (int i = 0; i < 4; i++) {
        add_directional_edges(m, b, x0, y0, dx[i], dy[i], max_steps);
    }
}

void add_straight_edges(bool m[][ADJ_M_WIDTH], board b, int x0, int y0, int max_steps) {
    const int dx[] = {1, 0, -1, 0};
    const int dy[] = {0, 1, 0, -1};
    for (int i = 0; i < 4; i++) {
        add_directional_edges(m, b, x0, y0, dx[i], dy[i], max_steps);
    }
}


void add_pawn_like_edges(bool m[][ADJ_M_WIDTH], board b, int x0, int y0) {
    int curr_node;
    int src_owner = get_piece_owner(b[y0][x0]);
    int src_node = transform_to_adj(x0, y0);
    int direction;
    bool is_on_back_rank = false;
    
    if (src_owner == WHITE) {
        if (y0 == WHITE_PAWN_ORIGIN)
            is_on_back_rank = true;
        direction = -1;
    }
    else if (src_owner == BLACK) {
        if (y0 == BLACK_PAWN_ORIGIN)
            is_on_back_rank = true;
        direction = 1;
    }
    else {
        return;
    }
    int y = y0 + 2 * direction;
    if (is_on_back_rank && b[y][x0] == EMPTY_SQUARE) {
         curr_node = transform_to_adj(x0, y);
            m[src_node][curr_node] = true;
    }
    y = y0 + direction;
    if (b[y][x0] == EMPTY_SQUARE) {
        curr_node = transform_to_adj(x0, y);
        m[src_node][curr_node] = true;
    }

    int x;
    const int dx[] = {1, -1};
    for (int i = 0; i < 2; i++) {
        x = x0 + dx[i];
        if (x < 0 || x >= N_FILES)
            continue;
        if (b[y][x] != EMPTY_SQUARE && get_piece_owner(b[y][x]) != src_owner) {
            curr_node = transform_to_adj(x, y);
            m[src_node][curr_node] = true;
        }
    }
}


void add_knight_like_edges(bool m[][ADJ_M_WIDTH], board b, int x0, int y0) {
    int x, y, curr_node;
    const int dx[] = {1, -1, 2, -2, 2, -2, 1, -1};
    const int dy[] = {2, 2, 1, 1, -1, -1, -2, -2};
    int src_node_owner = get_piece_owner(b[y0][x0]);
    int src_node = transform_to_adj(x0, y0);
    for (int i = 0; i < 8; i++) {
        x = x0 + dx[i];
        y = y0 + dy[i];
        if (x < 0 || x >= N_FILES || y < 0 || y >= N_RANKS)
            continue;
        if (get_piece_owner(b[y][x]) != src_node_owner) {
            curr_node = transform_to_adj(x, y);
            m[src_node][curr_node] = true;
        }
    }
}


bool has_path(bool m[][ADJ_M_WIDTH], int source, int dest) {
    if (m[source][dest])
        return true;
    for (int i = 0; i < ADJ_M_WIDTH; i++) {
        if (m[source][i] && has_path(m, i, dest))
            return true;    
    }
    return false;
}


bool is_valid_move(board b, int x1, int y1, int x2, int y2) {
    if (b[y1][x1] == EMPTY_SQUARE)
        return false;
    else if (x1 == x2 && y1 == y2)
        return false;
    else if (get_piece_type(b[y2][x2]) == KING)
        return false;
    
    bool m[ADJ_M_WIDTH][ADJ_M_WIDTH] = {{false}};
    switch (get_piece_type(b[y1][x1])) {
        case PAWN:
            add_pawn_like_edges(m, b, x1, y1);
            break;
        case KNIGHT:
            add_knight_like_edges(m, b, x1, y1);
            break;
        case BISHOP:
            add_diag_edges(m, b, x1, y1, -1);
            break;
        case ROOK:
            add_straight_edges(m, b, x1, y1, -1);
            break;
        case QUEEN:
            add_straight_edges(m, b, x1, y1, -1);
            add_diag_edges(m, b, x1, y1, -1);
            break;
        case KING:
            add_straight_edges(m, b, x1, y1, 1);
            add_diag_edges(m, b, x1, y1, 1);
    }
    int i = transform_to_adj(x1, y1);
    int j = transform_to_adj(x2, y2);
    return has_path(m, i, j);
}
#endif


