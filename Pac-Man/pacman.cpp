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
#define PACMAN_COLOR 10
#define BLINKY 1 // shadow, red
#define BLINKY_COLOR 11
#define PINKY 2 // speedy, pink
#define PINKY_COLOR 12
#define INKY 3 // bashful, blue
#define INKY_COLOR 13
#define CLYDE 4 // pokey, orange
#define CLYDE_COLOR 14
#define WALL 0
#define WALL_COLOR 15
#define VULNERABLE_COLOR 16
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
#define START_I_X 23
#define START_P_X 27
#define START_C_X 31
#define START_BOX_Y 14
#define TOTAL_PELLET 244

class gameCharacter {
    public:
        int id;
        int x;
        int y;
        int direction;
        bool vulnerable;
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
        bool vulnerable_mode;
        int vulnerable_count;
        bool P_INIT;
        bool I_INIT;
        bool C_INIT;
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
                        wattron(map_win, COLOR_PAIR(PACMAN_COLOR));
                        mvwaddch(map_win, i, j, 'O');
                        wattroff(map_win, COLOR_PAIR(PACMAN_COLOR));
                        break;
                    case BLINKY:
                        if (map->B->vulnerable) {
                            wattron(map_win, COLOR_PAIR(VULNERABLE_COLOR));
                            mvwaddch(map_win, i, j, 'B');
                            wattroff(map_win, COLOR_PAIR(VULNERABLE_COLOR));
                        }
                        else {
                            wattron(map_win, COLOR_PAIR(BLINKY_COLOR));
                            mvwaddch(map_win, i, j, 'B');
                            wattroff(map_win, COLOR_PAIR(BLINKY_COLOR));
                        }
                        break;
                    case PINKY:
                        if (map->P->vulnerable) {
                            wattron(map_win, COLOR_PAIR(VULNERABLE_COLOR));
                            mvwaddch(map_win, i, j, 'P');
                            wattroff(map_win, COLOR_PAIR(VULNERABLE_COLOR));
                        }
                        else {
                            wattron(map_win, COLOR_PAIR(PINKY_COLOR));
                            mvwaddch(map_win, i, j, 'P');
                            wattroff(map_win, COLOR_PAIR(PINKY_COLOR));
                        }
                        break;
                    case INKY:
                        if (map->I->vulnerable) {
                            wattron(map_win, COLOR_PAIR(VULNERABLE_COLOR));
                            mvwaddch(map_win, i, j, 'I');
                            wattroff(map_win, COLOR_PAIR(VULNERABLE_COLOR));
                        }
                        else {
                            wattron(map_win, COLOR_PAIR(INKY_COLOR));
                            mvwaddch(map_win, i, j, 'I');
                            wattroff(map_win, COLOR_PAIR(INKY_COLOR));
                        }
                        break;
                    case CLYDE:
                        if (map->C->vulnerable) {
                            wattron(map_win, COLOR_PAIR(VULNERABLE_COLOR));
                            mvwaddch(map_win, i, j, 'C');
                            wattroff(map_win, COLOR_PAIR(VULNERABLE_COLOR));
                        }
                        else {
                            wattron(map_win, COLOR_PAIR(CLYDE_COLOR));
                            mvwaddch(map_win, i, j, 'C');
                            wattroff(map_win, COLOR_PAIR(CLYDE_COLOR));
                        }
                        break;
                }
            }
            else {
                switch (map->terrain_type[i][j]) {
                    case WALL:
                        wattron(map_win, COLOR_PAIR(WALL_COLOR));
                        mvwaddch(map_win, i, j, '%');
                        wattroff(map_win, COLOR_PAIR(WALL_COLOR));
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
        "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
        "% . . . . . . . . . . . . % % . . . . . . . . . . . . %",
        "% . %%%%%%% . %%%%%%%%% . % % . %%%%%%%%% . %%%%%%% . %",
        "% o %     % . %       % . % % . %       % . %     % o %",
        "% . %%%%%%% . %%%%%%%%% . %%% . %%%%%%%%% . %%%%%%% . %",
        "% . . . . . . . . . . . . . . . . . . . . . . . . . . %",
        "% . %%%%%%% . %%% . %%%%%%%%%%%%%%% . %%% . %%%%%%% . %",
        "% . %%%%%%% . % % . %%%%%%% %%%%%%% . % % . %%%%%%% . %",
        "% . . . . . . % % . . . . % % . . . . % % . . . . . . %",
        "%%%%%%%%%%% . % %%%%%%%   % %   %%%%%%% % . %%%%%%%%%%%",
        "          % . % %%%%%%%   %%%   %%%%%%% % . %          ",
        "          % . % %                     % % . %          ",
        "          % . % %   %%%%%-----%%%%%   % % . %          ",
        "%%%%%%%%%%% . %%%   %             %   %%% . %%%%%%%%%%%",
        "            .       %             %       .            ",
        "%%%%%%%%%%% . %%%   %             %   %%% . %%%%%%%%%%%",
        "          % . % %   %%%%%%%%%%%%%%%   % % . %          ",
        "          % . % %                     % % . %          ",
        "          % . % %   %%%%%%%%%%%%%%%   % % . %          ",
        "%%%%%%%%%%% . %%%   %%%%%%% %%%%%%%   %%% . %%%%%%%%%%%",
        "% . . . . . . . . . . . . % % . . . . . . . . . . . . %",
        "% . %%%%%%% . %%%%%%%%% . % % . %%%%%%%%% . %%%%%%% . %",
        "% . %%%%% % . %%%%%%%%% . %%% . %%%%%%%%% . % %%%%% . %",
        "% o . . % % . . . . . . .     . . . . . . . % % . . o %",
        "%%%%% . % % . %%% . %%%%%%%%%%%%%%% . %%% . % % . %%%%%",
        "%%%%% . %%% . % % . %%%%%%% %%%%%%% . % % . %%% . %%%%%",
        "% . . . . . . % % . . . . % % . . . . % % . . . . . . %",
        "% . %%%%%%%%%%% %%%%%%% . % % . %%%%%%% %%%%%%%%%%% . %",
        "% . %%%%%%%%%%%%%%%%%%% . %%% . %%%%%%%%%%%%%%%%%%% . %",
        "% . . . . . . . . . . . . . . . . . . . . . . . . . . %",
        "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
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
    map->num_moves = 0;
    map->vulnerable_mode = false;
    map->vulnerable_count = 0;
    map->P_INIT = false;
    map->I_INIT = false;
    map->C_INIT = false;
    map->PM->vulnerable = false;
    map->B->vulnerable = false;
    map->P->vulnerable = false;
    map->I->vulnerable = false;
    map->C->vulnerable = false;
}

void vulnerableMode(struct map_key *map, bool vulnerable) {
    if (vulnerable) {
        map->B->vulnerable = true;
        map->P->vulnerable = true;
        map->I->vulnerable = true;
        map->C->vulnerable = true;
        map->vulnerable_mode = true;
        //turn on color
    }
    else {
        //turn off color
        map->B->vulnerable = false;
        map->P->vulnerable = false;
        map->I->vulnerable = false;
        map->C->vulnerable = false;
        map->vulnerable_mode = true;
        map->vulnerable_count = 0;
    }
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
        map->character_type[map->B->x][map->B->y] = -1;
        map->character_type[map->P->x][map->P->y] = -1;
        map->character_type[map->I->x][map->I->y] = -1;
        map->character_type[map->C->x][map->C->y] = -1;
        map->character_type[START_PA_Y][START_PA_X] = PACMAN;
        map->character_type[START_B_Y][START_B_X] = BLINKY;
        map->character_type[START_BOX_Y][START_P_X] = PINKY;
        map->character_type[START_BOX_Y][START_I_X] = INKY;
        map->character_type[START_BOX_Y][START_C_X] = CLYDE;
        delete map->PM;
        delete map->B;
        delete map->P;
        delete map->I;
        delete map->C;
        map->PM = newGameCharacter(PACMAN, START_PA_Y, START_PA_X);
        map->B = newGameCharacter(BLINKY, START_B_Y, START_B_X);
        map->P = newGameCharacter(PINKY, START_BOX_Y, START_P_X);
        map->I = newGameCharacter(INKY, START_BOX_Y, START_I_X);
        map->C = newGameCharacter(CLYDE, START_BOX_Y, START_C_X);
        map->num_moves = 0;
        map->P_INIT = false;
        map->I_INIT = false;
        map->C_INIT = false;        
    }
}


