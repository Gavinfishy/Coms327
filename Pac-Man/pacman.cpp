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

#define ROW 36
#define COL 28
#define PACMAN 0 // pac, yellow
#define BLINKY 1 // shadow, red
#define PINKY 2 // speedy, pink
#define INKY 3 // bashful, blue
#define CLYDE 4 // pokey, orange
#define TLG 0
#define BLG 1
#define TRG 2
#define BRG 3

class map_key{
    public:
        int terrain_type[ROW][COL];
        int character_type[ROW][COL];
        gameCharacter PC;
        gameCharacter NPC[10];
        int terrain_exists[5];
        int n,e,s,w;
        struct minHeap* turnHeap;
};