#include "map.h"


/*
    WINDOW *newwin(
             int nlines, int ncols,
             int begin_y, int begin_x);
*/
int trainers_window(map_t *m, WINDOW *parent_WIN) {
    uint8_t num_rows = 10;
    uint8_t num_cols = 20;
    uint8_t begin_y = 6;
    uint8_t begin_x = 31;
    uint8_t i;
    int16_t shift_max = m->num_trainers - num_rows;
    uint16_t shift = 0;
    int input = 0;

    WINDOW *trainer_win = subwin(parent_WIN, num_rows, num_cols,
                                    begin_y, begin_x);
    keypad(trainer_win, TRUE);

    while(1) {

        if (shift_max > 0 && shift < shift_max && input == KEY_DOWN) {
            shift++;
        }
        else if (shift > 0 && input == KEY_UP) {
            shift--;
        }
        else if (input == 27) { //esc key
            break;
        }
        else if (input == 'Q' || input == 'q') {
            return 'Q';
        }

        wclear(trainer_win);

        wbkgd(trainer_win, COLOR_PAIR(e));

        for (i = 0; i < num_rows; i++) {

            if ((i + shift) >= m->num_trainers) {
                wprintw(trainer_win, " ");
            }
            else {
                switch (m->turn_table[i + shift].character->type)
                {
                case h:
                    wprintw(trainer_win, "%3d: hiker", (i + shift + 1));
                    break;

                case r:
                    wprintw(trainer_win, "%3d: rival", (i + shift + 1));
                    break;

                case p:
                    wprintw(trainer_win, "%3d: pacer", (i + shift + 1));
                    break;

                case w:
                    wprintw(trainer_win, "%3d: wanderer", (i + shift + 1));
                    break;

                case s:
                    wprintw(trainer_win, "%3d: sentry", (i + shift + 1));
                    break;

                case e:
                    wprintw(trainer_win, "%3d: explorer", (i + shift + 1));
                    break;

                default:
                    break;
                }
            }

            if (i < (num_rows)) {
                wprintw(trainer_win, "\n");
            }
        }

        wrefresh(trainer_win);
        
        input = wgetch(trainer_win);

    }

    delwin(trainer_win);

    return 0;
}

int poke_mart_window(world_t *wrld) {
    uint8_t num_rows = 15;
    uint8_t num_cols = 68;
    uint8_t begin_y = 4;
    uint8_t begin_x = 6;
    int input = 0;

    WINDOW *pMart_win = subwin(main_w, num_rows, num_cols,
                                    begin_y, begin_x);

    while(1) {
        if (input == 't') {
            input = trainers_window(wrld->curr_map, pMart_win);
        }

        if (input == '<') {
            break;
        }
        else if (input == 'Q' || input == 'q') {
            return 'Q';
        }

        wclear(pMart_win);

        mvwprintw(main_w, 5, 30, "MART");

        wrefresh(pMart_win);

        input = wgetch(pMart_win);
    }

    delwin(pMart_win);
    
    return 0;
}

int poke_center_window(world_t *wrld) {
    uint8_t num_rows = 15;
    uint8_t num_cols = 68;
    uint8_t begin_y = 4;
    uint8_t begin_x = 6;
    int input = 0;

    WINDOW *pCntr_win = subwin(main_w, num_rows, num_cols,
                                    begin_y, begin_x);

    while(1) {
        if (input == 't') {
            input = trainers_window(wrld->curr_map, pCntr_win);
        }

        if (input == '<') {
            break;
        }
        else if (input == 'Q' || input == 'q') {
            return 'Q';
        }

        wclear(pCntr_win);

        mvwprintw(main_w, 5, 30, "CENTER");

        wrefresh(pCntr_win);

        input = wgetch(pCntr_win);
    }

    delwin(pCntr_win);
    
    return 0;
}

int pokemon_encounter_window(world_t *wrld, std::list<pokemon_t> pokemons, int index) {
    uint8_t num_rows = 15;
    uint8_t num_cols = 68;
    uint8_t begin_y = 4;
    uint8_t begin_x = 6;
    int input = 0;
    WINDOW *pEncntr_win = subwin(main_w, num_rows, num_cols, begin_y, begin_x);

    // int manhattan = (abs(wrld->curr_idx[0]-200) + abs(wrld->curr_idx[1]-200));
    // int minLevel;
    // int maxLevel;
    // if (manhattan <= 200) {
    //     minLevel = 1;
    //     maxLevel = (manhattan > 0) ? manhattan / 2 : 1;
    // }
    // else {
    //     minLevel = (manhattan - 200) / 2;
    //     maxLevel = 100;
    // }
    // int level = minLevel + rand() % (maxLevel - minLevel + 1);
    while(1) {
        if (input == 27) { //esc key
            break;
        }
        else if (input == 'Q' || input == 'q') {
            return 'Q';
        }
        wclear(pEncntr_win);
        // auto it = std::next(pokemons.begin(), index);
        Pokemon encounteredPokemon(1, 1);
        int moveIndex = 1;
        for (const auto& move : encounteredPokemon.move) {
            mvwprintw(main_w, 7 + moveIndex, 30, "Move %d: %s", moveIndex, move->identifier.c_str());
            moveIndex++;
        }
        mvwprintw(main_w, 8 + moveIndex, 30, "HP: %d", encounteredPokemon.hp_cur);
        mvwprintw(main_w, 9 + moveIndex, 30, "Attack: %d", encounteredPokemon.attack_cur);
        // ... print other _cur variables ...
        mvwprintw(main_w, 10 + moveIndex, 30, "Gender: %d", encounteredPokemon.gender);
        mvwprintw(main_w, 11 + moveIndex, 30, "Shiny: %d", encounteredPokemon.shiny);

        // pokemon_t encounteredPokemon = *it;
        // mvwprintw(main_w, 5, 30, "A wild %s appeared!", encounteredPokemon.identifier.c_str());
        // mvwprintw(main_w, 6, 30, "Level: %d", level);
        // mvwprintw(main_w, 7, 30, "Moves");

        wrefresh(pEncntr_win);
        input = wgetch(pEncntr_win);
    }
    delwin(pEncntr_win);
    return 0;
}
