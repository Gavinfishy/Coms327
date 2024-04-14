/****************************************
 * Adhere to statement in COPYWRITE.txt *
****************************************/

#include "map.h"

#include "db.h"

/*
    returns whether bool of is trainer defeated
*/
bool trainer_battle(world_t *wrld, NPC* trainer) {
    // uint8_t start_txt_y = 5;
    uint8_t start_txt_y_pc = 3;
    uint8_t start_txt_x_pc = 5;
    uint8_t start_txt_y_t = 10;
    uint8_t start_txt_x_t = 55;
    // uint8_t i;
    bool knocked = false;

    // only showing the first pokemon of each for now
    Pokemon &pc_pokemon = wrld->pc.pokemon_collection[0];
    Pokemon &t_pokemon = trainer->pokemon_collection[0];

    wclear(main_w);
    // print pc stuff
    mvwprintw(main_w, start_txt_y_pc, start_txt_x_pc, pc_pokemon.name.c_str());
    mvwprintw(main_w, start_txt_y_pc + 1, start_txt_x_pc, "L:%d %c", pc_pokemon.level, pc_pokemon.gender == 1 ? 'M' : 'F');
    mvwprintw(main_w, start_txt_y_pc + 2, start_txt_x_pc, "hp: %d", pc_pokemon.hp);
    // mvwprintw(main_w, start_txt_y_pc, start_txt_x_pc, "PC");
    // mvwprintw(main_w, start_txt_y_pc + 2, start_txt_x_pc, "xp: %d", pc_pokemon.xp);
    // mvwprintw(main_w, start_txt_y_pc + 4, start_txt_x_pc, "atk: %d", pc_pokemon.atk);
    // mvwprintw(main_w, start_txt_y_pc + 5, start_txt_x_pc, "def: %d", pc_pokemon.def);
    // mvwprintw(main_w, start_txt_y_pc + 6, start_txt_x_pc, "speed: %d", pc_pokemon.speed);
    // mvwprintw(main_w, start_txt_y_pc + 7, start_txt_x_pc, "special atk: %d", pc_pokemon.special_atk);
    // mvwprintw(main_w, start_txt_y_pc + 8, start_txt_x_pc, "special def: %d", pc_pokemon.special_def);

    // i = 0;
    // for (const moves_t* mv : pc_pokemon.myMoves) {
    //     mvwprintw(main_w, start_txt_y_pc + 10 + i, start_txt_x_pc,
    //                 "move %d: %s", (i + 1), mv->identifier.c_str());
    //     i++;
    // }

    mvwprintw(main_w, start_txt_y_t, start_txt_x_t, t_pokemon.name.c_str());
    mvwprintw(main_w, start_txt_y_t + 1, start_txt_x_t, "L:%d %c", t_pokemon.level, t_pokemon.gender == 1 ? 'M' : 'F');
    mvwprintw(main_w, start_txt_y_t + 2, start_txt_x_t, "hp: %d", t_pokemon .hp);
    // mvwprintw(main_w, start_txt_y_t, start_txt_x_t, "Trainer");
    // mvwprintw(main_w, start_txt_y_t + 2, start_txt_x_t, "xp: %d", t_pokemon .xp);
    // mvwprintw(main_w, start_txt_y_t + 4, start_txt_x_t, "atk: %d", t_pokemon .atk);
    // mvwprintw(main_w, start_txt_y_t + 5, start_txt_x_t, "def: %d", t_pokemon .def);
    // mvwprintw(main_w, start_txt_y_t + 6, start_txt_x_t, "speed: %d", t_pokemon .speed);
    // mvwprintw(main_w, start_txt_y_t + 7, start_txt_x_t, "special atk: %d", t_pokemon .special_atk);
    // mvwprintw(main_w, start_txt_y_t + 8, start_txt_x_t, "special def: %d", t_pokemon .special_def);
    // i = 0;
    // for (const moves_t* mv : t_pokemon.myMoves) {
    //     mvwprintw(main_w, start_txt_y_t + 10 + i, start_txt_x_t,
    //                 "move %d: %s", (i + 1), mv->identifier.c_str());
    //     i++;
    // }

    wrefresh(main_w);

    while (wgetch(main_w) != 27){
        if (knocked) {
            break;
        }

    }

    print_map(wrld);
    
    return true;
}


bool pokemon_encounter (world_t *wrld, Pokemon &pokemon) {
    uint8_t start_txt_y = 5;
    uint8_t start_txt_x = 32;
    bool captured = false;
    bool fled = false;
    bool knocked = false;
    int input = 0;
    // int i;

    wclear(main_w);

    while(1) {
        if (input == KEY_RIGHT) {
            captured = false;
        }
        else if (input == KEY_LEFT) {
            captured = true;
        }
        else if (input == '\n') {
            break;
        }
        if (captured || fled || knocked) {
            break;
        }

        // mvwprintw(main_w, start_txt_y, start_txt_x, pokemon.name.c_str());
        // mvwprintw(main_w, start_txt_y + 1, start_txt_x, "xp: %d", pokemon.xp);
        // mvwprintw(main_w, start_txt_y + 2, start_txt_x, "hp: %d", pokemon.hp);
        // mvwprintw(main_w, start_txt_y + 3, start_txt_x, "atk: %d", pokemon.atk);
        // mvwprintw(main_w, start_txt_y + 4, start_txt_x, "def: %d", pokemon.def);
        // mvwprintw(main_w, start_txt_y + 5, start_txt_x, "speed: %d", pokemon.speed);
        // mvwprintw(main_w, start_txt_y + 6, start_txt_x, "special atk: %d", pokemon.special_atk);
        // mvwprintw(main_w, start_txt_y + 7, start_txt_x, "special def: %d", pokemon.special_def);
        // mvwprintw(main_w, start_txt_y + 8, start_txt_x, "level: %d", pokemon.level);
        // pokemon.hp += 1;
        // display moves
        // i = 0;
        // for (const moves_t* mv : pokemon.myMoves) {
        //     mvwprintw(main_w, start_txt_y + 9 + i, start_txt_x,
        //                 "move %d: %s", (i + 1), mv->identifier.c_str());
        //     i++;
        // }
        mvwprintw(main_w, start_txt_y, start_txt_x, "Pokemon: %s", pokemon.name.c_str());
        mvwprintw(main_w, start_txt_y + 1, start_txt_x, "Level: %d", pokemon.level);

        // Calculate health bar length (e.g., 1 HP = 1 character)
        int health_bar_length = pokemon.hp;

        // Print health bar
        mvwprintw(main_w, start_txt_y + 2, start_txt_x, "HP: ");
        for (int i = 0; i < health_bar_length; i++) {
            waddch(main_w, '|');  // Use '|' character for health bar
        }


        mvwprintw(main_w, start_txt_y + 12, start_txt_x - 6, "(Press enter to select)");
        mvwprintw(main_w, start_txt_y + 13, start_txt_x + 3, "y");
        mvwprintw(main_w, start_txt_y + 13, start_txt_x + 6, "n");

        if (captured) {
            wattron(main_w, COLOR_PAIR(e)); 
            mvwprintw(main_w, start_txt_y + 13, start_txt_x + 3, "y");
            wattroff(main_w, COLOR_PAIR(e));
        }
        else {
            wattron(main_w, COLOR_PAIR(e)); 
            mvwprintw(main_w, start_txt_y + 13, start_txt_x + 6, "n");
            wattroff(main_w, COLOR_PAIR(e));
        }

        wrefresh(main_w);

        input = wgetch(main_w);
    }

    print_map(wrld);

    return captured;
}