int moveCharacter(struct map_key *map, struct gameCharacter *character, int dx, int dy, 
WINDOW *map_win) {
    int newX = character->x + dx;
    int newY = character->y + dy;
    int terrain = map->terrain_type[newX][newY];
    int npc_present;
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -2}, {0, 2}};
    // for ghosts change in direction
    int gdx = directions[character->direction][0];
    int gdy = directions[character->direction][1];
    if (map->curr_pellets < 1) {
        nextLevel(map);
        return 1;
    }
    
    if (character->id == PACMAN) {
        if (terrain != WALL && terrain != FENCE) {
            npc_present = map->character_type[newX][newY];
            if (npc_present == BLINKY) {
                // deathRestart(map);
            }
            else {
                if (newY != COL - 2 && newY != 0) {
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
                        vulnerableMode(map, true);
                    }
                    return 1;
                }
                else if (newY == COL - 2) {
                    map->character_type[character->x][character->y] = -1;
                    character->x = 14;
                    character->y = 2;
                    map->character_type[character->x][character->y] = PACMAN;
                    return 1;
                }
                else if (newY == 0) {
                    map->character_type[character->x][character->y] = -1;
                    character->x = 14;
                    character->y = COL - 4;
                    map->character_type[character->x][character->y] = PACMAN;
                    return 1;
                }
                
            }
        }
    }
    else if (character->id == BLINKY) {
        if (map->terrain_type[character->x + gdx][character->y + gdy] == WALL || 
        map->terrain_type[character->x + gdx][character->y + gdy] == FENCE) {
            do {
                character->direction = rand() % 4;
                gdx = directions[character->direction][0];
                gdy = directions[character->direction][1];
            } while (map->terrain_type[character->x + gdx][character->y + gdy] == WALL ||
            map->terrain_type[character->x + gdx][character->y + gdy] == FENCE);
        }
        npc_present = map->character_type[character->x + gdx][character->y + gdy];
        if (npc_present == PACMAN) {
            deathRestart(map);
        }
        else {
            map->character_type[character->x][character->y] = -1;
            character->x += gdx;
            character->y += gdy;
            map->character_type[character->x][character->y] = BLINKY;
        }
        return 1;
    }
    else if (character->id == PINKY) {
        if (map->num_moves > 10 && !map->P_INIT) {
            map->P_INIT = true;
            map->character_type[character->x][character->y] = -1;
            character->x = START_B_Y;
            character->y = START_B_X;
            map->character_type[character->x][character->y] = PINKY;

        }
        if (map->P_INIT) {
            if (map->terrain_type[character->x + gdx][character->y + gdy] == WALL || 
            map->terrain_type[character->x + gdx][character->y + gdy] == FENCE) {
                do {
                    character->direction = rand() % 4;
                    gdx = directions[character->direction][0];
                    gdy = directions[character->direction][1];
                } while (map->terrain_type[character->x + gdx][character->y + gdy] == WALL ||
                map->terrain_type[character->x + gdx][character->y + gdy] == FENCE);
            }
            npc_present = map->character_type[character->x + gdx][character->y + gdy];
            if (npc_present == PACMAN) {
                deathRestart(map);
            }
            else {
                map->character_type[character->x][character->y] = -1;
                character->x += gdx;
                character->y += gdy;
                map->character_type[character->x][character->y] = PINKY;
            }
            return 1;
        }
    }
    else if (character->id == INKY) {
        if (map->num_moves > 20 && !map->I_INIT) {
            map->I_INIT = true;
            map->character_type[character->x][character->y] = -1;
            character->x = START_B_Y;
            character->y = START_B_X;
            map->character_type[character->x][character->y] = INKY;

        }
        if (map->I_INIT) {
            if (map->terrain_type[character->x + gdx][character->y + gdy] == WALL || 
            map->terrain_type[character->x + gdx][character->y + gdy] == FENCE) {
                do {
                    character->direction = rand() % 4;
                    gdx = directions[character->direction][0];
                    gdy = directions[character->direction][1];
                } while (map->terrain_type[character->x + gdx][character->y + gdy] == WALL ||
                map->terrain_type[character->x + gdx][character->y + gdy] == FENCE);
            }
            npc_present = map->character_type[character->x + gdx][character->y + gdy];
            if (npc_present == PACMAN) {
                deathRestart(map);
            }
            else {
                map->character_type[character->x][character->y] = -1;
                character->x += gdx;
                character->y += gdy;
                map->character_type[character->x][character->y] = INKY;
            }
            return 1;
        }

    }
    else if (character->id = CLYDE) {
        if (map->num_moves > 30 && !map->C_INIT) {
            map->C_INIT = true;
            map->character_type[character->x][character->y] = -1;
            character->x = START_B_Y;
            character->y = START_B_X;
            map->character_type[character->x][character->y] = CLYDE;

        }
        if (map->C_INIT) {
            if (map->terrain_type[character->x + gdx][character->y + gdy] == WALL || 
            map->terrain_type[character->x + gdx][character->y + gdy] == FENCE) {
                do {
                    character->direction = rand() % 4;
                    gdx = directions[character->direction][0];
                    gdy = directions[character->direction][1];
                } while (map->terrain_type[character->x + gdx][character->y + gdy] == WALL ||
                map->terrain_type[character->x + gdx][character->y + gdy] == FENCE);
            }
            npc_present = map->character_type[character->x + gdx][character->y + gdy];
            if (npc_present == PACMAN) {
                deathRestart(map);
            }
            else {
                map->character_type[character->x][character->y] = -1;
                character->x += gdx;
                character->y += gdy;
                map->character_type[character->x][character->y] = CLYDE;
            }
            return 1;
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
        if (map->vulnerable_mode && map->vulnerable_count <= 30) {
            map->vulnerable_count += 1;
        }
        else {
            vulnerableMode(map, false);
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
        map->num_moves += 1;
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
        moveCharacter(map, map->B, 0, 0, map_win);
        moveCharacter(map, map->P, 0, 0, map_win);
        moveCharacter(map, map->I, 0, 0, map_win);
        moveCharacter(map, map->C, 0, 0, map_win);
        

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
    start_color();
    keypad(stdscr, true);
    noecho();
    init_pair(PACMAN_COLOR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLINKY_COLOR, COLOR_RED, COLOR_BLACK);
    init_pair(PINKY_COLOR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(INKY_COLOR, COLOR_CYAN, COLOR_BLACK);
    init_pair(CLYDE_COLOR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(VULNERABLE_COLOR, COLOR_BLUE, COLOR_BLACK);
    init_pair(WALL_COLOR, COLOR_BLUE, COLOR_BLUE);

    srand(time(NULL));
    gameLoop();
    endwin();
    return 0;
}


