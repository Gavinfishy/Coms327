#include "map.h"

/*
    returns whether bool of is trainer defeated
*/
bool battle(world_t *wrld, Character *trainer) {
    // trainer->addPokemon(trainer->x, trainer->y);
    wclear(main_w);
    int pokemonIndex = 1;
    switch (trainer->type) {
        case r:
            mvwprintw(main_w, 8, 35, "player vs rival");
            break; 
        case h:
            mvwprintw(main_w, 8, 35, "player vs hiker");
            break; 
        case s:
            mvwprintw(main_w, 8, 35, "player vs sentry");
            break; 
        case w:
            mvwprintw(main_w, 8, 35, "player vs wanderer");
            break;
        case e:
            mvwprintw(main_w, 8, 35, "player vs explorer");
            break;
        case p: 
            mvwprintw(main_w, 8, 35, "player vs pacer");
        default:
            break;
    }
    std::vector<Pokemon> tempPokemons;
    tempPokemons.push_back(Pokemon(wrld->curr_idx[0], wrld->curr_idx[1]));
            for (int i = 0; i < 5; i++) {
                if ((rand() % 100) <= 60) {
                    tempPokemons.push_back(Pokemon(wrld->curr_idx[0], wrld->curr_idx[1]));
                } else {
                    break;
                }
            }
            
            for (const auto& pokemon : tempPokemons) {
                wprintw(main_w, "\n    Pokemon %d: %s, Level: %d", pokemonIndex, pokemon.name.c_str(), pokemon.level);
                pokemonIndex++;
            }
    wrefresh(main_w);

    while (wgetch(main_w) != 27){

    }

    print_map(wrld);
    
    return true;
}