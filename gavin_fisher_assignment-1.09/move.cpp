/****************************************
 * Adhere to statement in COPYWRITE.txt *
****************************************/

#include "map.h"
#include "characters.h"

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <typeinfo>

#define SKIP 'S'

#define npc_y npc->y
#define npc_x npc->x

#define pc_y wrld->pc.y
#define pc_x wrld->pc.x

#define costyx(y, x) (*cost_map)[y][x]

#define validMove(c, y, x)                                      \
    ((0 < y && y < 20) &&                                       \
     (0 < x && x < 79) &&                                       \
     (MV_COST(c, wrld->curr_map->ascii[y][x]) != INF) &&        \
     (wrld->curr_map->c_map[y][x] == NULL))      

#define playerValidMove(y, x)                                   \
    ((0 <= y && y < 21) &&                                      \
     (0 <= x && x < 80) &&                                      \
     (MV_COST(player, wrld->curr_map->ascii[y][x]) != INF) &&   \
     (wrld->curr_map->c_map[y][x] == NULL))

#define validWrldMove(y, x)                                     \
    ((0 <= y && y <= 400) &&                                     \
     (0 <= x && x <= 400))

#define isGate(y, x)                                            \
    ((y == 0 || y == 20) || (x == 0 || x == 79))

#define inAttackRange(c, y, x)                                  \
    ((c == r ||                                                 \
        c == h ||                                               \
        c == s) &&                                              \
        wrld->curr_map->c_map[y][x] == &wrld->pc)      

#define notAShop(y, x)                                          \
    (wrld->curr_map->ascii[y][x] != pMart &&                    \
    wrld->curr_map->ascii[y][x] != pCntr)

static int32_t turn_cmp(const void *key, const void *with) {
    int32_t cmp = ((turn_t *) key)->next_turn - ((turn_t *) with)->next_turn;

    if (cmp == 0) {
        return ((turn_t *) key)->seq_num - ((turn_t *) with)->seq_num;
    }
    return cmp;
}

