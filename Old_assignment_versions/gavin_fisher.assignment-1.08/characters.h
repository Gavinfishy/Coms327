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
    int id;
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
    Pokemon(int pokemon_id) {
        id = pokemon_id;
        for (const auto& pokemon : pokemons) {
            if (pokemon.id == pokemon_id) {
                name = pokemon.identifier;
                break;
            }
        }
        level = 1;

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
    Pokemon(int x, int y) {
        int index = rand() % pokemons.size();
        auto it = std::next(pokemons.begin(), index);
        pokemon_t encounteredPokemon = *it;
        name = encounteredPokemon.identifier;
        id = encounteredPokemon.id;

        int manhattan = abs(x - 200) + abs(y - 200);
        int minLevel;
        int maxLevel;
        if (manhattan <= 200) {
            minLevel = 1;
            maxLevel = (manhattan > 0) ? manhattan / 2 : 1;
        } 
        else {
            minLevel = (manhattan - 200) / 2;
            maxLevel = 100;
        }
        level = minLevel + rand() % (maxLevel - minLevel + 1);

        std::vector<const moves_t*> eligible_moves;
        for (const auto& pm : pokemon_moves) {
            if (pm.pokemon_id == id && pm.pokemon_move_method_id == 1) {
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
            if (stat.pokemon_id == id) {
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
        virtual void addPokemon(int x, int y) = 0;
};

class PC : public Character {
    public:
        void addPokemon(int x, int y) override {
            pokemons.push_back(Pokemon(x, y));
        }
};

class NPC : public Character {
    public:
        void addPokemon(int x, int y) override {
            pokemons.push_back(Pokemon(x, y)); 

            for (int i = 0; i < 5; i++) {
                if ((rand() % 100) < 60) {
                    pokemons.push_back(Pokemon(x, y));
                } else {
                    break;
                }
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