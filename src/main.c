#include <argp.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "game_gui.h"
#include "gui_utils.h"
#include "main_gui.h"

const char *argp_program_version = "nchess 0.1";
const char *argp_program_bug_address = "github.com/billyvinning/nchess";
static char doc[] = "Chess in the terminal; written in C99.";
static char args_doc[] = "";
static struct argp_option options[] = {
    {"debug", 'd', 0, 0, "Show debugging information during play."},
};

struct arguments {
    bool isDebugMode;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
    case 'd':
        arguments->isDebugMode = true;
        break;
    case ARGP_KEY_ARG:
        return 0;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[]) {
    struct arguments arguments;
    arguments.isDebugMode = false;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    init_gui();
    switch (run_main_gui()) {
    case ONE_PLAYER:
        break;
    case TWO_PLAYER:
        clear();
        refresh();
        Game g = make_game();
        run_game_gui(&g, arguments.isDebugMode);
        break;
    case QUIT:
        break;
    }

    endwin();
    return EXIT_SUCCESS;
}