char move_player(world_t *wrld, heap_t *turn_heap) {
    uint8_t y, x, i;
    int16_t wrld_y = 0;
    int16_t wrld_x = 0;
    int input;
    bool is_valid;
    bool shouldPrint = true;

    do { 
        if (shouldPrint) {
            print_map(wrld);
        }
        shouldPrint = true;

        y = pc_y;
        x = pc_x;
        input = wgetch(main_w);
        print_top_msg(wrld, false, NULL);

        if (input == 'y' || input == '7')
        {
            y--;
            x--;
        }
        else if (input == 'k' || input == '8') {
            y--;
        }
        else if (input == 'u' || input == '9')
        {
            y--;
            x++;
        }
        
        else if (input == 'l' || input == '6')
        {
            x++;
        }
        else if (input == 'n' || input == '3')
        {
            y++;
            x++;
        }
        else if (input == 'j' || input == '2')
        {
            y++;
        }
        else if (input == 'b' || input == '1')
        {
            y++;
            x--;
        }
        else if (input == 'h' || input == '4')
        {
            x--;
        }
        else if (input == '>')
        {
            if (wrld->curr_map->ascii[y][x] == pMart) {
                input = poke_mart_window(wrld);
                continue;
            }
            else if (wrld->curr_map->ascii[y][x] == pCntr) {
                input = poke_center_window(wrld);
                continue;
            }
            else {
                print_top_msg(wrld, false, "Not at a shop.");
            }
        }
        else if (input == '5' || input == ' ' || input == '.')
        {
            break;
        }
        else if (input == 't')
        {
            input = trainers_window(wrld->curr_map, main_w);
            continue;
        }
        else if (input == 'f') { // command for flying
            const char *str = "Fly to (%d, %d)";
            char num_one[4];
            char num_two[4];
            char *num = NULL;

            char tmp_input;
            int16_t* dim;
            wrld_y = 0;
            wrld_x = 0;

            print_top_msg(wrld, true, str, wrld_x, wrld_y);
            wrefresh(top_msg_w);

            for (int k = 0; k < 2; k++) {
                if (k == 0){
                    dim = &wrld_x;
                    num = &num_one[0];
                }
                else {
                    dim = &wrld_y;
                    num = &num_two[0];
                }
                i = 0;

                while (1) {
                    tmp_input = wgetch(top_msg_w);

                    if (tmp_input == 27 ||
                        (tmp_input == '\n' && -200 <= *dim && *dim <= 200)) {
                        break;
                    }

                    if (!isdigit(tmp_input) && tmp_input != '-') {
                        continue;
                    }
                    num[i] = tmp_input;
                    *dim = static_cast<int16_t>(atoi(num));

                    print_top_msg(wrld, false, NULL, 0, 0);
                    print_top_msg(wrld, true, str, wrld_x, wrld_y);

                    if (-200 > *dim || *dim > 200) {
                        k--;
                        break;
                    }

                    if (i < 3) i++;
                    else continue;
                }
                if (tmp_input == 27) break;
                for (i = 0; i < 3; i++) {
                    num[i] = '\0';
                }
            }
            if (tmp_input == 27) continue;

            wrld_y += 200;
            wrld_x += 200;
            if (validWrldMove(wrld_y, wrld_x)) break;
        }


        if (input == 'Q' || input == 'q') {
            return 'Q';
        }
        
        is_valid = playerValidMove(y, x);

        if (wrld->curr_map->c_map[y][x] != NULL &&
                !wrld->curr_map->c_map[y][x]->isDefeated) {

            if (typeid(*wrld->curr_map->c_map[y][x]) == typeid(NPC)) {
                wrld->curr_map->c_map[y][x]->isDefeated =
                            trainer_battle(wrld,
                                static_cast<NPC*>(wrld->curr_map->c_map[y][x]));
                y = pc_y;
                x = pc_x;
                break;
            }
        }

        if ((pc_y != y || pc_x != x) && !is_valid) {
            print_top_msg(wrld, false, "You cannot go there.");
            shouldPrint = false;
        }
        
        
    } while(!is_valid);

    wrld->curr_map->c_map[pc_y][pc_x] = NULL;

    if (isGate(y, x) || input == 'f') { // return -2 so while loop knows to skip to next iteration
        
        if (input == 'f') {
            map_mover(wrld, N, 'f', wrld_y, wrld_x);
        }
        else if (y == 0) {
            map_mover(wrld, N, '\0', 0, 0);
        }
        else if (y == 20) {
            map_mover(wrld, S, '\0', 0, 0);
        }
        else if (x == 0) {
            map_mover(wrld, W, '\0', 0, 0);
        }
        else if (x == 79) {
            map_mover(wrld, E, '\0', 0, 0);
        }

        input = SKIP;

        heap_delete(turn_heap);

        heap_init(turn_heap, turn_cmp, NULL);
        
        wrld->pc_turn.hn = heap_insert(turn_heap, &wrld->pc_turn);
        for (i = 0; i < (int) curr_m->num_trainers; i++){
            curr_m->turn_table[i].hn = heap_insert(turn_heap,
                                                    &curr_m->turn_table[i]);
        }
    }
    else {
        pc_y = y;
        pc_x = x;

        if (curr_m->ascii[pc_y][pc_x] == tGrass && (rand() % 10) == 0) {

            Pokemon pokemon = Pokemon(wrld->curr_idx[dim_y], wrld->curr_idx[dim_x]);
            
            if (pokemon_encounter(wrld, pokemon)) {
                wrld->pc.add_pokemon(pokemon);
            }
        }

        wrld->pc_turn.next_turn += MV_COST(player, wrld->curr_map->ascii[y][x]);
    }

    wrld->curr_map->c_map[pc_y][pc_x] = &wrld->pc;

    // recalculate cost maps for trainers
    dijkstra_npc(wrld->curr_map, &wrld->hiker_cost, &wrld->pc, h);
    dijkstra_npc(wrld->curr_map, &wrld->rival_cost, &wrld->pc, r);

    return input;

}

