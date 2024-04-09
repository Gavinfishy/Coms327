#include "map.h"

/*
    returns whether bool of is trainer defeated
*/
bool battle(world_t *wrld, Character *trainer) {

    wclear(main_w);

    switch (trainer->type) {
        case r:
            mvwprintw(main_w, 9, 35, "player vs rival");
            break; 
        case h:
            mvwprintw(main_w, 9, 35, "player vs hiker");
            break; 
        case s:
            mvwprintw(main_w, 9, 35, "player vs sentry");
            break; 
        case w:
            mvwprintw(main_w, 9, 35, "player vs wanderer");
            break;
        case e:
            mvwprintw(main_w, 9, 35, "player vs explorer");
            break;
        case p: 
            mvwprintw(main_w, 9, 35, "player vs pacer");
        default:
            break;
    }

    wrefresh(main_w);

    while (wgetch(main_w) != 27){

    }

    print_map(wrld);
    
    return true;
}