#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <stdint.h>
#include <limits.h>
#include <vector>
#include "heap.h"
#include "db.h"
#include <cstring>
#include <list>

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
    int level;
    std::list<const moves_t*> move;
    int hp_b;
    int hp_IV;
    int hp_cur;
    int attack_b;
    int attack_IV;
    int attack_cur;
    int defense_b;
    int defense_IV;
    int defense_cur;
    int special_attack_b;
    int special_attack_IV;
    int special_attack_cur;
    int special_defense_b;
    int special_defense_IV;
    int special_defense_cur;
    int speed_b;
    int speed_IV;
    int speed_cur; 
    int accuracy_b;
    int accuracy_IV;
    int accuracy_cur; 
    int evasion_b;
    int evasion_IV;
    int evasion_cur; 
    int gender;
    int shiny;
    Pokemon(int pokemon_id, int lvl) {
        for (const auto& pokemon : pokemons) {
            if (pokemon.id == pokemon_id) {
                name = pokemon.identifier;
                break;
            }
        }
        level = lvl;
        std::vector<const moves_t*> eligible_moves;

        for (const auto& pm : pokemon_moves) {
            if (pm.pokemon_id == pokemon_id && pm.pokemon_move_method_id == 1) {
                for (auto& m : moves) {
                    if (m.id == pm.move_id) {
                        eligible_moves.push_back(&m);
                        break;
                    }
                }
            }
        }
        while (move.size() < 2 && !eligible_moves.empty()) {
            int index = rand() % eligible_moves.size();
            move.push_back(eligible_moves[index]);
            eligible_moves.erase(eligible_moves.begin() + index);
        }
        for (const auto& stat : pokemon_stats) {
            if (stat.pokemon_id == pokemon_id) {
                switch (stat.stat_id) {
                    case 1: 
                        hp_b = hp_cur = stat.base_stat; 
                        break;
                    case 2: 
                        attack_b = attack_cur = stat.base_stat; 
                        break;
                    case 3: 
                        defense_b = defense_cur = stat.base_stat; 
                        break;
                    case 4: 
                        special_attack_b = special_attack_cur = stat.base_stat;
                        break;
                    case 5:
                        special_defense_b = special_defense_cur = stat.base_stat;
                        break;
                    case 6:
                        speed_b = speed_cur = stat.base_stat;
                        break;
                    case 7:
                        accuracy_b = accuracy_cur = stat.base_stat;
                        break;
                    case 8:
                        evasion_b = evasion_cur = stat.base_stat;
                        break;
                }
            }
        }
        hp_IV = rand() % 16;
        attack_IV = rand() % 16;
        defense_IV = rand() % 16;
        special_attack_IV = rand() % 16;
        special_defense_IV = rand() % 16;
        speed_IV = rand() % 16;
        accuracy_IV = rand() % 16;
        evasion_IV = rand() % 16;
        gender = rand() % 2;
        shiny = (rand() % 8192) == 0;
    }
};

class Character {
    public:
        virtual ~Character() {}
        uint8_t x;
        uint8_t y;
        character_t type;
        bool isDefeated;
        cardinal_t direction;
        std::vector<Pokemon> pokemons;
        // void new_pokemon(name, level) {

        // }
        // void new_pokemon(x,y){

        // }
};

class PC : public Character {

};

class NPC : public Character {
};

typedef struct turn {
    heap_node_t *hn;
    Character *character;
    int seq_num;
    int next_turn;
} turn_t;



#endif