void move_npc(world_t *wrld, turn_t *turn) {
    uint8_t x, y;
    int (*cost_map)[Y_MAX][X_MAX];
    uint8_t loop_count = 0;
    point_t path_cost;
    path_cost[dim_y] = 0;
    path_cost[dim_x] = 0;
    NPC* npc;

    if (typeid(*turn->character) == typeid(NPC)) {
        npc = static_cast<NPC*>(turn->character);
    }
    else {
        return;
    }

    y = npc_y;
    x = npc_x;

    switch (npc->type)
    {
        case h:
            cost_map = &wrld->hiker_cost;
            break;
        case r:
            cost_map = &wrld->rival_cost;
            break;
        default:
            break;
    }

    path_cost[dim_y] = 0;
    path_cost[dim_x] = 0;

    if (npc->type == e) {
        while(1) {
            switch (npc->direction) {
                case N:
                    path_cost[dim_y] = npc_y - 1;
                    path_cost[dim_x] = npc_x;
                    break;
                case S:
                    path_cost[dim_y] = npc_y + 1;
                    path_cost[dim_x] = npc_x;
                    break;
                case E:
                    path_cost[dim_y] = npc_y;
                    path_cost[dim_x] = npc_x + 1;
                    break;
                case W:
                    path_cost[dim_y] = npc_y;
                    path_cost[dim_x] = npc_x - 1;
                    break;
                default:
                    break;
            }
            if (validMove(e, path_cost[dim_y], path_cost[dim_x])) {
                break;
            }
            else if (loop_count > 3) {
                path_cost[dim_y] = 0;
                path_cost[dim_x] = 0;
                break;
            }

            npc->direction = (cardinal_t) (rand() % 4);
            loop_count++;
        }
    }
    else if (npc->type == w) {
        while(1) {
            switch (npc->direction) {
                case N:
                    path_cost[dim_y] = npc_y - 1;
                    path_cost[dim_x] = npc_x;
                    break;
                case S:
                    path_cost[dim_y] = npc_y + 1;
                    path_cost[dim_x] = npc_x;
                    break;
                case E:
                    path_cost[dim_y] = npc_y;
                    path_cost[dim_x] = npc_x + 1;
                    break;
                case W:
                    path_cost[dim_y] = npc_y;
                    path_cost[dim_x] = npc_x - 1;
                    break;
                default:
                    break;
            }
            if ((validMove(w, path_cost[dim_y], path_cost[dim_x]) &&
                wrld->curr_map->ascii[npc_y][npc_x] == 
                wrld->curr_map->ascii[path_cost[dim_y]][path_cost[dim_x]])) {
                break;
            }
            else if (loop_count > 3) {
                path_cost[dim_y] = 0;
                path_cost[dim_x] = 0;
                break;
            }
            npc->direction = (cardinal_t) (rand() % 4);
            loop_count++;
        }
    }
    else if (npc->type == p) {
        while(1) {
            switch (npc->direction) {
                case N:
                    path_cost[dim_y] = npc_y - 1;
                    path_cost[dim_x] = npc_x;
                    break;
                case S:
                    path_cost[dim_y] = npc_y + 1;
                    path_cost[dim_x] = npc_x;
                    break;
                case E:
                    path_cost[dim_y] = npc_y;
                    path_cost[dim_x] = npc_x + 1;
                    break;
                case W:
                    path_cost[dim_y] = npc_y;
                    path_cost[dim_x] = npc_x - 1;
                    break;
                default:
                    break;
            }

            if (validMove(p, path_cost[dim_y], path_cost[dim_x])) {
                break;
            }
            else if (loop_count > 3) {
                path_cost[dim_y] = 0;
                path_cost[dim_x] = 0;
                break;
            }
            
            npc->direction = (cardinal_t) ((npc->direction + 2) % 4);
            loop_count++;
        }
    }
    else {
        if (!npc->isDefeated) {
            if ((costyx(y - 1, x) < costyx(path_cost[dim_y], path_cost[dim_x])
                    && validMove(npc->type, y - 1, x)) ||
                    inAttackRange(npc->type, y - 1, x)) {
                path_cost[dim_y] = y - 1;
                path_cost[dim_x] = x;
            }

            if ((costyx(y + 1, x) < costyx(path_cost[dim_y], path_cost[dim_x])
                    && validMove(npc->type, y + 1, x)) ||
                    inAttackRange(npc->type, y + 1, x)) {
                path_cost[dim_y] = y + 1;
                path_cost[dim_x] = x;
            }

            if ((costyx(y, x - 1) < costyx(path_cost[dim_y], path_cost[dim_x])
                    && validMove(npc->type, y, x - 1)) ||
                    inAttackRange(npc->type, y, x - 1)) {
                path_cost[dim_y] = y;
                path_cost[dim_x] = x - 1;
            }

            if ((costyx(y, x + 1) < costyx(path_cost[dim_y], path_cost[dim_x])
                    && validMove(npc->type, y, x + 1)) ||
                    inAttackRange(npc->type, y, x + 1)) {
                path_cost[dim_y] = y;
                path_cost[dim_x] = x + 1;
            }

            // for sentry if the pc is out side a 100 range cost then dont move
            if (npc->type == s) {
                path_cost[dim_y] = 0;
                path_cost[dim_x] = 0;
            }
        }
        else {
            path_cost[dim_y] = npc_y;
            path_cost[dim_x] = npc_x;
            if (costyx(y - 1, x) >= costyx(path_cost[dim_y], path_cost[dim_x])
                    && validMove(npc->type, y - 1, x) &&
                    notAShop(y - 1, x)) {
                path_cost[dim_y] = y - 1;
                path_cost[dim_x] = x;
            }

            if (costyx(y + 1, x) >= costyx(path_cost[dim_y], path_cost[dim_x])
                    && validMove(npc->type, y + 1, x) &&
                    notAShop(y + 1, x)) {
                path_cost[dim_y] = y + 1;
                path_cost[dim_x] = x;
            }

            if (costyx(y, x - 1) >= costyx(path_cost[dim_y], path_cost[dim_x])
                    && validMove(npc->type, y, x - 1) &&
                    notAShop(y, x - 1)) {
                path_cost[dim_y] = y;
                path_cost[dim_x] = x - 1;
            }

            if (costyx(y, x + 1) >= costyx(path_cost[dim_y], path_cost[dim_x])
                    && validMove(npc->type, y, x + 1) &&
                    notAShop(y, x + 1)) {
                path_cost[dim_y] = y;
                path_cost[dim_x] = x + 1;
            }

            // for sentry if the pc is out side a 100 range cost then dont move
            if (npc->type == s && notAShop(npc_y, npc_x)) {
                path_cost[dim_y] = npc_y;
                path_cost[dim_x] = npc_x;
            }
        }
    }

    if (path_cost[dim_y] == 0 && path_cost[dim_x] == 0) {
        turn->next_turn++;
    }
    else if (path_cost[dim_y] == pc_y && path_cost[dim_x] == pc_x &&
                (npc->type == r ||
                npc->type == h)) {// attack turn

        npc->isDefeated = trainer_battle(wrld, npc);
    }
    else {
        wrld->curr_map->c_map[npc_y][npc_x] = NULL;
        npc_y = path_cost[dim_y];
        npc_x = path_cost[dim_x];
        wrld->curr_map->c_map[npc_y][npc_x] = npc;

        turn->next_turn += MV_COST(npc->type,
                                        wrld->curr_map->ascii[npc_y][npc_x]);

    }
}

