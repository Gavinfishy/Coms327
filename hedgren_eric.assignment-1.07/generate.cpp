#include "map.h"

void generate_gate (map_t *m, char cardinal, uint8_t g) {
    switch (cardinal)
    {
    case N:
        if (g == 0) {
            m->n = rand() % 78 + 1;
        }
        else {
            m->n = g;
        }
        mapyx(0, m->n) = path;
        break;
    
    case S:
        if (g == 0) {
            m->s = rand() % 78 + 1;
        }
        else {
            m->s = g;
        }
        mapyx((Y_MAX - 1), m->s) = path;
        break;
    
    case E:
        if (g == 0) {
            m->e = rand() % 19 + 1;
        }
        else {
            m->e = g;
        }
        mapyx(m->e, (X_MAX - 1)) = path;
        break;

    case W:
        if (g == 0) {
            m->w = rand() % 19 + 1;
        }
        else {
            m->w = g;
        }
        mapyx(m->w, 0) = path;
        break;

    default:
        break;
    }
}

void generate_wall (map_t *m, cardinal_t cardinal) {
    uint8_t e = 0;
    uint8_t i;
    if (cardinal == N || cardinal == S) {
        if (cardinal == N) {
            e = 0;
        }
        else { // south
            e = Y_MAX - 1;
        }
        
        for (i = 0; i < X_MAX; i++) {
            if (mapyx(e, i) != path) {
                mapyx(e, i) = bldr;
            }

        }

    }
    else {
        if (cardinal == E) {
            e = X_MAX - 1;
        }
        else { // west
            e = 0;
        }
        
        for (i = 0; i < Y_MAX; i++) {
            if (mapyx(i, e) != path) {
                mapyx(i, e) = bldr;
            }
        }
    }
}

void generate_terrain(map_t *m) {

    uint16_t inc = 0;
    bool isFinished = false;
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t sum = 0;
    uint8_t random;
    uint8_t t, length, width, zero;
    int i, j;

    uint8_t numTerrains = 6;
    terrain_t terrains[numTerrains];

    for (t = 0; t < numTerrains; t++) {
        random = rand() % 100;
        if (sum <= 2) { // dont want too many water and mtns, so sum limits it to 2
            if (random < 35) {
                terrains[t] = sGrass;
            }
            else if (random < 70) {
                terrains[t] = tGrass;
            }
            else if (random < 90) {
                terrains[t] = water;
                sum++;
            }
            else {
                terrains[t] = mtn;
                sum++;
            }
        }
        else {
            if (random < 50) {
                terrains[t] = sGrass;
            }
            else {
                terrains[t] = tGrass;
            }
        }
    }

    point_t pts[numTerrains];
    bool terrainDone[numTerrains];

    for (t = 0; t < numTerrains; t++) {
        if (terrains[t] == mtn || terrains[t] == water){
            pts[t][dim_x] = (rand() % (X_MAX - 5)) + 3; // no real reason I chose these numbers just looked nice
            pts[t][dim_y] = (rand() % (Y_MAX - 4)) + 3;
        }
        else {
            pts[t][dim_x] = (rand() % (X_MAX - 2)) + 1;
            pts[t][dim_y] = (rand() % (Y_MAX - 2)) + 1;
        }
        terrainDone[t] = false;
    } 
    while (!isFinished) {
        for (t = 0; t < numTerrains; t++) {
            x = pts[t][dim_x];
            y = pts[t][dim_y];
            terrainDone[t] = true;

            // reduce the size of the water region
            if ((terrains[t] == water || terrains[t] == mtn)) {
                if (inc > 6) {
                    continue;
                }
                width = X_MAX - 3;
                length = Y_MAX - 3;
                zero = 2;
            }
            else {
                width = X_MAX - 1;
                length = Y_MAX - 1;
                zero = 0;
            }

            for (j = y - inc; j <= y + inc; j++) {
                if (zero < j && j < length) {
                    if ((x-inc) > zero && mapyx(j, (x - inc)) == UNKNOWN) {
                        terrainDone[t] = false;
                        mapyx(j, (x - inc)) = terrains[t];
                    }
                    if ((x+inc) < (width) && mapyx(j, (x + inc)) == UNKNOWN) {
                        terrainDone[t] = false;
                        mapyx(j, (x + inc)) = terrains[t];
                    }
                }
            }

            for (i = x - inc; i <= x + inc; i++) {
                if (zero < i && i < (width)) {
                    if ((y-inc) > zero && mapyx((y - inc), i) == UNKNOWN) {
                        terrainDone[t] = false;
                        mapyx((y - inc), i) = terrains[t];
                    }
                    if ((y+inc) < (length) && mapyx((y + inc), i) == UNKNOWN) {
                        terrainDone[t] = false;
                        mapyx((y + inc), i) = terrains[t];
                    }
                }
            }
        }
        sum = 0;
        for (t = 0; t < numTerrains; t++) {
            sum += terrainDone[t];
        }
        isFinished = (sum == numTerrains);
        inc++;
    }
}

