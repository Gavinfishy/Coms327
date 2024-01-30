//Author: Gavin D Fisher gdf73278


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <math.h>


#define ROW 21
#define COL 80
#define boulder 0
#define tree 1
#define grass 2
#define clearing 3
#define road 4
#define center 5
#define pokemart 6


struct map_key{
    int cost_map[ROW][COL];
    int terrain_type[ROW][COL];
    int n,e,s,w;
};

struct Point {
    int x;
    int y;
};


void printMap(struct map_key *map) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++ ) {
            switch(map->terrain_type[i][j]) {
                case boulder:
                    printf("%c", '%');
                    break;
                case tree:
                    printf("%c", '^');
                    break;
                case road:
                    printf("%c", '#');
                    break;
                case center:
                    printf("%c", 'C');
                    break;
                case pokemart:
                    printf("%c", 'M');
                    break;
                case grass:
                    printf("%c", ':');
                    break;
                case clearing:
                    printf("%c", '.');
                    break;
                default:
                    printf("%c", '/');
                    break;
            }
        }
        printf("\n");
    }
}


void setGates(struct map_key *map) {
    map->n = (rand() % 78 + 1);
    map->s = (rand() % 78 + 1);
    map->e = (rand() % 19 + 1);
    map->w = (rand() % 19 + 1);
    map->terrain_type[0][map->n] = road;
    map->terrain_type[ROW-1][map->s] = road;
    map->terrain_type[map->w][0] = road;
    map->terrain_type[map->e][COL-1] = road;
}


void terGen(struct map_key *map) {
//    int dx[] = {-1, 0, 1, 0};
//    int dy[] = {0, 1, 0, -1};
//
//    int max_seeds = ROW * COL;
//    int (*seeds)[2] = malloc(max_seeds * sizeof(*seeds));
//
//    int seed_num = 0;
//    bool space_left = true;
//
//    for (int i = 0; i < 15; i++) {
//        int x = rand() % (ROW - 2) + 1;
//        int y = rand() % (COL - 2) + 1;
//        int terrain = rand() % 4;
//
//        seeds[seed_num][0] = x + 1;
//        seeds[seed_num][1] = y + 1;
//        seed_num++;
//        map->terrain_type[x][y] = terrain;
//    }
//
//    int radius = 1;
//
//    while (space_left) {
//        space_left = false;
//        for (int i = 0; i < seed_num; i++) {
//            int x = seeds[i][0];
//            int y = seeds[i][1];
//            int terrain = map->terrain_type[x][y];
//
//            for (int dx = -radius; dx <= radius; dx++) {
//                for (int dy = -radius; dy <= radius; dy++) {
//                    double distance = sqrt(dx*dx + dy*dy);
//                    if (distance > radius) {
//                        continue;  // Skip points outside the radius
//                    }
//
//                    int new_x = x + dx;
//                    int new_y = y + dy;
//
//
//                    if (new_x > 0 && new_x < ROW - 1 && new_y > 0 && new_y < COL - 1 && map->terrain_type[new_x][new_y] == 7) {
//                        map->terrain_type[new_x][new_y] = terrain;
//                        space_left = true;
//                    }
//                }
//            }
//        }
//    }
//
//    free(seeds);
}



void setPaths(struct map_key *map) {
    int horz = (rand() % 76 + 1);
    int vert = (rand() % 17 + 1);

    //horz
    for (int i = 1; i < horz; i++) {
        map->terrain_type[map->w][i] = road;
    }
    for (int k = COL - 1; k >= horz; k--) {
        map->terrain_type[map->e][k] = road;
    }
    if (map->w > map->e) {
        for (int j = map->w; j > map ->e; j--) {
            map->terrain_type[j][horz] = road;
        }
    }
    else {
        for (int j = map->w; j < map ->e; j++) {
            map->terrain_type[j][horz] = road;
        }
    }

    //vert
    for (int i = 1; i < vert; i++) {
        map->terrain_type[i][map->n] = road;
    }
    for (int k = ROW - 1; k >= vert; k--) {
        map->terrain_type[k][map->s] = road;
    }
    if (map->n > map->s) {
        for (int j = map->n; j > map->s; j--) {
            map->terrain_type[vert][j] = road;
        }
    }
    else {
        for (int j = map->n; j < map ->s; j++) {
            map->terrain_type[vert][j] = road;
        }
    }
}


void placeBuildings(struct map_key *map) {
    int house_num = center;
    bool placed = false;
    while (!placed) {
        int y = (rand() % 76 + 1);
        int x = (rand() % 17 + 1);
        printf("%d %d\n", x,y);

        if (map->terrain_type[x][y] != road && ((x > 0 && map->terrain_type[x-1][y] == road) || (y > 0 && map->terrain_type[x][y-1] == road) || (x < COL - 1 && map->terrain_type[x+1][y] == road) || (y < ROW - 1 && map->terrain_type[x][y+1] == road))) {
            map->terrain_type[x][y] = house_num;
            if (house_num == pokemart) {
                placed = true;
            }
            house_num = pokemart;

        }
    }
}


void mapGen(struct map_key *map) {
    srand(time(NULL));
    // Generic fill
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++ ) {
            if (i == 0 || j == 0 || i == ROW-1 || j == COL-1) {
                map->terrain_type[i][j] = boulder;
            }
            else {
                map->terrain_type[i][j] = 7;
            }
        }
    }
    // Gates
    setGates(map);
    // Terrain
//    terGen(map);
    // Paths
    setPaths(map);
    // Buildings
    placeBuildings(map);
}


int main() {
    struct map_key cur_map;

    mapGen(&cur_map);

    printMap(&cur_map);


    return 0;
}
