/***************************************
 * Adhere statement in COPYWRITE.txt   *
***************************************/

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

void start_screen (world_t *wrld);

int max_num_trainers = 10;  // Default value
std::vector<pokemon_t> pokemons;
std::vector<moves_t> moves;
std::vector<pokemon_moves_t> pokemon_moves;
std::vector<pokemon_species_t> pokemon_species;
std::vector<experience_t> experience;
std::vector<type_names_t> type_names;
std::vector<pokemon_stats_t> pokemon_stats;
std::vector<stats_t> stats;
std::vector<pokemon_types_t> pokemon_types;

int main(int argc, char *argv[]) {
    int i, j;

    std::string filename;
    std::ifstream file;

    std::vector<std::string> files = { "pokemon", "moves", "pokemon_moves",
        "pokemon_species", "experience", "type_names",
        "pokemon_stats", "stats", "pokemon_types" };

    std::vector<std::string> paths = { "/share/cs327/pokedex/",
                                ((std::string) std::getenv("HOME") + "/.poke327/"),
                                ((std::string) std::getenv("HOME") + "/Documents/")};

    // Parse command-line options
    for (i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "--numtrainers=", 14) == 0) {
            max_num_trainers = atoi(argv[i] + 14);
        }

        if (strncmp(argv[i], "--filename=", 11) == 0) {
            filename = argv[i] + 11;
        }
    }

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
        ORNG,
        G,
        B,
    };

    // intializing custom colors
    init_color(GRAY, 172, 172, 172);
    init_color(BRWN, 435, 212, 16);
    init_color(DARK_GRN, 16, 106, 0);
    init_color(GRN, 34, 255, 0);
    init_color(DARK_GRAY, 101, 101, 101);
    init_color(DARK_YLLW, 179, 188, 44);
    init_color(ORNG, 251, 129, 1);
    init_color(B, 0, 242, 255);

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
    init_pair(20, COLOR_BLUE, COLOR_YELLOW); // start screen pokemon blue
    init_pair(21, COLOR_WHITE, COLOR_GREEN);
    init_pair(22, COLOR_WHITE, COLOR_RED);
    init_pair(23, COLOR_WHITE, COLOR_BLUE);

    world_t *wrld = new World;

    // choose first pokemon
    start_screen(wrld);

    // windows
    print_top_msg(wrld, false, NULL);
    

    print_bottom_msg(wrld, NULL);


    // generates and prints center map
    generate_map(wrld);

    turn_move(wrld);

    delwin(wrld->top_msg_win);
    delwin(wrld->main_win);
    delwin(wrld->bottom_msg_win);    

    //ending window
    endwin();
    exit_curses(0);
    
    wrld->free_World();
    
    return 0;
}