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
// #define COL 28
#define COL 56
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

class gameCharacter {
    public:
        int id;
        int x;
        int y;
        gameCharacter(int id, int x, int y) : id(id), x(x), y(y) {}
};

class map_key{
    public:
        int terrain_type[ROW][COL];
        int character_type[ROW][COL];
        gameCharacter* PM;
        gameCharacter* B;
        gameCharacter* P;
        gameCharacter* I;
        gameCharacter* C;
};

gameCharacter* newGameCharacter(int id, int x, int y) {
    gameCharacter* character = new gameCharacter(id, x, y);
    return character;
}

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
                        mvwaddch(map_win, i, j, 'I');
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
    std::string game_map[ROW] = {
        "% % % % % % % % % % % % % % % % % % % % % % % % % % % %",
        "% . . . . . . . . . . . . % % . . . . . . . . . . . . %",
        "% . % % % % . % % % % % . % % . % % % % % . % % % % . %",
        "% o % % % % . % % % % % . % % . % % % % % . % % % % o %",
        "% . % % % % . % % % % % . % % . % % % % % . % % % % . %",
        "% . . . . . . . . . . . . . . . . . . . . . . . . . . %",
        "% . % % % % . % % . % % % % % % % % . % % . % % % % . %",
        "% . % % % % . % % . % % % % % % % % . % % . % % % % . %",
        "% . . . . . . % % . . . . % % . . . . % % . . . . . . %",
        "% % % % % % . % % % % %   % %   % % % % % . % % % % % %",
        "          % . % % % % %   % %   % % % % % . %          ",
        "          % . % %                     % % . %          ",
        "          % . % %   % % % - - % % %   % % . %          ",
        "% % % % % % . % %   %             %   % % . % % % % % %",
        "            .       %             %       .            ",
        "% % % % % % . % %   %             %   % % . % % % % % %",
        "          % . % %   % % % % % % % %   % % . %          ",
        "          % . % %                     % % . %          ",
        "          % . % %   % % % % % % % %   % % . %          ",
        "% % % % % % . % %   % % % % % % % %   % % . % % % % % %",
        "% . . . . . . . . . . . . % % . . . . . . . . . . . . %",
        "% . % % % % . % % % % % . % % . % % % % % . % % % % . %",
        "% . % % % % . % % % % % . % % . % % % % % . % % % % . %",
        "% o . . % % . . . . . . .     . . . . . . . % % . . o %",
        "% % % . % % . % % . % % % % % % % % . % % . % % . % % %",
        "% % % . % % . % % . % % % % % % % % . % % . % % . % % %",
        "% . . . . . . % % . . . . % % . . . . % % . . . . . . %",
        "% . % % % % % % % % % % . % % . % % % % % % % % % % . %",
        "% . % % % % % % % % % % . % % . % % % % % % % % % % . %",
        "% . . . . . . . . . . . . . . . . . . . . . . . . . . %",
        "% % % % % % % % % % % % % % % % % % % % % % % % % % % %"
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
    map->character_type[23][26] = PACMAN;
    map->character_type[11][28] = BLINKY;
    map->character_type[14][23] = PINKY;
    map->character_type[14][27] = INKY;
    map->character_type[14][31] = CLYDE;
    map->PM = newGameCharacter(PACMAN, 23, 26);
    map->B = newGameCharacter(BLINKY, 11, 28);
    map->P = newGameCharacter(PINKY, 14, 23);
    map->I = newGameCharacter(INKY, 14, 27);
    map->C = newGameCharacter(CLYDE, 14, 31);
}

int moveCharacter(struct map_key *map, struct gameCharacter *character, int dx, int dy, 
WINDOW *map_win) {
    int newX = character->x + dx;
    int newY = character->y + dy;
    int terrain = map->terrain_type[newX][newY];
    if (terrain != WALL) {
        if (character->id == PACMAN) {
            map->character_type[character->x][character->y] = -1;
            character->x = newX;
            character->y = newY;
            map->character_type[character->x][character->y] = PACMAN;
            return 1;
        }
    }
    return 0;
}

void gameLoop() {
    WINDOW *map_win = newwin(ROW, COL, 1, 0);
    WINDOW *input_win = newwin(1, COL, ROW + 2, 0);
    char command[10];
    struct map_key *map = (struct map_key*) malloc(sizeof(struct map_key));
    mapGen(map);
    printMap(map, map_win);
    wrefresh(map_win);
    nodelay(input_win, true);
    while (1) {
        printMap(map, map_win);
        wmove(input_win, 0, 0);
        wrefresh(input_win);
        int ch = wgetch(input_win);
        while (ch == ERR) {
            ch = wgetch(input_win);
        }
        command[0] = ch;
        command[1] = '\0';

        if (strcmp(command, "w") == 0 || strcmp(command, "8") == 0) {
            //move up
            moveCharacter(map, map->PM, -1, 0, map_win);
        }
        else if (strcmp(command, "a") == 0 || strcmp(command, "4") == 0) {
            //left
            moveCharacter(map, map->PM, 0, -2, map_win);
        }
        else if (strcmp(command, "s") == 0 || strcmp(command, "2") == 0) {
            //down
            moveCharacter(map, map->PM, 1, 0, map_win);
        }
        else if (strcmp(command, "d") == 0 || strcmp(command, "6") == 0) {
            //right
            moveCharacter(map, map->PM, 0, 2, map_win);
        }
        else if (strcmp(command, "q") == 0) {
            //quit
            break;
        }

    }


    delete map->PM;
    delete map->B;
    delete map->P;
    delete map->I;
    delete map->C;
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


