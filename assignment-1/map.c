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
        for (int j = 0; j < COL - 3; j++ ) {
            switch(map->terrain_type[i][j]) {
                case boulder:
                    printf("%c ", '%');
                    break;
                case tree:
                    printf("%c ", '^');
                    break;
                case road:
                    printf("%c ", '#');
                    break;
                case center:
                    printf("%c ", 'C');
                    break;
                case pokemart:
                    printf("%c ", 'M');
                    break;
                case grass:
                    printf("%c ", ':');
                    break;
                case clearing:
                    printf("%c ", '.');
                    break;
                default:
                    printf("%c ", '/');
                    break;
            }
        }
        printf("\n");
    }
}

void mapGen(struct map_key *map) {
    srand(time(NULL));
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL - 3; j++ ) {
            map->terrain_type[i][j] = 6;
        }
    }
}


int main() {
    struct map_key cur_map;

    mapGen(&cur_map);

    printMap(&cur_map);


    return 0;
}
