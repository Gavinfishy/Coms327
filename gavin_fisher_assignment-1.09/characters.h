/****************************************
 * Adhere to statement in COPYWRITE.txt *
****************************************/

#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <cstdint>
#include <limits>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>
#include <list>

#include "db.h"
#include "heap.h"

typedef enum cardinal {
    N,  
    E,
    S,
    W
} cardinal_t;

typedef enum character_type {
    player = 11,
    h,
    r,
    p,
    w,
    s,
    e
} character_t;

class Pokemon {
    public:
        std::string name;
        const pokemon_t* pokemon;
        int pokemon_id;
        int level;
        int xp;
        int hp;
        int atk;
        int def;
        int speed;
        int special_atk;
        int special_def;
        bool isShiny;
        bool isKnockedOut;
        std::list<const moves_t*> myMoves;
        int gender;

    void add_move (int num_moves) {
        int i, random;
        bool inMoves = false;

        /*
            get the level up moveset for this pokemon, and does not allow duplicate moves
        */
        std::vector<const pokemon_moves_t*> level_up_moveset;
        for (const pokemon_moves_t& move: pokemon_moves) {
            inMoves = false;
            if (pokemon->species_id == move.pokemon_id &&
                    move.pokemon_move_method_id == 1 && 
                    move.level <= this->level) {

                    level_up_moveset.push_back(&move);
            }
        }

        /*
            randomly chooses moves from the level_up_moveset and does not allow duplicate moves
        */
        for (i = 0; i < num_moves; i++) {
            random = rand() % level_up_moveset.size();
            for (const moves_t& move: moves) {
                inMoves = false;
                if (level_up_moveset[random]->move_id == move.id){
                    for (const moves_t* mm: this->myMoves) {
                        if (move.id == mm->id) {
                            inMoves = true;
                        }
                    }
                    if (!inMoves) {
                        this->myMoves.push_back(&move);
                    }
                }
            }
        }

        // if the pokemon has no moves add struggle to its moveset
        if (this->myMoves.size() == 0) {
            for (const moves_t &move : moves) {
                if (move.identifier == "struggle") {
                    this->myMoves.push_back(&move);
                    break;
                }
            }
        }
    }

    Pokemon(std::string name, int level) {
        int i;
        int iv = 0;
        
        this->name = name;
        this->level = level;
        this->xp = 0;
        this->isShiny = rand() % 8192 == 0;
        this->gender = rand() % 2;
        this->isKnockedOut = false;
        
        auto it = std::find_if(pokemons.begin(), pokemons.end(),
                                [&name](const pokemon_t& pokemon) {
                                    return pokemon.identifier == name; });
        if (it == pokemons.end()) {
            return;
        }

        const pokemon_t& p = *it;
        
        this->pokemon = &p;
        this->pokemon_id = p.id;

        this->hp = iv;

        for (i = 1; i <= 6; i++) {
            iv = rand() % 16; // random number 0-15
            auto it = std::find_if(pokemon_stats.begin(), pokemon_stats.end(),
                        [&](const pokemon_stats_t& s) {
                            return s.pokemon_id == pokemon_id &&
                                   s.stat_id == i;
                        });


            const pokemon_stats_t& s = *it;
            
            switch (i)
            {
            case 1:
                this->hp = floor(((((s.base_stat + iv)*2)*this->level)/100)+this->level+10);
                break;
            case 2:
                this->atk = floor(((((s.base_stat + iv)*2)*this->level)/100)+5);
                break;
            case 3:
                this->def = floor(((((s.base_stat + iv)*2)*this->level)/100)+5);
                break;
            case 4:
                this->speed = floor(((((s.base_stat + iv)*2)*this->level)/100)+5);
                break;
            case 5:
                this->special_atk = floor(((((s.base_stat + iv)*2)*this->level)/100)+5);
                break;
            case 6:
                this->special_def = floor(((((s.base_stat + iv)*2)*this->level)/100)+5);
                break;
            
            default:
                break;
            }
        }

        add_move (2);
    }

    Pokemon (const int &wrld_y, const int &wrld_x) {
        int i;
        int iv = 0;

        int man_distance = abs(wrld_y - 200) + abs(wrld_x - 200);

        if (man_distance <= 200) {
            this->level = std::max(1, (man_distance / 2));
        }
        else {
            this->level = std::min(((man_distance - 200) / 2), 100);
        }
        
        this->pokemon_id = (rand() % pokemons.size()) + 1;
        this->xp = 0;
        this->isShiny = rand() % 8192 == 0;
        this->gender = rand() % 2;
        this->isKnockedOut = false;
        
        this->pokemon = &pokemons[this->pokemon_id];
        this->name = this->pokemon->identifier;

        this->hp = iv;

        for (i = 1; i <= 6; i++) {
            iv = rand() % 16; // random number 0-15
            auto it = std::find_if(pokemon_stats.begin(), pokemon_stats.end(),
                        [&](const pokemon_stats_t& s) {
                            return s.pokemon_id == pokemon_id &&
                                   s.stat_id == i;
                        });


            const pokemon_stats_t& s = *it;
            
            switch (i)
            {
            case 1:
                this->hp = floor(((((s.base_stat + iv)*2)*this->level)/100)+this->level+10);
                break;
            case 2:
                this->atk = floor(((((s.base_stat + iv)*2)*this->level)/100)+5);
                break;
            case 3:
                this->def = floor(((((s.base_stat + iv)*2)*this->level)/100)+5);
                break;
            case 4:
                this->speed = floor(((((s.base_stat + iv)*2)*this->level)/100)+5);
                break;
            case 5:
                this->special_atk = floor(((((s.base_stat + iv)*2)*this->level)/100)+5);
                break;
            case 6:
                this->special_def = floor(((((s.base_stat + iv)*2)*this->level)/100)+5);
                break;
            
            default:
                break;
            }
        }

        add_move (2);
    }

    void level_up () {

    }
 };

class Character {
    public:
        virtual ~Character() {}
        uint8_t x;
        uint8_t y;
        character_t type;
        bool isDefeated;
        std::vector<Pokemon> pokemon_collection;

        void add_pokemon (const int &wrld_y, const int &wrld_x) {
            this->pokemon_collection.push_back(Pokemon(wrld_y, wrld_x));
        }

        void add_pokemon (Pokemon p) {
            this->pokemon_collection.push_back(p);
        }
};

class PC : public Character {
    public:
        PC () {
            this->y = 0;
            this->x = 0;
            this->type = player;
            this->isDefeated = false;
        }

        void add_pokemon_by_name (std::string name, int level) {
            this->pokemon_collection.push_back(Pokemon(name, level));
        }

};

class NPC : public Character {
    public:

        cardinal_t direction;

        NPC (const int &wrld_y, const int &wrld_x) {

            int num_pokemon = 1;

            type = (character_t) ((rand() % 6) + h);
            direction = (cardinal_t) (rand() % 4);
            isDefeated = false;

            add_pokemon(wrld_y, wrld_x); //@todo change lvl by distance
            while ((rand() % 10) < 6 && num_pokemon < 6) {
                add_pokemon(wrld_y, wrld_x) ;
                num_pokemon++;
            }

        }
};

typedef struct turn {
    heap_node_t *hn;
    Character *character;
    int seq_num;
    int next_turn;
} turn_t;



#endif