void generate_path(map_t *m) {
    uint8_t y_NS = rand() % 14 + 4;
    uint8_t x_EW = rand() % 60 + 10;
    uint8_t n = m->n;
    uint8_t s = m->s;
    uint8_t e = m->e;
    uint8_t w = m->w;
    uint8_t i;

    if (n == 0) {
        n = 40;
    }
    if (s == 0) {
        s = 40;
    }
    if (w == 0) {
        w = 10;
    }
    if (e == 0) {
        e = 10;
    }

    uint8_t min_NS = MIN(n, s);
    uint8_t min_EW = MIN(e, w);

    for (i = min_NS; i <= MAX(n, s); i++) {
        mapyx(y_NS, i) = path;
    }
    for (i = 1; i < y_NS; i++) {
        mapyx(i, n) = path;
    }
    for (i = y_NS; i < (Y_MAX - 1); i++) {
        mapyx(i, s) = path;
    }
    
    for (i = min_EW; i <= MAX(e, w); i++) {
        mapyx(i, x_EW) = path;
    }
    for (i = 1; i < x_EW; i++) {
        mapyx(w, i) = path;
    }
    for (i = x_EW; i < (X_MAX - 1); i++) {
        mapyx(e, i) = path;
    }
}

bool valid_stop_coord(uint8_t y, uint8_t x, map_t *m) {
    char ch;
    uint8_t i, j;
    for (uint8_t r = 0; r < 2; r++) {
        for (uint8_t c = 0; c < 2; c++) {
            ch = mapyx((r + y), (c + x));
            if (ch == path || ch == mtn || ch == water ||
                ch == pMart || ch == pCntr) {
                    return false;
            }
        }   
    }
    
    for (j = y; j <= y + 1; j++) {
        if (mapyx(j, x - 1) == path) {
            return true;
        }
        if (mapyx(j, x + 2) == path) {
            return true;
        }
    }

    for (i = x; i <= x + 1; i++) {
        if (mapyx(y - 1, i) == path) {
            return true;
        }
        if (mapyx(y + 2, i) == path) {
            return true;
        }
    }
    return false;      
}

void generate_poke_center(map_t *m) {
    uint8_t x, y;

    do {
        x = (rand() % (X_MAX - 3)) + 1;
        y = (rand() % (Y_MAX - 3)) + 1;
    } while (!valid_stop_coord(y, x, m));

    for (uint8_t r = 0; r < 2; r++) {
        for (uint8_t c = 0; c < 2; c++) {
            mapyx((y + r), (x + c)) = pCntr;
        }
    }
}

void generate_pokemart(map_t *m) {
    uint8_t x, y;
    do {
        x = (rand() % (X_MAX - 3)) + 1;
        y = (rand() % (Y_MAX - 3)) + 1;
    } while (!valid_stop_coord(y, x, m));

    for (uint8_t r = 0; r < 2; r++) {
        for (uint8_t c = 0; c < 2; c++) {
            mapyx((y + r), (x + c)) = pMart;
        }
    }
}

void generate_trees(map_t *m, uint8_t min, uint8_t max) {
    uint8_t x, y;
    for (int i = 0; i < ((rand() % max) + min); i++) {
       do {
            x = (rand() % (X_MAX - 3)) + 1;
            y = (rand() % (Y_MAX - 3)) + 1;
       } while (!(mapyx(y, x) == sGrass || mapyx(y, x) == tGrass));
       mapyx(y, x) = tree;
    }
}

void generate_boulders(map_t *m, uint8_t min, uint8_t max) {
    uint8_t x, y;
    for (uint8_t i = 0; i < ((rand() % max) + min); i++) {
       do {
            x = (rand() % (X_MAX - 3)) + 1;
            y = (rand() % (Y_MAX - 3)) + 1;
       } while (!(mapyx(y, x) == sGrass || mapyx(y, x) == tGrass));
       mapyx(y, x) = bldr;
    }
}

// NOTE: before switching maps change pc coords and delete pc from clayer of previous map
void generate_pc (world_t* wrld) {
    wrld->pc.isDefeated = false;
    if (wrld->pc.y == 0 && wrld->pc.x == 0) {
        do {
            wrld->pc.y = (rand() % (Y_MAX - 2)) + 1;
            wrld->pc.x = (rand() % (X_MAX - 2)) + 1;
        } while(wrld->curr_map->ascii[wrld->pc.y][wrld->pc.x] != path);
    }

    wrld->curr_map->c_map[wrld->pc.y][wrld->pc.x] = &wrld->pc;
}

void generate_npc (map_t* m, NPC *npc) {
    npc->type = (character_t) ((rand() % 6) + h); // random type starting at h (first enum)
    npc->direction = (cardinal_t) (rand() % 4);
    npc->isDefeated = false;

    if (npc != NULL) {
        do {
            npc->y = (rand() % (Y_MAX - 2)) + 1;
            npc->x = (rand() % (X_MAX - 2)) + 1;
        } while(!(mapyx(npc->y, npc->x) == path
                    || mapyx(npc->y, npc->x) == sGrass
                    || mapyx(npc->y, npc->x) == tGrass
                    || clayer(npc->y, npc->x) != NULL));
                    // while the point is not path, sGrass, tGrass, or not a NULL (double negative)

        clayer(npc->y, npc->x) = npc;
    }
}

