//Author: Gavin D Fisher gdf73278

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define ROW 21
#define COL 80
#define boulder 0
#define tree 1
#define road 2
#define center 3
#define pokemart 4
#define grass 5
#define clearing 6



struct map_key{
    int cost_map[ROW][COL];
    int terrain_type[ROW][COL];
    int n,e,s,w;
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

void setPaths(struct map_key *map) {
    int horz = (rand() % 76 + 1);
    int vert = (rand() % 17 + 1);
    //horz
    for (int i = 1; i < horz; i++) {
        map->terrain_type[map->w][i] = road;
//        map->terrain_type[map->e][i + (78-horz)] = road;
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

    printf("%d %d\n", map->w, map->e);

    //horz
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
                map->terrain_type[i][j] = clearing;
            }
        }
    }
    // Gates
    setGates(map);
    // Paths
    setPaths(map);

    // etc

}


int main() {
    struct map_key cur_map;

    mapGen(&cur_map);

    printMap(&cur_map);


    return 0;
}
