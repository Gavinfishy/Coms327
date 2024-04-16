/****************************************
 * Adhere to statement in COPYWRITE.txt *
****************************************/

#include "map.h"
#include <unistd.h>
#include "db.h"

/*
    returns whether bool of is trainer defeated
*/
bool trainer_battle(world_t *wrld, NPC* trainer) {
    uint8_t start_txt_y_t = 3;
    uint8_t start_txt_x_t = 5;
    uint8_t start_txt_y_pc = 10;
    uint8_t start_txt_x_pc = 55;

    // uint8_t i;
    bool knocked = false;

    // only showing the first pokemon of each for now
    Pokemon &pc_pokemon = wrld->pc.pokemon_collection[0];
    Pokemon &t_pokemon = trainer->pokemon_collection[0];

    wclear(main_w);
    // print pc stuff
    mvwprintw(main_w, start_txt_y_pc, start_txt_x_pc, pc_pokemon.name.c_str());
    mvwprintw(main_w, start_txt_y_pc + 1, start_txt_x_pc, "L:%d %c", pc_pokemon.level, pc_pokemon.gender == 1 ? 'M' : 'F');
    mvwprintw(main_w, start_txt_y_pc + 2, start_txt_x_pc, "hp: %d", pc_pokemon.total_hp);
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
    mvwprintw(main_w, start_txt_y_t + 2, start_txt_x_t, "hp: %d", t_pokemon .total_hp);
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
    uint8_t start_txt_y_w = 3;
    uint8_t start_txt_x_w = 5;
    uint8_t start_txt_y_pc = 10;
    uint8_t start_txt_x_pc = 55;
    uint8_t start_txt_y = 5;
    uint8_t start_txt_x = 32;
    bool captured = false;
    bool fled = false;
    bool knocked = false;
    bool mainScreen = true;
    bool fightScreen = false;
    bool packScreen = false;
    bool pkmnScreen = false;
    bool runScreen = false;
    int input = 0;
    // 1 fight, 2 pack, 3 PKMN, 4 Run
    int direction = 1;
    int i;
    int escape_chance = 30;


    wclear(main_w);
    Pokemon &pc_pokemon = wrld->pc.pokemon_collection[0];

    mvwprintw(main_w, start_txt_y + 12, start_txt_x - 8, "A wild %s appeared!", pokemon.name.c_str());
    wrefresh(main_w);
    sleep(1);
    wclear(main_w);
    mvwprintw(main_w, start_txt_y + 12, start_txt_x - 8, "Go! %s!", pc_pokemon.name.c_str());
    wrefresh(main_w);
    sleep(1);
    wclear(main_w);


    while(1) {
        if (input == '\n') {
            if (mainScreen) {
                mainScreen = false;
                if (direction == 1) {
                    fightScreen = true;
                }
                else if (direction == 2) {
                    packScreen = true;
                }
                else if (direction == 3) {
                    pkmnScreen = true;
                }
                else if (direction == 4) {
                    runScreen = true;
                }
            }
            else if (fightScreen) {
                fightScreen = false;
                mainScreen = true;
            }
            else if (packScreen) {
                packScreen = false;
                mainScreen = true;
            }
            else if (pkmnScreen) {
                pkmnScreen = false;
                mainScreen = true;
            }
            else if (runScreen) {
                runScreen = false;
                mainScreen = true;
            }
            input = 0;
            direction = 1;
            continue;
        }
        if (input == 'q') {
            break;
        }
        if (captured || fled || knocked) {
            break;
        }
        
        mvwprintw(main_w, start_txt_y + 12, start_txt_x - 6, "(Press enter to select)");
        wmove(main_w, start_txt_y + 13, 0);
        wclrtoeol(main_w);
        if (mainScreen) {
            if (input == KEY_RIGHT) {
                if (direction < 4) {
                    direction += 1;
                }
            }
            else if (input == KEY_LEFT) {
                if (direction > 1) {
                    direction -= 1;
                }
            }
            mvwprintw(main_w, start_txt_y + 13, start_txt_x - 12, "Fight");
            mvwprintw(main_w, start_txt_y + 13, start_txt_x - 2, "Pack");
            mvwprintw(main_w, start_txt_y + 13, start_txt_x + 8, "PKMN");
            mvwprintw(main_w, start_txt_y + 13, start_txt_x + 18, "Run");
            if (direction == 1) {
                wattron(main_w, COLOR_PAIR(e)); 
                mvwprintw(main_w, start_txt_y + 13, start_txt_x - 12, "Fight");
                wattroff(main_w, COLOR_PAIR(e));
            }
            else if (direction == 2) {
                wattron(main_w, COLOR_PAIR(e)); 
                mvwprintw(main_w, start_txt_y + 13, start_txt_x - 2, "Pack");
                wattroff(main_w, COLOR_PAIR(e));
            }
            else if (direction == 3) {
                wattron(main_w, COLOR_PAIR(e)); 
                mvwprintw(main_w, start_txt_y + 13, start_txt_x + 8, "PKMN");
                wattroff(main_w, COLOR_PAIR(e));
            }
            else if (direction == 4) {
                wattron(main_w, COLOR_PAIR(e)); 
                mvwprintw(main_w, start_txt_y + 13, start_txt_x + 18, "Run");
                wattroff(main_w, COLOR_PAIR(e));
            }
        }
        else if (fightScreen) {
            if (input == KEY_RIGHT) {
                if (direction < 2) {
                    direction += 1;
                }
            }
            else if (input == KEY_LEFT) {
                if (direction > 1) {
                    direction -= 1;
                }
            }
            i = 0;
            for (const moves_t* mv : pc_pokemon.myMoves) {
                i++;
                if (i == 1) {
                    if (direction == 1) {
                        wattron(main_w, COLOR_PAIR(e)); 
                        mvwprintw(main_w, start_txt_y + 13, start_txt_x - 6,"%s", mv->identifier.c_str());
                        wattroff(main_w, COLOR_PAIR(e));
                    }
                    else {
                        mvwprintw(main_w, start_txt_y + 13, start_txt_x - 6,"%s", mv->identifier.c_str());
                    }
                }
                else {
                    if (direction == 2) {
                        wattron(main_w, COLOR_PAIR(e)); 
                        mvwprintw(main_w, start_txt_y + 13, start_txt_x + 8,"%s", mv->identifier.c_str());
                        wattroff(main_w, COLOR_PAIR(e));
                    }
                    else {
                        mvwprintw(main_w, start_txt_y + 13, start_txt_x + 8,"%s", mv->identifier.c_str());
                    }
                }
            }
        }
        else if (packScreen) {
            if (input == KEY_RIGHT) {
                if (direction < 3) {
                    direction += 1;
                }
            }
            else if (input == KEY_LEFT) {
                if (direction > 1) {
                    direction -= 1;
                }
            }
            mvwprintw(main_w, start_txt_y + 13, start_txt_x - 13, "Revive:%d", wrld->pc.bag[0]);
            mvwprintw(main_w, start_txt_y + 13, start_txt_x + 1, "Potion:%d", wrld->pc.bag[1]);
            mvwprintw(main_w, start_txt_y + 13, start_txt_x + 14, "Pokeballs:%d", wrld->pc.bag[2]);
            if (direction == 1) {
                wattron(main_w, COLOR_PAIR(e)); 
                mvwprintw(main_w, start_txt_y + 13, start_txt_x - 13, "Revive:%d", wrld->pc.bag[0]);
                wattroff(main_w, COLOR_PAIR(e));
            }
            else if (direction == 2) {
                wattron(main_w, COLOR_PAIR(e)); 
                mvwprintw(main_w, start_txt_y + 13, start_txt_x + 1, "Potion:%d", wrld->pc.bag[1]);
                wattroff(main_w, COLOR_PAIR(e));
            }
            else if (direction == 3) {
                wattron(main_w, COLOR_PAIR(e)); 
                mvwprintw(main_w, start_txt_y + 13, start_txt_x + 14, "Pokeballs:%d", wrld->pc.bag[2]);
                wattroff(main_w, COLOR_PAIR(e));
            }
            
        }
        else if (pkmnScreen) {

        }
        else if (runScreen) {
            wmove(main_w, start_txt_y + 12, 0);
            wclrtoeol(main_w);
            if (rand() % 100 < escape_chance) {
                mvwprintw(main_w, start_txt_y + 12, start_txt_x - 6, "Successfully escaped!");
                wrefresh(main_w);
                sleep(1);
                wmove(main_w, start_txt_y + 12, 0);
                wclrtoeol(main_w);
                break;
            }
            else {
                mvwprintw(main_w, start_txt_y + 12, start_txt_x - 6, "Did not escape!");
                wrefresh(main_w);
                sleep(1);
                escape_chance += 5;
                input = '\n';
                wmove(main_w, start_txt_y + 12, 0);
                wclrtoeol(main_w);
                continue;
            }
        }
        
        mvwprintw(main_w, start_txt_y_pc, start_txt_x_pc, pc_pokemon.name.c_str());
        mvwprintw(main_w, start_txt_y_pc + 1, start_txt_x_pc, "L:%d %c", pc_pokemon.level, pc_pokemon.gender == 1 ? 'M' : 'F');
        double health_percentage_pc = static_cast<double>(pc_pokemon.current_hp) / pc_pokemon.total_hp;
        int health_bar_length_pc = static_cast<int>(health_percentage_pc * 10);
        mvwprintw(main_w, start_txt_y_pc + 2, start_txt_x_pc, "HP: [");
        for (int i = 0; i < health_bar_length_pc; i++) {
            waddch(main_w, '|'); 
        }
        for (int i = health_bar_length_pc; i < 10; i++) {
            waddch(main_w, ' ');   
        }
        waddch(main_w, ']');
        mvwprintw(main_w, start_txt_y_pc + 3, start_txt_x_pc, "%d/%d", pc_pokemon.current_hp, pc_pokemon.total_hp);
        
        mvwprintw(main_w, start_txt_y_w, start_txt_x_w, pokemon.name.c_str());
        mvwprintw(main_w, start_txt_y_w + 1, start_txt_x_w, "L:%d %c", pokemon.level, pokemon.gender == 1 ? 'M' : 'F');
        double health_percentage = static_cast<double>(pokemon.current_hp) / pokemon.total_hp;
        int health_bar_length = static_cast<int>(health_percentage * 10);
        mvwprintw(main_w, start_txt_y_w + 2, start_txt_x_w, "HP: [");
        for (int i = 0; i < health_bar_length; i++) {
            waddch(main_w, '|'); 
        }
        for (int i = health_bar_length; i < 10; i++) {
            waddch(main_w, ' ');   
        }
        waddch(main_w, ']');
        mvwprintw(main_w, start_txt_y_w + 3, start_txt_x_w, "%d/%d", pokemon.current_hp, pokemon.total_hp);
        // mvwprintw(main_w, start_txt_y + 1, start_txt_x, "xp: %d", pokemon.xp);
        // mvwprintw(main_w, start_txt_y + 3, start_txt_x, "atk: %d", pokemon.atk);
        // mvwprintw(main_w, start_txt_y + 4, start_txt_x, "def: %d", pokemon.def);
        // mvwprintw(main_w, start_txt_y + 5, start_txt_x, "speed: %d", pokemon.speed);
        // mvwprintw(main_w, start_txt_y + 6, start_txt_x, "special atk: %d", pokemon.special_atk);
        // mvwprintw(main_w, start_txt_y + 7, start_txt_x, "special def: %d", pokemon.special_def);
        // pokemon.hp += 1;
        // i = 0;
        // for (const moves_t* mv : pokemon.myMoves) {
        //     mv.
        //     mvwprintw(main_w, start_txt_y + 9 + i, start_txt_x,
        //                 "move %d: %s", (i + 1), mv->identifier.c_str());
        //     i++;
        // }
        

        // mvwprintw(main_w, start_txt_y + 12, start_txt_x - 6, "(Press enter to select)");
        // mvwprintw(main_w, start_txt_y + 13, start_txt_x + 3, "y");
        // mvwprintw(main_w, start_txt_y + 13, start_txt_x + 6, "n");

        wrefresh(main_w);
        input = wgetch(main_w);
    }

    print_map(wrld);

    return captured;
}
