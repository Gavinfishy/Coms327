#include <time.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include "map.h"
#include "db.h"

#define COLOR_ENUM(n) (n + 8)

int max_num_trainers = 10;  // Default value
std::list<pokemon_t> pokemons;
std::list<moves_t> moves;
std::list<pokemon_moves_t> pokemon_moves;
std::list<pokemon_species_t> pokemon_species;
std::list<experience_t> experience;
std::list<type_names_t> type_names;
std::list<pokemon_stats_t> pokemon_stats;
std::list<stats_t> stats;
std::list<pokemon_types_t> pokemon_types;

int main(int argc, char *argv[]) {
    int i, j, t;
    world_t wrld;

    std::string filename;
    std::ifstream file;

    std::vector<std::string> files = { "pokemon", "moves", "pokemon_moves",
        "pokemon_species", "experience", "type_names",
        "pokemon_stats", "stats", "pokemon_types" };

    std::vector<std::string> paths = { "/share/cs327/pokedex/",
                                ((std::string) std::getenv("HOME") + "/.poke327/"),
                                ("/mnt/c/Users/BlueD/git_repos/327_poke/pokedex/")};

    // Parse command-line options
    for (i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "--numtrainers=", 14) == 0) {
            max_num_trainers = atoi(argv[i] + 14);
        }

        if (strncmp(argv[i], "--filename=", 11) == 0) {
            filename = argv[i] + 11;
        }
    }

    
    // @todo replace 1 with files.size()
    for (i = 0; (size_t) i < files.size(); i++) {
        for (j = 0; j < 3; j++) {
            file.open(paths[j] + "pokedex/data/csv/" + files[i] + ".csv");
            
            if (file.is_open()) {
                break;
            }
        }

        if (j == 3 && !file.is_open()) {
            std::cout << "Error: database not found" << std::endl;
            break;
        }

        parse_db_file(file, files[i]);
    }

    print_db(filename);



    // return 0; // ending before game starts

    // random seed
    srand(time(NULL));

    // starting ncurses window and color
    initscr();
    raw();
    start_color();
    cbreak();
    noecho();
    curs_set(0);

    enum custom_colors {
        GRAY = 8,
        BRWN,
        DARK_GRN,
        GRN,
        DARK_GRAY,
        DARK_YLLW,
    };

    // intializing custom colors
    init_color(GRAY, 172, 172, 172);
    init_color(BRWN, 435, 212, 16);
    init_color(DARK_GRN, 16, 106, 0);
    init_color(GRN, 34, 255, 0);
    init_color(DARK_GRAY, 101, 101, 101);
    init_color(DARK_YLLW, 179, 188, 44);

    // initializing color pairs
    init_pair(bldr, COLOR_BLACK, GRAY);
    init_pair(tree, DARK_GRN, COLOR_GREEN);
    init_pair(path, COLOR_BLACK, BRWN);
    init_pair(sGrass, DARK_GRN, COLOR_GREEN);
    init_pair(tGrass, DARK_YLLW, COLOR_YELLOW);
    init_pair(mtn, COLOR_BLACK, DARK_GRAY);
    init_pair(water, COLOR_WHITE, COLOR_BLUE);
    init_pair(pCntr, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(pMart, COLOR_BLACK, COLOR_MAGENTA);

    init_pair(player, COLOR_BLACK, COLOR_CYAN);
    init_pair(h, COLOR_BLACK, COLOR_RED);
    init_pair(r, COLOR_BLACK, COLOR_RED);
    init_pair(p, COLOR_BLACK, COLOR_WHITE);
    init_pair(w, COLOR_BLACK, COLOR_WHITE);
    init_pair(s, COLOR_BLACK, COLOR_RED);
    init_pair(e, COLOR_BLACK, COLOR_WHITE);

    init_pair(19, DARK_GRN, COLOR_GREEN); // text area color pair

    // init wrld
    for (i = 0; i < WRLD_MAX; i++) {
        for (j = 0; j < WRLD_MAX; j++) {
            wrld.world[i][j] = NULL;
        }
    }

    wrld.curr_idx[dim_y] = 200;
    wrld.curr_idx[dim_x] = 200;

    for (i = 0; i < Y_MAX; ++i) {
        for (j = 0; j < X_MAX; ++j) {
            wrld.hiker_cost[i][j] = 0;  // or any other suitable initial value
            wrld.rival_cost[i][j] = 0;  // or any other suitable initial value
        }
    }

    wrld.pc_turn.character = &wrld.pc;
    wrld.pc.type = player;
    wrld.pc.y = 0;
    wrld.pc.x = 0;
    // wrld.pc.addPokemon(wrld.curr_idx[0], wrld.curr_idx[1]);
    wrld.pc_turn.next_turn = 0;
    wrld.pc_turn.seq_num = 0;

    // windows
    wrld.top_msg_win = newwin(1, X_MAX, 0, 0);
    print_top_msg(&wrld, false, NULL);

    wrld.bottom_msg_win = newwin(2, X_MAX, 22, 0);
    print_bottom_msg(&wrld, NULL);

    wrld.main_win = newwin(Y_MAX, X_MAX, 1, 0);
    keypad(wrld.main_win, TRUE);

    // generates and prints center map
    generate_map(&wrld);
    pokemon_start_win(&wrld);
    turn_move(&wrld);

    delwin(wrld.top_msg_win);
    delwin(wrld.main_win);
    delwin(wrld.bottom_msg_win);    

    //ending window
    endwin();
    exit_curses(0);
    
    for (i = 0; i < WRLD_MAX; i++) {
        for (j = 0; j < WRLD_MAX; j++) {
            if (wrld.world[i][j] != NULL) {
                for (t = 0; t < (int) (wrld.world[i][j]->num_trainers); t++) {
                    if (wrld.world[i][j]->turn_table[t].character != NULL) 
                        free(wrld.world[i][j]->turn_table[t].character);

                    if (wrld.world[i][j]->turn_table[t].hn != NULL) 
                        free(wrld.world[i][j]->turn_table[t].hn);
                }
                free(wrld.world[i][j]);
            }
        }
    }
    

    return 0;
}