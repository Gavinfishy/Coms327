/****************************************
 * Adhere to statement in COPYWRITE.txt *
****************************************/

#include "map.h"

int32_t move_cost[7][10] = { // 7 character types and 10 terrain types 
//  boulder,tree,path,mart,center,tall grass,short grass,mountain,water,gate
  { INF, INF, 10, 10, 10, 20, 10, INF, INF, 10  },
  { INF, INF, 10, 50, 50, 15, 10, 15 , INF, INF },
  { INF, INF, 10, 50, 50, 20, 10, INF, INF, INF },
  { INF, INF, 10, 50, 50, 20, 10, INF, INF, INF },
  { INF, INF, 10, 50, 50, 20, 10, INF, INF, INF },
  { INF, INF, 10, 50, 50, 20, 10, INF, INF, INF },
  { INF, INF, 10, 50, 50, 20, 10, INF, INF, INF },
};


static int32_t path_cmp(const void *key, const void *with) {
    return ((path_t *) key)->cost - ((path_t *) with)->cost;
}

void dijkstra_npc(map_t *m, int (*cost_map)[Y_MAX][X_MAX], PC *pc, character_t npc) {
    static path_t path[Y_MAX][X_MAX], *p;
    static uint32_t initialized = 0;
    heap_t h;
    uint32_t x, y, w;

    if (!initialized) {
        initialized = 1;
        for (y = 0; y < Y_MAX; y++) {
            for (x = 0; x < X_MAX; x++) {
                path[y][x].pos[dim_y] = y;
                path[y][x].pos[dim_x] = x;
            }
        }
    }
    
    for (y = 0; y < Y_MAX; y++) {
        for (x = 0; x < X_MAX; x++) {
            path[y][x].cost = INT_MAX;
        }
    }

    path[pc->y][pc->x].cost = 0;

    heap_init(&h, path_cmp, NULL);

    for (y = 1; y < Y_MAX - 1; y++) {
        for (x = 1; x < X_MAX - 1; x++) {
            path[y][x].hn = heap_insert(&h, &path[y][x]);
        }
    }

    while ((p = (path_t*) heap_remove_min(&h))) {
        p->hn = NULL;

        for (y = (p->pos[dim_y] - 1); y <= (uint32_t) (p->pos[dim_y] + 1); y++) {
            for (x = (uint32_t) (p->pos[dim_x] - 1); x <= (uint32_t) (p->pos[dim_x] + 1); x++) {
                if ((y == p->pos[dim_y] && x == p->pos[dim_x]) || (y >= Y_MAX
                    || y < 0 || x >= X_MAX || x < 0)) {
                    continue;
                }

                w = MV_COST(npc, mapyx(y, x));
                if (path[y][x].hn && path[y][x].cost > p->cost + w) {
                    path[y][x].cost = p->cost + w;
                    heap_decrease_key_no_replace(&h, path[y][x].hn);
                }
            }
        }
    }


    for (y = 0; y < Y_MAX; y++) {
        for (x = 0; x < X_MAX; x++) {
            (*cost_map)[y][x] = path[y][x].cost;
        }
    }
}

// static void dijkstra_path(map_t *m, point_t from, point_t to)
// {
//   static path_t path[Y_MAX][X_MAX], *p;
//   static uint32_t initialized = 0;
//   heap_t h;
//   uint32_t x, y;

//   if (!initialized) {
//     for (y = 0; y < Y_MAX; y++) {
//       for (x = 0; x < X_MAX; x++) {
//         path[y][x].pos[dim_y] = y;
//         path[y][x].pos[dim_x] = x;
//       }
//     }
//     initialized = 1;
//   }
  
//   for (y = 0; y < Y_MAX; y++) {
//     for (x = 0; x < X_MAX; x++) {
//       path[y][x].cost = INT_MAX;
//     }
//   }

//   path[from[dim_y]][from[dim_x]].cost = 0;

//   heap_init(&h, path_cmp, NULL);

//   for (y = 1; y < Y_MAX - 1; y++) {
//     for (x = 1; x < X_MAX - 1; x++) {
//       path[y][x].hn = heap_insert(&h, &path[y][x]);
//     }
//   }

//   while ((p = heap_remove_min(&h))) {
//     p->hn = NULL;

//     if ((p->pos[dim_y] == to.y) && p->pos[dim_x] == to.x) {
//       for (x = to.x, y = to.y;
//            (x != from[dim_x]) || (y != from[dim_y]);
//            p = &path[y][x], x = p->from[dim_x], y = p->from[dim_y]) {
//         mapyx(y, x) = '#';
//         heightxy(x, y) = 0;
//       }
//       heap_delete(&h);
//       return;
//     }

//     if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) &&
//         (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost >
//          ((p->cost + heightpair(p->pos)) *
//           edge_penalty(p->pos[dim_x], p->pos[dim_y] - 1)))) {
//       path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost =
//         ((p->cost + heightpair(p->pos)) *
//          edge_penalty(p->pos[dim_x], p->pos[dim_y] - 1));
//       path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
//       path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
//       heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
//                                            [p->pos[dim_x]    ].hn);
//     }
//     if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) &&
//         (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost >
//          ((p->cost + heightpair(p->pos)) *
//           edge_penalty(p->pos[dim_x] - 1, p->pos[dim_y])))) {
//       path[p->pos[dim_y]][p->pos[dim_x] - 1].cost =
//         ((p->cost + heightpair(p->pos)) *
//          edge_penalty(p->pos[dim_x] - 1, p->pos[dim_y]));
//       path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
//       path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
//       heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
//                                            [p->pos[dim_x] - 1].hn);
//     }
//     if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) &&
//         (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost >
//          ((p->cost + heightpair(p->pos)) *
//           edge_penalty(p->pos[dim_x] + 1, p->pos[dim_y])))) {
//       path[p->pos[dim_y]][p->pos[dim_x] + 1].cost =
//         ((p->cost + heightpair(p->pos)) *
//          edge_penalty(p->pos[dim_x] + 1, p->pos[dim_y]));
//       path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
//       path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
//       heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
//                                            [p->pos[dim_x] + 1].hn);
//     }
//     if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) &&
//         (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost >
//          ((p->cost + heightpair(p->pos)) *
//           edge_penalty(p->pos[dim_x], p->pos[dim_y] + 1)))) {
//       path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost =
//         ((p->cost + heightpair(p->pos)) *
//          edge_penalty(p->pos[dim_x], p->pos[dim_y] + 1));
//       path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
//       path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
//       heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
//                                            [p->pos[dim_x]    ].hn);
//     }
//   }
// }
