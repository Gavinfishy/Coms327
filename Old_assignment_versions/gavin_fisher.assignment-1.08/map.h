#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <curses.h>
#include <vector>

#include "characters.h"
#include "db.h"

#define ASCII_INIT_VAL 48
#define Y_MAX 21 // screen length
#define X_MAX 80 // screen width
#define WRLD_MAX 401 // world screen length and width
#define INF (INT_MAX / 2) 

#define mapyx(y, x) m->ascii[y][x]
#define clayer(y, x) m->c_map[y][x]

#define wrldyx(y, x) wrld->world[y][x]
#define curr_y (wrld->curr_idx[dim_y])
#define curr_x (wrld->curr_idx[dim_x])
#define curr_m (wrld->curr_map)
#define top_msg_w (wrld->top_msg_win)
#define main_w (wrld->main_win)
#define bottom_msg_w (wrld->bottom_msg_win)

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// struct pokemonCenter { // 2x2 block
//     int x;
//     int y;
//     /*
//         will need other things for the center
//     */
// };

// struct pokemart {
//     /*
//         will need other things for the center
//     */
// };
typedef struct terrain_weight {
  uint32_t bldr;
  uint32_t tree;
  uint32_t path;
  uint32_t pMart;
  uint32_t pCntr;
  uint32_t tGras;
  uint32_t sGras;
  uint32_t mtn;
  uint32_t water;
  uint32_t gate;
} terrain_weight_t;

typedef enum terrain {
  bldr,
  tree,
  path,
  pMart,
  pCntr,
  tGrass,
  sGrass,
  mtn,
  water,
  gate,
  UNKNOWN
} terrain_t;

typedef enum colors {
  bldr_gray = 9,
  mtn_gray,
  tGrass_yllw,
  path_brown,

} colors_t;

class Map {
  public:
  // char name[25]; may choose to add a name of the map in the future
  terrain_t ascii [Y_MAX][X_MAX]; // terrain map
  Character *c_map[Y_MAX][X_MAX]; // character map (pointers to character structs)
  // uint8_t elevations [Y_MAX][X_MAX];
  uint8_t n, s, e, w;
  size_t num_trainers;
  turn_t turn_table[]; // default number of characters right now is 10
  // struct pokemonCenter center;
  // struct pokemart mart;
};

typedef Map map_t;

typedef struct path {
  heap_node_t *hn;
  uint8_t pos[2];
  uint8_t from[2];
  uint32_t cost;
} path_t;

typedef enum dim {
  dim_x,
  dim_y,
  num_dims
} dim_t;

typedef int16_t point_t[num_dims];

class World {
  public:
  map_t *world[WRLD_MAX][WRLD_MAX];
  point_t curr_idx;
  map_t *curr_map;

  /*placing distance maps and pc in world beause only
  need these for current map*/
  int hiker_cost[Y_MAX][X_MAX];
  int rival_cost[Y_MAX][X_MAX];
  int sentry_cost[Y_MAX][X_MAX];
  PC pc;
  turn_t pc_turn;

  WINDOW *top_msg_win;
  WINDOW *main_win;
  WINDOW *bottom_msg_win;
};


typedef World world_t;

extern int max_num_trainers;

extern int32_t move_cost[7][10];
#define MV_COST(c, t) (move_cost[c % 11][t]) // enum start at 11

void generate_map(world_t *wrld);
void print_point_conversion(int x, int y);
// void print_map(map_t *m);
void print_cost_map(int (*cost_map)[Y_MAX][X_MAX]);
void dijkstra_npc(map_t *m, int (*cost_map)[Y_MAX][X_MAX], PC *pc, character_t npc);

void generate_npc (map_t* m, NPC *npc);
// static void dijkstra_path(map_t *m, point_t from, point_t to)
void generate_pc(world_t *wrld);

// void map_mover_cmd(world_t *wrld, uint8_t num_trainers);
void turn_move(world_t *wrld);

void print_map(world_t *wrld);

// windows
int trainers_window(map_t *m, WINDOW *parent_WIN);
int poke_mart_window(world_t *wrld);
int poke_center_window(world_t *wrld);
int pokemon_encounter_window(world_t *wrld, std::list<pokemon_t> pokemons, int index);
int pokemon_start_win(world_t *wrld);

void print_top_msg(world_t *wrld, bool isInput, const char *msg, ...);
void print_bottom_msg(world_t *wrld, char *msg);

bool battle(world_t *wrld, Character *trainer);

void map_mover(world_t *wrld, cardinal_t dir, char input,
                    int16_t wrld_y, int16_t wrld_x);

#endif