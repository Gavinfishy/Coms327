#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <stdint.h>
#include <limits.h>

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

class Character {
    public:
        virtual ~Character() {}
        uint8_t x;
        uint8_t y;
        character_t type;
        bool isDefeated;
        cardinal_t direction;
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