/*
    get the x and y coord in the 2D array of maps

    DONT HAVE TO pass n and w. get the previous s and e from the previous maps. example    getting south gate wrldyx(y, x - 1)->s;
*/
void generate_map(world_t *wrld){
    uint8_t d, i, j;

    uint8_t num_trainers = (rand() % (max_num_trainers / 2)) + (max_num_trainers / 2);

    wrldyx(wrld->curr_idx[dim_y], wrld->curr_idx[dim_x]) = 
                    (map_t*)malloc(sizeof(map_t) + ((num_trainers) * sizeof(turn_t)));

    curr_m = wrldyx(wrld->curr_idx[dim_y], wrld->curr_idx[dim_x]);
    curr_m->num_trainers = num_trainers;

    for (i = 0; i < Y_MAX; i++) {
        for (j = 0; j < X_MAX; j++) {
            curr_m->c_map[i][j] = NULL;
            curr_m->ascii[i][j] = UNKNOWN;
        }
    }

    for (i = 0; i < num_trainers; i++) {
        curr_m->turn_table[i].character = NULL;
        curr_m->turn_table[i].next_turn = 0;
        curr_m->turn_table[i].seq_num = i;
    }

    curr_m->n = 0;
    curr_m->s = 0;
    curr_m->e = 0;
    curr_m->w = 0;
    // init_map(curr_m, num_trainers);


    /*
        for 2d array pass the south and east gates for n and w, if it is the first on the pass 0.

        Instead of parameter n_gate, s_gate, etc get the x and y coordinates then determine based off location
        use this information for generate_road as well (which gates to connect)
    */

    // generate gates
    // North
    if (curr_y > 0) {
        if (wrldyx(curr_y - 1, curr_x) != NULL) {
            generate_gate(curr_m, N, wrldyx(curr_y - 1, curr_x)->s);
        }
        else {
            generate_gate(curr_m, N, 0);
        }
    }

    // West
    if (curr_x > 0) {
        if (wrldyx(curr_y, curr_x - 1) != NULL && wrldyx(curr_y, curr_x - 1)->e != 0) {
            generate_gate(curr_m, W, wrldyx(curr_y, curr_x - 1)->e);
        }
        else {
            generate_gate(curr_m, W, 0);
        }
    }

    // South
    if (curr_y < 400) {
        if (wrldyx(curr_y + 1, curr_x) != NULL && wrldyx(curr_y + 1, curr_x)->n != 0) {
            generate_gate(curr_m, S, wrldyx(curr_y + 1, curr_x)->n);
        }
        else {
            generate_gate(curr_m, S, 0);
        }
    }

    // East
    if (curr_x < 400) {
        if (wrldyx(curr_y, curr_x + 1) != NULL && wrldyx(curr_y, curr_x + 1)->w != 0) {
            generate_gate(curr_m, E, wrldyx(curr_y, curr_x + 1)->w);
        }
        else {
            generate_gate(curr_m, E, 0);
        }
    }

    // generate all walls
    generate_wall(curr_m, N);
    generate_wall(curr_m, S);
    generate_wall(curr_m, E);
    generate_wall(curr_m, W);
    
    // generate all terrain
    generate_terrain(curr_m);

    // generate roads connecting gates
    generate_path(curr_m);
    
    // generate PokeMart and Pokemon Center based on distance from (200, 200)
    d = abs(curr_x - 200) + abs(curr_y - 200);

    if (curr_x == 200 && curr_y == 200) {
        generate_poke_center(curr_m);
        generate_pokemart(curr_m);
    }
    else {
        if (d > 200) {
            d = 200;
        }

        uint8_t numerator = (((-45) * d) / 200) + 50;

        if (rand() % 100 < numerator) {
            generate_pokemart(curr_m);
        }

        if (rand() % 100 < numerator) {
            generate_poke_center(curr_m);
        }
    }

    generate_trees(curr_m, 50, 150);
    generate_boulders(curr_m, 5, 10);

    for (d = 0; d < (curr_m->num_trainers); d++) {
        curr_m->turn_table[d].character = (NPC*) malloc(sizeof(NPC));
        curr_m->turn_table[d].seq_num = d + 1;
        generate_npc(curr_m, static_cast<NPC*>(curr_m->turn_table[d].character));
    }

    generate_pc(wrld);

    // generate cost maps
    dijkstra_npc(wrld->curr_map, &wrld->hiker_cost, &wrld->pc, h);
    dijkstra_npc(wrld->curr_map, &wrld->rival_cost, &wrld->pc, r);
    dijkstra_npc(wrld->curr_map, &wrld->sentry_cost, &wrld->pc, s);
}