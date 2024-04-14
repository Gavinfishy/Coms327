/****************************************
 * Adhere to statement in COPYWRITE.txt *
****************************************/

#include "map.h"
#include "art.h"

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

// void print_map(map_t *m) {
//     int x, y;
//     for (y = 0; y < Y_MAX; y++) {
//         for (x = 0; x < X_MAX; x++) {

//             if (clayer(y, x) != NULL) {
//                 switch (((struct character*)clayer(y,x))->type)
//                 {
//                     case h:
//                         printf( REDB "%c" COLOR_RESET, 'h');
//                         break;

//                     case r:
//                         printf( REDB "%c" COLOR_RESET, 'r');
//                         break;

//                     case p:
//                         printf( REDB "%c" COLOR_RESET, 'p');
//                         break;

//                     case w:
//                         printf( REDB "%c" COLOR_RESET, 'w');
//                         break;

//                     case s:
//                         printf( REDB "%c" COLOR_RESET, 's');
//                         break;

//                     case e:
//                         printf( REDB "%c" COLOR_RESET, 'e');
//                         break;

//                     case player:
//                         printf( WHTB "%c" COLOR_RESET, '@');
//                         break;

//                     default:
//                         printf( REDB "?" COLOR_RESET);
//                         break;
//                 }
//             } else {
//                 switch (m->ascii[y][x])
//                 {
//                     case sGrass:
//                         printf(BRGRNB "%c" COLOR_RESET, '.');
//                         break;
                    
//                     case tGrass:
//                         printf(GRN YELB "%c" COLOR_RESET, ':');
//                         break;
                    
//                     case path:
//                         printf(BLKB "%c" COLOR_RESET, '#');
//                         break;
                    
//                     case mtn:
//                         printf(BRBLKB "%c" COLOR_RESET, '%');
//                         break;

//                     case bldr:
//                         printf(BRBLKB "%c" COLOR_RESET, '%');
//                         break;

//                     case water:
//                         printf(BLUB "%c" COLOR_RESET, '~');
//                         break;

//                     case tree:
//                         printf(BLK GRNB "%c" COLOR_RESET, '^');
//                         break;

//                     case pCntr:
//                         printf(WHT BRCYANB "%c" COLOR_RESET, 'C');
//                         break;

//                     case pMart:
//                         printf(WHT BRMAGB "%c" COLOR_RESET, 'M');
//                         break;
                        
//                     default:
//                         printf("%d", m->ascii[y][x]);
//                         break;
//                 }
//             }
            
//         }
//         printf("\n");
//     }
    
// }

// // TODO
// void print_text(char a[3][X_MAX]) {

// }

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

    werase(main_w);

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

void print_top_msg(world_t *wrld, bool isInput, const char *msg, ...) {
    uint8_t i, n, shift;
    
    va_list args;
    va_start(args, msg);

    if (msg == NULL) {
        werase(top_msg_w);

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
        mvwprintw(top_msg_w, 0, shift, frmt_msg);
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

void start_screen (world_t *wrld) {
    uint8_t poke_idx = 1;
    int input, i;
    size_t c;

    wbkgd(main_w, COLOR_PAIR(tGrass));

    // print logo
    for (i = 0; i < 12; i++) {
        for (c = 0; c < pokemon_logo[i].length(); c++) {
            wattron(main_w, COLOR_PAIR(20)); 
            mvwprintw(main_w, i, c, "%c", pokemon_logo[i].at(c));
            wattroff(main_w, COLOR_PAIR(20));
        }
    }

    for (i = 0; i < 13; i++) {
        mvwprintw(main_w, (9 + i), 55, pikachu_art[i].c_str());
    }

    while(1) {
        mvwprintw(main_w, 14, 20, "Choose your Pokemon:");
        mvwprintw(main_w, 15, 20, "Squirtle");
        mvwprintw(main_w, 16, 20, "Bulbasaur");
        mvwprintw(main_w, 17, 20, "Charmander");
        
        switch (poke_idx)
        {
        case 1:
            wattron(main_w, COLOR_PAIR(23)); 
            mvwprintw(main_w, 15, 20, "Squirtle");
            wattroff(main_w, COLOR_PAIR(23));
            break;
        case 2:
            wattron(main_w, COLOR_PAIR(21)); 
            mvwprintw(main_w, 16, 20, "Bulbasaur");
            wattroff(main_w, COLOR_PAIR(21));
            break;
        case 3:
            wattron(main_w, COLOR_PAIR(22)); 
            mvwprintw(main_w, 17, 20, "Charmander");
            wattroff(main_w, COLOR_PAIR(22));
            break;
        
        default:
            break;
        }
        wrefresh(main_w);

        input = wgetch(main_w);
        if (input == KEY_UP && poke_idx > 1) {
            poke_idx--;
        }
        else if (input == KEY_DOWN && poke_idx < 3) {
            poke_idx++;
        }
        else if (input == '\n') {
            break;
        }
    }

    switch (poke_idx)
    {
    case 1:
        wrld->pc.add_pokemon_by_name("squirtle", 1);
        break;
    case 2:
        wrld->pc.add_pokemon_by_name("bulbasaur", 1);
        break;
    case 3:
        wrld->pc.add_pokemon_by_name("charmander", 1);
        break;
    
    default:
        break;
    }

    wbkgd(main_w, COLOR_PAIR(0));
}