void turn_move(world_t *wrld) {
    heap_t h;
    uint8_t i;
    turn_t *t;
    heap_init(&h, turn_cmp, NULL);

    wrld->pc_turn.hn = heap_insert(&h, &wrld->pc_turn);
    for (i = 0;
            i < wrld->curr_map->num_trainers; i++){
        wrld->curr_map->turn_table[i].hn = heap_insert(&h,
                                            &wrld->curr_map->turn_table[i]);
    }

    while ((t = (turn_t*) heap_remove_min(&h))) {
        char rtrn;

        if (t->character->type == player) {
            rtrn = move_player(wrld, &h);
            if (rtrn == 'Q') {
                return;
            }
            else if (rtrn == SKIP){
                continue;
            }
        }
        else {
            move_npc(wrld, t);
        }

        t->hn = NULL;
        t->hn = heap_insert(&h, t);
    }
}

void map_mover(world_t *wrld, cardinal_t dir, char input,
                    int16_t wrld_y, int16_t wrld_x) {

    if (input == 'f') {
        curr_y = wrld_y;
        curr_x = wrld_x;
        pc_x = 0;
        pc_y = 0;
    }
    else {
        switch (dir)
        {
        case N:
            curr_y--;
            pc_y = 19;
            break;

        case S:
            curr_y++;
            pc_y = 1;
            break;

        case E:
            curr_x++;
            pc_x = 1;
            break;
        
        case W:
            curr_x--;
            pc_x = 78;
            break;
        
        default:
            break;
        }
    }

    // if map has not been generated, generate
    if (wrldyx(curr_y, curr_x) == NULL) {
        wrldyx(curr_y, curr_x) = (map_t*)malloc(sizeof(map_t));
        generate_map(wrld);
    }
    else {
        curr_m = wrldyx(curr_y, curr_x);
        generate_pc(wrld);
        for (int i = 0; i < (int) curr_m->num_trainers; i++) {
            curr_m->turn_table[i].next_turn = 0;
        }
    }
    wrld->pc_turn.next_turn = 0;
}