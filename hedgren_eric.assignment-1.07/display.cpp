#include "map.h"

#include <stdarg.h>

// ANSI escape codes for colors
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define BLKB "\e[40m"
#define REDB "\e[41m"
#define GRNB "\e[42m"
#define YELB "\e[43m"
#define BLUB "\e[44m"
#define MAGB "\e[45m"
#define CYNB "\e[46m"
#define WHTB "\e[47m"
#define BRGRNB "\e[0;102m"
#define BRBLKB "\e[0;100m"
#define BRCYANB "\e[0;106m"
#define BRMAGB "\e[0;105m"
#define COLOR_RESET "\e[0m"

#define print_ncurses(WIN, ch, t)                  \
    wattron(WIN, COLOR_PAIR(t));                      \
    wprintw(WIN, ch);                        \
    wattroff(WIN, COLOR_PAIR(t));                      \


void print_point_conversion(int y, int x) {
    printf("(%d, %d)", (x - 200), (y - 200));
}


void print_cost_map(int (*cost_map)[Y_MAX][X_MAX]) {
    int x, y;

    for (y = 0; y < Y_MAX; y++) {
        for (x = 0; x < X_MAX; x++) {
            if ((*cost_map)[y][x] > 1000) {
                printf("   ");
            }
            else {
                printf(" %02d", ((*cost_map)[y][x] % 100));
            }
        }
        printf("\n");
    }
}

void print_map(world_t *wrld) {
    int x, y;
    map_t *m = wrld->curr_map;

    wclear(main_w);

    for (y = 0; y < Y_MAX; y++) {
        for (x = 0; x < X_MAX; x++) {

            if (clayer(y, x) != NULL) {
                switch(clayer(y,x)->type)
                {
                    case h:
                        print_ncurses(main_w, "h", h);
                        break;

                    case r:
                        print_ncurses(main_w, "r", r);
                        break;

                    case p:
                        print_ncurses(main_w, "p", p);
                        break;

                    case w:
                        print_ncurses(main_w, "w", w);
                        break;

                    case s:
                        print_ncurses(main_w, "s", s);
                        break;

                    case e:
                        print_ncurses(main_w, "e", e);
                        break;

                    case player:
                        print_ncurses(main_w, "@", player);
                        break;

                    default:
                        wprintw(main_w, "X");
                        break;
                }

            } else {
                switch (m->ascii[y][x])
                {
                    case sGrass:
                        print_ncurses(main_w, ".", sGrass);
                        break;
                    
                    case tGrass:
                        print_ncurses(main_w, ":", tGrass);
                        break;
                    
                    case path:
                        print_ncurses(main_w, "#", path);
                        break;
                    
                    case mtn:
                        print_ncurses(main_w, "%%", mtn);
                        break;

                    case bldr:
                        print_ncurses(main_w, "%%", bldr)
                        break;

                    case water:
                        print_ncurses(main_w, "~", water);
                        break;

                    case tree:
                        print_ncurses(main_w, "^", tree);
                        break;

                    case pCntr:
                        print_ncurses(main_w, "C", pCntr);
                        break;

                    case pMart:
                        print_ncurses(main_w, "M", pMart);
                        break;
                        
                    default:
                        wprintw(main_w, "?");
                        break;
                }   
            }
        }
    }
    wrefresh(main_w);
}

// print_battle

void print_top_msg(world_t *wrld, bool isInput, const char *msg, ...) {
    uint8_t i, n, shift;
    
    va_list args;
    va_start(args, msg);

    if (msg == NULL) {
        wclear(top_msg_w);

        for (i = 0; i < 80; i++) {
            print_ncurses(top_msg_w, "`", 19);
        }
    }
    else {
        char *frmt_msg = (char*) malloc(sizeof(char) * strlen(msg));
        n = strlen(msg);

        if (isInput) shift = 35;
        else shift = (40 - (int) (n/2));

        if (n > 80) return;

        vsprintf(frmt_msg, msg, args);

        wattron(top_msg_w, COLOR_PAIR(e));
        mvwprintw(top_msg_w, 0, shift, "%s", frmt_msg);
        wattroff(top_msg_w, COLOR_PAIR(e));

        free(frmt_msg);
    }

    va_end(args);
    wrefresh(top_msg_w);
}

void print_bottom_msg(world_t *wrld, char *msg) {
    uint8_t i, n, line_one_end;
    
    if (msg == NULL) {
        for (i = 0; i < 160; i++) {
            print_ncurses(bottom_msg_w, "`", 19);
        }
    }
    else {
        n = strlen(msg);

        if (n > 152) return;

        line_one_end = n;

        // if (n > 72) {
        //     for (i = 0; i < n; i++) {
        //         if (i == 72) {
        //             while (msg[i] != ' ') {
        //                 //dont print if no spaces at all
        //                 if (i == 0) return;
        //                 i--;
        //             }
        //             line_one_end = i - 1;
        //             break;
        //         }
        //     }
        // }

        for (i = n/2; i > 0; i--) {
            if ((n - i) > 76) return;

            if (msg[i] == ' ') {
                line_one_end = i - 1;
                break;
            }
        }

        for (i = 0; i < line_one_end; i++) {
            
        }
    }
    wrefresh(bottom_msg_w);
}