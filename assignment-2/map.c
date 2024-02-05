//Author: Gavin D Fisher gdf73278

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#define ROW 21
#define COL 80
#define boulder 0
#define tree 1
#define grass 2
#define clearing 3
#define water 4
#define road 5
#define center 6
#define pokemart 7
#define world_size 3
#define world_size_a (world_size/2)


struct map_key{
    int cost_map[ROW][COL];
    int terrain_type[ROW][COL];
    int n,e,s,w;
};

struct map_key* world[world_size][world_size] = {NULL};

int currentX = 0;
int currentY = 0;

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
                case water:
                    printf("%c", '~');
                    break;
                default:
                    printf("%c", '/');
                    break;
            }
        }
        printf("\n");
    }
}

void setGates(struct map_key *map, int x, int y) {
    if (y > -world_size_a && world[x + world_size_a][y-1 + world_size_a] != NULL) {
        map->n = world[x + world_size_a][y-1 + world_size_a]->s;
    } else {
        map->n = (rand() % 78 + 1);
    }
    if (y < world_size_a && world[x + world_size_a][y+1 + world_size_a] != NULL) {
        map->s = world[x + world_size_a][y+1 + world_size_a]->n;
    } else {
        map->s = (rand() % 78 + 1);
    }
    if (x < world_size_a && world[x+1 + world_size_a][y + world_size_a] != NULL) {
        map->e = world[x+1 + world_size_a][y + world_size_a]->w;
    } else {
        map->e = (rand() % 19 + 1);
    }
    if (x > -world_size_a && world[x-1 + world_size_a][y + world_size_a] != NULL) {
        map->w = world[x-1 + world_size_a][y + world_size_a]->e;
    } else {
        map->w = (rand() % 19 + 1);
    }
    map->terrain_type[0][map->n] = road;
    map->terrain_type[ROW-1][map->s] = road;
    map->terrain_type[map->w][0] = road;
    map->terrain_type[map->e][COL-1] = road;

    printf("Gates for map at (%d, %d):\n", x, y);
    printf("N: %d\n", map->n);
//    printf("E: %d\n", map->e);
//    printf("S: %d\n", map->s);
//    printf("W: %d\n", map->w);
}

//void setGates(struct map_key *map, int x, int y) {
//    if (y > -world_size_a) {
//        if (world[x + world_size_a][y-1 + world_size_a] == NULL) {
//            world[x + world_size_a][y-1 + world_size_a] = malloc(sizeof(struct map_key));
//            if (world[x + world_size_a][y-1 + world_size_a]->s == 0) {
//                world[x + world_size_a][y-1 + world_size_a]->s = (rand() % 78 + 1);
//            }
//        }
//        map->n = world[x + world_size_a][y-1 + world_size_a]->s;
//    } else {
//        map->n = (rand() % 78 + 1);
//    }
//    if (y < world_size_a && world[x + world_size_a][y+1 + world_size_a] != NULL) {
//        map->s = world[x + world_size_a][y+1 + world_size_a]->n;
//    }
//    else {
//        map->s = (rand() % 78 + 1);
//    }
//    if (x < world_size_a && world[x+1 + world_size_a][y + world_size_a] != NULL) {
//        map->e = world[x+1 + world_size_a][y + world_size_a]->w;
//    } else {
//        map->e = (rand() % 19 + 1);
//    }
//    if (x > -world_size_a && world[x-1 + world_size_a][y + world_size_a] != NULL) {
//        map->w = world[x-1 + world_size_a][y + world_size_a]->e;
//    } else {
//        map->w = (rand() % 19 + 1);
//    }
//    map->terrain_type[0][map->n] = road;
//    map->terrain_type[ROW-1][map->s] = road;
//    map->terrain_type[map->w][0] = road;
//    map->terrain_type[map->e][COL-1] = road;
//
//    printf("Gates for map at (%d, %d):\n", x, y);
//    printf("N: %d\n", map->n);
//    printf("E: %d\n", map->e);
//    printf("S: %d\n", map->s);
//    printf("W: %d\n", map->w);
//    if (y > -world_size_a && world[x + world_size_a][y-1 + world_size_a] != NULL) {
//        printf("South gate of north map: %d\n", world[x + world_size_a][y-1 + world_size_a]->s);
//    }
//    if (y < world_size_a && world[x + world_size_a][y+1 + world_size_a] != NULL) {
//        printf("North gate of south map: %d\n", world[x + world_size_a][y+1 + world_size_a]->n);
//    }
//    if (x < world_size_a && world[x+1 + world_size_a][y + world_size_a] != NULL) {
//        printf("West gate of east map: %d\n", world[x+1 + world_size_a][y + world_size_a]->w);
//    }
//    if (x > -world_size_a && world[x-1 + world_size_a][y + world_size_a] != NULL) {
//        printf("East gate of west map: %d\n", world[x-1 + world_size_a][y + world_size_a]->e);
//    }
//}


struct point {
    int x;
    int y;
};


struct queue_node {
    struct queue_node *next;
    struct point pt;
};


struct queue {
    struct queue_node *head, *tail;
    int length;
};


int queue_init(struct queue *q){
    q->head = q->tail = NULL;
    q->length = 0;
    return 0;
}


int queue_enqueue(struct queue *q, struct point pt) {
    struct queue_node *tmp;
    if (!(tmp = malloc(sizeof (*tmp)))) {
        return 1;
    }
    tmp->pt = pt;
    tmp->next = NULL;
    if (!q->head) {
        q->head = q->tail = tmp;
    }
    else {
        q->tail->next = tmp;
        q->tail = tmp;
    }
    q->length++;
    return 0;
}


