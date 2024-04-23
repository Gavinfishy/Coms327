//Author: Gavin D Fisher gdf73278
// https://gameinternals.com/understanding-pac-man-ghost-behavior
#include <cstdio>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <cstdint>
#include <math.h>
#include <limits.h>
#include <ncurses.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <climits>

#define GAME_ROW 36
#define ROW 31
#define COL 28
#define PACMAN 0 // pacman, yellow
#define BLINKY 1 // shadow, red
#define PINKY 2 // speedy, pink
#define INKY 3 // bashful, blue
#define CLYDE 4 // pokey, orange
#define WALL 0
#define FENCE 1
#define PELLET 2
#define BIG_PELLET 3
#define FRUIT 4
#define SPACE 5
#define LG 0 // left gate
#define RG 1 // right gate

class map_key{
    public:
        int terrain_type[ROW][COL];
        int character_type[ROW][COL];
        // gameCharacter PC;
        // gameCharacter NPC[10];
        // int terrain_exists[5];
        // int lg = 18; // left gate at 0,18
        // int rg = 18; // right gate at 28,18
        // struct minHeap* turnHeap;
};

void printMap(struct map_key *map, WINDOW *map_win) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++ ) {
            if (map->character_type[i][j] != -1) {
                switch(map->character_type[i][j]) {
                    case PACMAN:
                        // ^, <, >, v, O
                        mvwaddch(map_win, i, j, 'O');
                        break;
                    case BLINKY:
                        mvwaddch(map_win, i, j, 'B');
                        break;
                    case PINKY:
                        mvwaddch(map_win, i, j, 'P');
                        break;
                    case INKY:
                        mvwaddch(map_win, i, j, 'i');
                        break;
                    case CLYDE:
                        mvwaddch(map_win, i, j, 'C');
                        break;
                }
            }
            else {
                switch (map->terrain_type[i][j]) {
                    case WALL:
                        mvwaddch(map_win, i, j, '%');
                        break;
                    case FENCE:
                        mvwaddch(map_win, i, j, '-');
                        break;
                    case PELLET:
                        mvwaddch(map_win, i, j, '.');
                        break;
                    case BIG_PELLET:
                        mvwaddch(map_win, i, j, 'o');
                        break;
                    case SPACE:
                        mvwaddch(map_win, i, j, ' ');
                        break;
                    default:
                        mvwaddch(map_win, i, j, '/');
                        break;
                }
            }
        }
    }
    wrefresh(map_win);
}

void mapGen(struct map_key *map) {
    // for (int i = 0; i < ROW; i++) {
    //     for (int j = 0; j < COL; j++ ) {
    //         if (i == 0 || j == 0 || i == ROW-1 || j == COL-1) {
    //             map->terrain_type[i][j] = WALL;
    //         }
    //         else {
    //             map->terrain_type[i][j] = -1;
    //         }
    //         map->character_type[i][j] = -1;
    //     }
    // }
    std::string game_map[ROW] = {
        "%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
        "%............%%............%",
        "%.%%%%.%%%%%.%%.%%%%%.%%%%.%",
        "%o%%%%.%%%%%.%%.%%%%%.%%%%o%",
        "%.%%%%.%%%%%.%%.%%%%%.%%%%.%",
        "%..........................%",
        "%.%%%%.%%.%%%%%%%%.%%.%%%%.%",
        "%.%%%%.%%.%%%%%%%%.%%.%%%%.%",
        "%......%%....%%....%%......%",
        "%%%%%%.%%%%% %% %%%%%.%%%%%%",
        "     %.%%%%% %% %%%%%.%     ",
        "     %.%%          %%.%     ",
        "     %.%% %%%--%%% %%.%     ",
        "%%%%%%.%% %      % %%.%%%%%%",
        "      .   %      %   .      ",
        "%%%%%%.%% %      % %%.%%%%%%",
        "     %.%% %%%%%%%% %%.%     ",
        "     %.%%          %%.%     ",
        "     %.%% %%%%%%%% %%.%     ",
        "%%%%%%.%% %%%%%%%% %%.%%%%%%",
        "%............%%............%",
        "%.%%%%.%%%%%.%%.%%%%%.%%%%.%",
        "%.%%%%.%%%%%.%%.%%%%%.%%%%.%",
        "%o..%%.......  .......%%..o%",
        "%%%.%%.%%.%%%%%%%%.%%.%%.%%%",
        "%%%.%%.%%.%%%%%%%%.%%.%%.%%%",
        "%......%%....%%....%%......%",
        "%.%%%%%%%%%%.%%.%%%%%%%%%%.%",
        "%.%%%%%%%%%%.%%.%%%%%%%%%%.%",
        "%..........................%",
        "%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
    };

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            switch(game_map[i][j]) {
                case '%':
                    map->terrain_type[i][j] = WALL;
                    break;
                case '.':
                    map->terrain_type[i][j] = PELLET;
                    break;
                case 'o':
                    map->terrain_type[i][j] = BIG_PELLET;
                    break;
                case '-':
                    map->terrain_type[i][j] = FENCE;
                    break;
                default:
                    map->terrain_type[i][j] = SPACE;
                    break;
            }
        }
    }
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            map->character_type[i][j] = -1;
        }
    }
    map->character_type[23][13] = PACMAN;
}

void gameLoop() {
    WINDOW *map_win = newwin(ROW, COL, 1, 0);
    struct map_key *map = (struct map_key*) malloc(sizeof(struct map_key));
    mapGen(map);
    printMap(map, map_win);
    sleep(5);
    free(map);
    
}

int main(int argc, char* argv[]) {
    initscr();
    cbreak();
    raw();
    keypad(stdscr, true);
    noecho();
    srand(time(NULL));
    gameLoop();
    endwin();
    return 0;
}


