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
// Starting coordinates for game restart
#define START_PA_X 26
#define START_PA_Y 23
#define START_B_X 28
#define START_B_Y 11
#define START_P_X 23
#define START_I_X 27
#define START_C_X 31
#define START_BOX_Y 14
#define TOTAL_PELLET 244

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
        int lives = 3;
        int GameOver = false;
        int score;
        int curr_pellets;
        int num_moves;
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
    
    map->character_type[START_PA_Y][START_PA_X] = PACMAN;
    map->character_type[START_B_Y][START_B_X] = BLINKY;
    map->character_type[START_BOX_Y][START_P_X] = PINKY;
    map->character_type[START_BOX_Y][START_I_X] = INKY;
    map->character_type[START_BOX_Y][START_C_X] = CLYDE;
    map->PM = newGameCharacter(PACMAN, START_PA_Y, START_PA_X);
    map->B = newGameCharacter(BLINKY, START_B_Y, START_B_X);
    map->P = newGameCharacter(PINKY, START_BOX_Y, START_P_X);
    map->I = newGameCharacter(INKY, START_BOX_Y, START_I_X);
    map->C = newGameCharacter(CLYDE, START_BOX_Y, START_C_X);
    if (map->lives == 0) {
        map->lives = 3;
    }
    map->GameOver = false;
    map->curr_pellets = TOTAL_PELLET;
}

void nextLevel(struct map_key *map) {
    mapGen(map);
}

void deathRestart(struct map_key *map) {
    if (map->lives < 1) {
        map->GameOver = true;
    }
    else {
        map->lives -= 1;
        map->character_type[map->PM->x][map->PM->y] = -1;
        map->character_type[START_PA_Y][START_PA_X] = PACMAN;
        map->PM->x = START_PA_Y;
        map->PM->y = START_PA_X;
    }
}


int moveCharacter(struct map_key *map, struct gameCharacter *character, int dx, int dy, 
WINDOW *map_win) {
    int newX = character->x + dx;
    int newY = character->y + dy;
    int terrain = map->terrain_type[newX][newY];
    int npc_present = map->character_type[newX][newY];
    if (map->curr_pellets < 1) {
        nextLevel(map);
        return 1;
    }
    if (terrain != WALL && terrain != FENCE) {
        if (character->id == PACMAN) {
            if (npc_present != -1) {
                deathRestart(map);
            }
            else {
                map->character_type[character->x][character->y] = -1;
                character->x = newX;
                character->y = newY;
                map->character_type[character->x][character->y] = PACMAN;

                if (terrain == PELLET) {
                    map->terrain_type[newX][newY] = SPACE;
                    map->score += 10;
                    map->curr_pellets -= 1;
                }
                else if (terrain == BIG_PELLET) {
                    map->terrain_type[newX][newY] = SPACE;
                    map->score += 50;
                    map->curr_pellets -= 1;
                }
                return 1;
            }
        }
        else if (character->id == BLINKY) {

        }
        else if (character->id == PINKY) {

        }
        else if (character->id == INKY) {

        }
        else if (character->id = CLYDE) {

        }
    }
    return 0;
}

void gameLoop() {
    WINDOW *score_win = newwin(3, COL, 1, 0);
    WINDOW *map_win = newwin(ROW, COL, 4, 0);
    WINDOW *life_win = newwin(2, COL, ROW + 3, 0);
    WINDOW *input_win = newwin(1, COL, ROW + 5, 0);
    char command[10];
    struct map_key *map = (struct map_key*) malloc(sizeof(struct map_key));
    mapGen(map);
    printMap(map, map_win);
    wrefresh(map_win);
    nodelay(input_win, true);
    while (1) {
        if (map->GameOver == true) {
            break;
        }
        mvwprintw(score_win, 1, 6, "%d", map->score);
        wrefresh(score_win);
        wclrtoeol(life_win);
        wmove(life_win, 1, 1);
        for (int i = 0; i < map->lives; i++) {
            waddch(life_win, 'O');
            waddch(life_win, ' ');
        }
        wrefresh(life_win);
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


