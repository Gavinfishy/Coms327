/****************************************
 * Adhere to statement in COPYWRITE.txt *
****************************************/

#include "map.h"
#include "characters.h"

#include <cstring>

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
    wbkgd(trainer_win, COLOR_PAIR(e));

    while(1) {

        input = wgetch(trainer_win);

        if (shift_max > 0 && shift < shift_max && input == KEY_DOWN) {
            shift++;
        }
        else if (shift > 0 && input == KEY_UP) {
            shift--;
        }
        else if (input == 27) { //esc key
            break;
        }
        else if (input == 'Q') {
            return 'Q';
        }

        werase(trainer_win);

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

    werase(pMart_win);

    while(1) {
        input = wgetch(pMart_win);

        if (input == 't') {
            input = trainers_window(wrld->curr_map, pMart_win);
        }

        if (input == '<') {
            break;
        }
        else if (input == 'Q') {
            return 'Q';
        }

        mvwprintw(main_w, 5, 30, "MART");

        wrefresh(pMart_win);
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

    werase(pCntr_win);

    while(1) {

        input = wgetch(pCntr_win);

        if (input == 't') {
            input = trainers_window(wrld->curr_map, pCntr_win);
        }

        if (input == '<') {
            break;
        }
        else if (input == 'Q') {
            return 'Q';
        }

        mvwprintw(main_w, 5, 30, "CENTER");

        wrefresh(pCntr_win);
    }

    delwin(pCntr_win);
    
    return 0;
}