int queue_dequeue(struct queue *q, struct point *pt) {
    if (!q->head) {
        return 1;
    }
    struct queue_node *tmp = q->head;
    *pt = tmp->pt;
    q->head = q->head->next;
    free(tmp);
    tmp = NULL;
    if (!q->head) {
        q->tail = NULL;
    }
    free(tmp);
    tmp = NULL;
    q->length--;
    return 0;
}


int queue_length(struct queue *q) {
    return q->length;
}


int queue_is_empty(struct queue *q) {
    return !q->length;
}


void expand_with_queue(struct map_key *map, struct queue *q) {
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};
    int seed_num = (rand() % 25) + 15;
//    printf("%d\n", seed_num);


    for (int i = 0; i < seed_num; i++) {
        int x = rand() % (ROW - 2) + 1;
        int y = rand() % (COL - 2) + 1;
        struct point start_seed = {x,y};
        map->terrain_type[start_seed.x][start_seed.y] = rand() % 5;
        queue_enqueue(q,start_seed);
    }

    while (!queue_is_empty(q)) {
        struct point pt;
        queue_dequeue(q, &pt);

        int terrain = map->terrain_type[pt.x][pt.y];

        for (int i = 0; i < 4; i++) {
            int nx = pt.x + dx[i];
            int ny = pt.y + dy[i];

            if (nx >= 1 && nx < ROW-1 && ny >= 1 && ny < COL-1) {
                if (map->terrain_type[nx][ny] < 0) {
                    struct point np = {nx, ny};
                    map->terrain_type[nx][ny] = terrain;
                    queue_enqueue(q, np);
                }
            }
        }
    }
}


void terGen(struct map_key *map) {
    struct queue q;
    queue_init(&q);
    expand_with_queue(map, &q);
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
//        printf("%d %d\n", x,y);

        if (map->terrain_type[x][y] != road && ((x > 0 && map->terrain_type[x-1][y] == road) || (y > 0 && map->terrain_type[x][y-1] == road) || (x < COL - 1 && map->terrain_type[x+1][y] == road) || (y < ROW - 1 && map->terrain_type[x][y+1] == road))) {
            map->terrain_type[x][y] = house_num;
            if (house_num == pokemart) {
                placed = true;
            }
            house_num = pokemart;
        }
    }
}


void mapGen(struct map_key *map, int x, int y) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++ ) {
            if (i == 0 || j == 0 || i == ROW-1 || j == COL-1) {
                map->terrain_type[i][j] = boulder;
            }
            else {
                map->terrain_type[i][j] = -1;
            }
        }
    }
    // Gates
    setGates(map, x, y);
    // Terrain
    terGen(map);
    // Paths
    setPaths(map);
    // Buildings
    placeBuildings(map);
}


void move_maps(int dx, int dy) {
    int newX = currentX + dx;
    int newY = currentY + dy;
    if (newX < -world_size_a || newX > world_size_a || newY < -world_size_a || newY > world_size_a) {
        printf("Cannot go out of bounds\n");
        return;
    }
    if (world[newX + world_size_a][newY + world_size_a] == NULL) {
        world[newX + world_size_a][newY + world_size_a] = malloc(sizeof(struct map_key));
//        setGates(world[newX + world_size_a][newY + world_size_a], newX, newY);
        mapGen(world[newX + world_size_a][newY + world_size_a], newX, newY);
    }

    printMap(world[newX + world_size_a][newY + world_size_a]);
    printf("Current coordinates: (%d, %d)\n", newX, newY);
    currentX = newX;
    currentY = newY;
}


void fly(int x, int y) {
    if (x < -world_size_a || x > world_size_a || y < -world_size_a || y > world_size_a) {
        printf("Cannot go out of bounds\n");
        return;
    }
    if (world[x + world_size_a][y + world_size_a] == NULL) {
        world[x + world_size_a][y + world_size_a] = malloc(sizeof(struct map_key));
        mapGen(world[x + world_size_a][y + world_size_a], x, y);
    }
    currentX = x;
    currentY = y;
    printMap(world[currentX + world_size_a][currentY + world_size_a]);
    printf("Current coordinates: (%d, %d)\n", x, y);
}

void gameLoop() {
    char command[10];
    int x;
    int y;

    if (world[currentX + world_size_a][currentY + world_size_a] == NULL) {
        world[currentX + world_size_a][currentY + world_size_a] = malloc(sizeof(struct map_key));
    }
    mapGen(world[currentX + world_size_a][currentY + world_size_a], currentX, currentY);
    printMap(world[currentX + world_size_a][currentY + world_size_a]);
    while (1) {
        printf("Enter command:\n");
        scanf("%s", command);
        if (strcmp(command, "n") == 0) {
            move_maps(0,-1);
        }
        else if (strcmp(command, "s") == 0) {
            move_maps(0,1);
        }
        else if (strcmp(command, "e") == 0) {
            move_maps(1,0);
        }
        else if (strcmp(command, "w") == 0) {
            move_maps(-1,0);
        }
        else if (strcmp(command, "f") == 0) {
            scanf("%d %d", &x, &y);
            fly(x, y);
        }
        else if (strcmp(command, "q") == 0) {
            break;
        }
        else {
            printf("Invalid command.\n");
        }
    }
}



int main() {
    srand(time(NULL));
//    struct map_key cur_map;
    gameLoop();
//    mapGen(&cur_map);

//    printMap(&cur_map);


    return 0;
}
