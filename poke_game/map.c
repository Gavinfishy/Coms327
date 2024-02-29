//Author: Gavin D Fisher gdf73278

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>

#define ROW 21
#define COL 80
#define empty (-1)
#define boulder 0
#define tree 1
#define grass 2
#define clearing 3
#define water 4
#define road 5
#define center 6
#define pokemart 7
#define player 8
#define hiker 9
#define rival 10
#define swimmer 11
#define pacers 12
#define wanderers 13
#define sentries 14
#define explorers 15
#define world_size 401
#define world_size_a (world_size/2)
#define NUM_CHARACTER_TYPES 8
#define NUM_TERRAIN_TYPES 8


/*
 * This struct keeps track of a character with their x and y coordinates in the character map
 */
//struct character {
//    int x;
//    int y;
//};

struct gameCharacter {
    int id;
    int x;
    int y;
    int type;
    int movementCost;
    int direction[2];
    int initialTerrain;
    char pacerAxis;
};

/*
 * This struct keeps track of the terrain map, character map, whether a terrain was created and gate locations
 * terrain_exists more specifically helps decide which npcs to spawn for example
 */
struct map_key{
    int terrain_type[ROW][COL];
    int character_type[ROW][COL];
    struct gameCharacter PC;
    int terrain_exists[5];
    int n,e,s,w;
};


/*
 * keeps track of the dijkstras cost map for each character type
 */
typedef struct cost_map_key {
    int map[ROW][COL];
}cost_map_key_t;
cost_map_key_t road_cost_map;
cost_map_key_t player_cost_map;
cost_map_key_t hiker_cost_map;
cost_map_key_t rival_cost_map;
cost_map_key_t swimmer_cost_map;
cost_map_key_t pacer_cost_map;
cost_map_key_t wanderer_cost_map;
cost_map_key_t sentry_cost_map;
cost_map_key_t explorer_cost_map;


/*
 * world struct which holds all maps
 */
struct map_key* world[world_size][world_size] = {NULL};

int currentX = 0;

int currentY = 0;

struct gameCharacter* newGameCharacter(int id, int x, int y, int type, int movementCost) {
    struct gameCharacter* character = (struct gameCharacter*) malloc(sizeof(struct gameCharacter));
    character->id = id;
    character->x = x;
    character->y = y;
    character->type = type;
    character->movementCost = movementCost;
    return character;
}

struct gameCharacter NPC[10];

struct adjacencyListNode {
    int dest;
    int weight;
    struct adjacencyListNode* next;
};

struct adjacencyList {
    struct adjacencyListNode* head;
};

struct graph {
    int v;
    struct adjacencyList* array;
};

struct adjacencyListNode* newAdjacencyListNode(int dest, int weight) {
    struct adjacencyListNode* newNode = (struct adjacencyListNode*) malloc(sizeof (struct adjacencyListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

struct graph* createGraph(int v) {
    struct graph* graph = (struct graph*) malloc(sizeof(struct graph));
    graph->v=v;
    graph->array = (struct adjacencyList*) malloc(v * sizeof(struct adjacencyList));
    for (int i = 0; i < v; i++) {
        graph->array[i].head = NULL;
    }
    return graph;
}

struct minHeap {
    int size;
    int capacity;
    int *position;
    struct minHeapNode **array;
};

struct minHeapNode {
    int v;
    int distance;
};

struct minHeapNode* newMinHeapNode(int v, int distance) {
    struct minHeapNode* minHeapNode = (struct minHeapNode*) malloc(sizeof (struct minHeapNode));
    minHeapNode->v=v;
    minHeapNode->distance=distance;
    return minHeapNode;
}


struct minHeapNode* newCharacterNode(int characterId, int movementCost) {
    struct minHeapNode* node = (struct minHeapNode*) malloc(sizeof(struct minHeapNode));
    node->v = characterId;
    node->distance = movementCost;
    return node;
}


struct minHeap* createMinHeap(int capacity) {
    struct minHeap* minHeap = (struct minHeap*) malloc(sizeof(struct minHeap));
    minHeap->position = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct minHeapNode**) malloc(capacity * sizeof (struct minHeapNode*));
    return minHeap;
}

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


/*
 * prints the terrain map and character map
 */
void printMap(struct map_key *map) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++ ) {
            if (map->character_type[i][j] != -1) {
                switch(map->character_type[i][j]) {
                    case player:
                        printf("%c", '@');
                        break;
                    case hiker:
                        printf("%c", 'h');
                        break;
                    case rival:
                        printf("%c", 'r');
                        break;
                    case pacers:
                        printf("%c", 'p');
                        break;
                    case wanderers:
                        printf("%c", 'w');
                        break;
                    case sentries:
                        printf("%c", 's');
                        break;
                    case explorers:
                        printf("%c", 'e');
                        break;
                    case swimmer:
                        printf("%c", 'm');
                        break;
                }
            }
            else {
                switch (map->terrain_type[i][j]) {

                    case boulder:
                        printf("%c", '%');
                        break;
                    case tree:
                        printf("%c", '^');
                        break;
                    case road:
                        printf("%c", '#');
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
                    case center:
                        printf("%c", 'C');
                        break;
                    case pokemart:
                        printf("%c", 'M');
                        break;
                    default:
                        printf("%c", '/');
                        break;
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}


/*
 * Sets gates to same as nearby maps, if the map is against an edge no gate is set, else pick a random gate location
 */
void setGates(struct map_key *map, int x, int y) {
    if (y > -world_size_a && world[x + world_size_a][y-1 + world_size_a] != NULL) {
        map->n = world[x + world_size_a][y-1 + world_size_a]->s;
    }
    else if (y == -world_size_a) {
        map->n = empty;
    }
    else {
        map->n = (rand() % 78 + 1);
    }
    if (y < world_size_a && world[x + world_size_a][y+1 + world_size_a] != NULL) {
        map->s = world[x + world_size_a][y+1 + world_size_a]->n;
    }
    else if (y == world_size_a) {
        map->s = empty;
    }
    else {
        map->s = (rand() % 78 + 1);
    }
    if (x < world_size_a && world[x+1 + world_size_a][y + world_size_a] != NULL) {
        map->e = world[x+1 + world_size_a][y + world_size_a]->w;
    }
    else if (x == world_size_a) {
        map->e = empty;
    }
    else {
        map->e = (rand() % 19 + 1);
    }
    if (x > -world_size_a && world[x-1 + world_size_a][y + world_size_a] != NULL) {
        map->w = world[x-1 + world_size_a][y + world_size_a]->e;
    }
    else if (x == -world_size_a) {
        map->w = empty;
    }
    else {
        map->w = (rand() % 19 + 1);
    }

    if (map->n != empty) {
        map->terrain_type[0][map->n] = road;
    }
    else {
        map->n = (rand() % 78 + 1);
    }
    if (map->s != empty) {
        map->terrain_type[ROW-1][map->s] = road;
    }
    else {
        map->s = (rand() % 78 + 1);
    }
    if (map->e != empty) {
        map->terrain_type[map->e][COL-1] = road;
    }
    else {
        map->e = (rand() % 19 + 1);
    }
    if (map->w != empty) {
        map->terrain_type[map->w][0] = road;
    }
    else {
        map->w = (rand() % 19 + 1);
    }
}


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


/*
 * Picks random seed locations between 15 and 35 in the 2d map array and expands them through a queue
 * 15-35 was chosen to add enough variety but not seem like random placements
 * Seed location is expanded with a queue until all spots are filled
 */
void expand_with_queue(struct map_key *map, struct queue *q) {
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};
    int seed_num = (rand() % 25) + 15;


    for (int i = 0; i < seed_num; i++) {
        int x = rand() % (ROW - 2) + 1;
        int y = rand() % (COL - 2) + 1;
        struct point start_seed = {x,y};
        int spawnProbability = rand() % 100;
        int terrain;
        if (spawnProbability < 15) {
            terrain = boulder; // 15% chance
        } else if (spawnProbability < 35) {
            terrain = tree; // 20% chance
        } else if (spawnProbability < 50) {
            terrain = water; // 15% chance
        } else if (spawnProbability < 75) {
            terrain = clearing; // 25% chance
        } else {
            terrain = grass; // 25% chance
        }
        map->terrain_type[start_seed.x][start_seed.y] = terrain;
        map->terrain_exists[terrain] = 1;
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


//void addEdge(struct graph* graph, int src, int dest, int weight) {
//    struct adjacencyListNode* newNode = newAdjacencyListNode(dest, weight);
//    newNode->next = graph->array[src].head;
//    graph->array[src].head = newNode;
//    newNode = newAdjacencyListNode(src, weight);
//    newNode->next = graph->array[dest].head;
//    graph->array[dest].head = newNode;
//}


void swapMinHeapNode(struct minHeapNode** a, struct minHeapNode** b) {
    struct minHeapNode* tmp = *a;
    *a = *b;
    *b = tmp;
}


void minHeapify(struct minHeap* minHeap, int index) {
    int smallest;
    int left;
    int right;
    smallest = index;
    left = 2 * index + 1;
    right = 2 * index + 2;
    if (left < minHeap->size && minHeap->array[left]->distance < minHeap->array[smallest]->distance ) {
        smallest = left;
    }
    if (right < minHeap->size && minHeap->array[right]->distance < minHeap->array[smallest]->distance ) {
        smallest = right;
    }
    if (smallest != index) {
        struct minHeapNode *smallestNode = minHeap->array[smallest];
        struct minHeapNode *indexNode = minHeap->array[index];
        minHeap->position[smallestNode->v] = index;
        minHeap->position[indexNode->v] = smallest;
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[index]);
        minHeapify(minHeap, smallest);
    }
}


int heapIsEmpty(struct minHeap* minHeap) {
    return minHeap->size == 0;
}


struct minHeapNode* extractMin(struct minHeap* minHeap) {
    if (heapIsEmpty(minHeap)) {
        return NULL;
    }
    struct minHeapNode* root = minHeap->array[0];
    struct minHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    minHeap->position[root->v] = minHeap->size-1;
    minHeap->position[lastNode->v] = 0;
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
}


void decreaseKey(struct minHeap* minHeap, int v, int distance) {
    int i = minHeap->position[v];
    minHeap->array[i]->distance = distance;
    while (i && minHeap->array[i]->distance < minHeap->array[(i - 1) / 2]->distance) {
        minHeap->position[minHeap->array[i]->v] = (i-1)/2;
        minHeap->position[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}


bool isInMinHeap(struct minHeap *minHeap, int v) {
    if (minHeap->position[v] < minHeap->size)
        return true;
    return false;
}


//void printArr(int dist[], int n) {
//    printf("Vertex Distance from Source\n");
//    for (int i = 0; i < n; ++i)
//        printf("%d \t\t %d\n", i, dist[i]);
//}


void addCharacterToHeap(struct minHeap* minHeap, int characterId, int movementCost) {
    struct minHeapNode* node = newCharacterNode(characterId, movementCost);
    minHeap->array[minHeap->size] = node;
    minHeap->position[characterId] = minHeap->size;
    minHeap->size++;
    decreaseKey(minHeap, characterId, movementCost);
}


void printHeap(struct minHeap* minHeap) {
    for (int i = 0; i < minHeap->size; i++) {
        printf("Character ID: %d, Movement Cost: %d\n", minHeap->array[i]->v, minHeap->array[i]->distance);
    }
    printf("\n");
}


void printCostMap(cost_map_key_t* cost_map) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (cost_map->map[i][j] == INT_MAX) {
                printf("   ");
            }
            else {
                printf(" %02d", cost_map->map[i][j] % 100);
            }
        }
        printf("\n");
    }
}


void dijkstra(cost_map_key_t* cost_map, int startX, int startY) {
    int V = ROW*COL;
    int distance[V];
    struct minHeap* minHeap = createMinHeap(V);
    for (int v = 0; v < V; ++v) {
        distance[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, distance[v]);
        minHeap->position[v] = v;
    }
    int playerIndex = startX*COL + startY;
    distance[playerIndex] = 0;
    decreaseKey(minHeap, playerIndex, distance[playerIndex]);
    minHeap->size = V;
    while (!heapIsEmpty(minHeap)) {
        struct minHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;
        int ux = u / COL;
        int uy = u % COL;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) {
                    continue;
                }
                int vx = ux + dx;
                int vy = uy + dy;
                if (vx >= 0 && vx < ROW && vy >= 0 && vy < COL) {
                    int v = vx * COL + vy;
                    if (isInMinHeap(minHeap, v) && distance[u] != INT_MAX) {
                        int newDist = distance[u] + cost_map->map[vx][vy];
                        if (newDist < 0 || newDist > INT_MAX) {
                            newDist = INT_MAX;
                        }
                        if (newDist < distance[v]) {
                            distance[v] = newDist;
                            decreaseKey(minHeap, v, distance[v]);
                        }
                    }
                }
            }
        }
        free(minHeapNode);
    }
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            int index = i * COL + j;
            cost_map->map[i][j] = distance[index];
        }
    }
//    printArr(distance, V);
    free(minHeap->array);
    free(minHeap->position);
    free(minHeap);
}


void terGen(struct map_key *map) {
    struct queue q;
    queue_init(&q);
    expand_with_queue(map, &q);
}


/*
 * Connects a road between gate locations. Parallel gates go to a random location inbetween then connect on that point
 */
void setPaths(struct map_key *map) {
    int horz = (rand() % 76 + 1);
    int vert = (rand() % 17 + 1);

    //horz
    for (int i = 1; i < horz; i++) {
        map->terrain_type[map->w][i] = road;
    }
    for (int k = COL - 2; k >= horz; k--) {
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
    for (int k = ROW - 2; k >= vert; k--) {
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


void setPathCost(struct map_key *map) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            switch(map->terrain_type[i][j]) {
                case boulder:
                    road_cost_map.map[i][j] = 100;
                    break;
                case tree:
                    road_cost_map.map[i][j] = 50;
                    break;
                case grass:
                    road_cost_map.map[i][j] = 20;
                    break;
                case clearing:
                    road_cost_map.map[i][j] = 5;
                    break;
                case water:
                    road_cost_map.map[i][j] = 80;
                    break;
                default:
                    road_cost_map.map[i][j] = INT_MAX;
                    break;
            }
        }
    }
}

int movementCostLookup[NUM_CHARACTER_TYPES][NUM_TERRAIN_TYPES] = {
        // 0-player, 1-hiker, 2-rival, 3-swimmer, 4-pacers, 5-wanderers, 6-sentries, 7-explorers
        // 0-boulder, 1-tree, 2-grass, 3-clearing, 4-water, 5-road, 6-center, 7-pokemart
        {INT_MAX, INT_MAX, 20, 10, INT_MAX, 10, 10, 10},
        {15, 15, 15, 10, INT_MAX, 10, 50, 50},
        {INT_MAX, INT_MAX, 20, 10, INT_MAX, 10, 50, 50},
        {INT_MAX, INT_MAX, INT_MAX, INT_MAX, 7, INT_MAX, INT_MAX},
        {INT_MAX, INT_MAX, 15, 10, INT_MAX, 10, 50, 50},
        {INT_MAX, INT_MAX, 20, 10, INT_MAX, 10, 50, 50},
        {INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX},
        {INT_MAX, INT_MAX, 20, 10, INT_MAX, 10, 50, 50}
};


int getMovementCostLookupIndex(int npcType) {
    switch(npcType) {
        case player:
            return 0;
        case hiker:
            return 1;
        case rival:
            return 2;
        case swimmer:
            return 3;
        case pacers:
            return 4;
        case wanderers:
            return 5;
        case sentries:
            return 6;
        case explorers:
            return 7;
    }
}

void setCostMaps(struct map_key *map) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            int terrainType = map->terrain_type[i][j];
            if (i == 0 || j == 0 || i == ROW-1 || j == COL-1) {
                player_cost_map.map[i][j] = INT_MAX;
                hiker_cost_map.map[i][j] = INT_MAX;
                rival_cost_map.map[i][j] = INT_MAX;
                swimmer_cost_map.map[i][j] = INT_MAX;
                pacer_cost_map.map[i][j] = INT_MAX;
                wanderer_cost_map.map[i][j] = INT_MAX;
                sentry_cost_map.map[i][j] = INT_MAX;
                explorer_cost_map.map[i][j] = INT_MAX;

            }
            else {
                player_cost_map.map[i][j] = movementCostLookup[getMovementCostLookupIndex(player)][terrainType];
                hiker_cost_map.map[i][j] = movementCostLookup[getMovementCostLookupIndex(hiker)][terrainType];
                rival_cost_map.map[i][j] = movementCostLookup[getMovementCostLookupIndex(rival)][terrainType];
                swimmer_cost_map.map[i][j] = movementCostLookup[getMovementCostLookupIndex(swimmer)][terrainType];
                pacer_cost_map.map[i][j] = movementCostLookup[getMovementCostLookupIndex(pacers)][terrainType];
                wanderer_cost_map.map[i][j] = movementCostLookup[getMovementCostLookupIndex(wanderers)][terrainType];
                sentry_cost_map.map[i][j] = movementCostLookup[getMovementCostLookupIndex(sentries)][terrainType];
                explorer_cost_map.map[i][j] = movementCostLookup[getMovementCostLookupIndex(explorers)][terrainType];
            }
        }
    }
}


/*
 * Chooses a random location near road to place each building. Chance of buildings not spawning
 */
void placeBuildings(struct map_key *map, int mapx, int mapy) {
    int house_num = center;
    bool placed = false;
    while (!placed) {
        int y = (rand() % 76 + 1);
        int x = (rand() % 17 + 1);

        if (map->terrain_type[x][y] != road && ((x > 1 && map->terrain_type[x-1][y] == road) ||
        (y > 1 && map->terrain_type[x][y-1] == road) || (x < COL - 2 && map->terrain_type[x+1][y] == road) ||
        (y < ROW - 2 && map->terrain_type[x][y+1] == road))) {
            int manhattan = (int) (abs(mapx) + abs(mapy));
            double spawnChance = -45*manhattan;
            spawnChance = ceil(fmax(5, ((spawnChance/200) + 50)));
            int c = rand() % 100 + 1;
            if (c <= spawnChance || (mapx == 0 && mapy == 0)) {
                map->terrain_type[x][y] = house_num;
            }
            if (house_num == pokemart) {
                placed = true;
            }
            house_num = pokemart;
        }
    }
}


/*
 * Randomly places each character type in their respective environment
 */
void placeCharacter(struct map_key *map, struct gameCharacter *character, int type) {
    bool placed = false;
    while (!placed) {
        int x = (rand() % (ROW - 2) + 1);
        int y = (rand() % (COL - 2) + 1);
        if (x > 0 && x < ROW - 1 && y > 0 && y < COL - 1) {
            if (type == player && map->terrain_type[x][y] == road) {
                map->character_type[x][y] = type;
                map->PC.x = x;
                map->PC.y = y;
                placed = true;
            } else if ((type == hiker) && map->terrain_type[x][y] != water && map->terrain_type[x][y] != road) {
                map->character_type[x][y] = type;
                character->x = x;
                character->y = y;
                placed = true;
            } else if ((type == rival || type == pacers || type == sentries || type == wanderers || type == explorers)
                       && map->terrain_type[x][y] != boulder && map->terrain_type[x][y] != tree
                       && map->terrain_type[x][y] != water && map->terrain_type[x][y] != road) {
                map->character_type[x][y] = type;
                character->x = x;
                character->y = y;
                placed = true;
                if (type == pacers) {
                    character->pacerAxis = (rand() % 2 == 0) ? 'x' : 'y';
                }
                //TODO
                if (type == wanderers) {
                    character->initialTerrain = map->terrain_type[character->x][character->y];
                }
            } else if (type == swimmer) {
                if (map->terrain_exists[water] == 1) {
                    if (map->terrain_type[x][y] == water) {
                        map->character_type[x][y] = type;
                        character->x = x;
                        character->y = y;
                        placed = true;
                    }
                } else {
                    break;
                }
            }
        }
    }
}


void placeCharacters(struct map_key *map) {
    placeCharacter(map, &map->PC, player);
    for (int i = 0; i < sizeof(NPC)/sizeof(NPC[0]); i++) {
        int type = (i % 7) + 9;
        NPC[i].type = type;
        placeCharacter(map, &NPC[i], type);
    }
}


/*
 * Responsible for moving the characters everytime the player moves.
 */
int moveCharacter(struct map_key *map, struct gameCharacter *character, int dx, int dy) {
    int newX = character->x + dx;
    int newY = character->y +dy;
    if (newX < 1 || newX > ROW - 2 || newY < 1 || newY > COL - 2) {
        printf("Cannot go out of bounds\n");
        return -1;
    }
    if (player_cost_map.map[newX][newY] == INT_MAX || map->character_type[newX][newY] != -1) {
        printf("Cannot move onto that terrain\n");
        return -1;
    }
    map->character_type[character->x][character->y] = -1;
    character->x += dx;
    character->y += dy;
    map->character_type[character->x][character->y] = player;
//    printMap(world[currentX + world_size_a][currentY + world_size_a]);
    return player_cost_map.map[newX][newY];
}


int moveNPC(struct gameCharacter* npc, cost_map_key_t* cost_map, struct map_key *map, int npcType) {
    int minCost = INT_MAX;
    int minDx = 0;
    int minDy = 0;
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        switch(npcType) {
            case pacers:
                if (npc->pacerAxis == 'x') {
                    dx = npc->direction[0];
                    dy = 0;
                } else {
                    dx = 0;
                    dy = npc->direction[1];
                }
                break;
            case wanderers:
//                if (map->terrain_type[npc->x + dx][npc->y + dy] != npc->initialTerrain) {
//                    continue;
//                }
                dx = npc->direction[0];
                dy = npc->direction[1];
                break;
            case explorers:
                dx = npc->direction[0];
                dy = npc->direction[1];
                break;
            case sentries:
                return INT_MAX;
            default:
                dx = directions[i][0];
                dy = directions[i][1];
                break;

        }
        if (dx == 0 && dy == 0) {
            continue;
        }
        if ((map->terrain_type[npc->x + dx][npc->y + dy] != npc->initialTerrain) && (npcType == wanderers)) {
            continue;
        }
        int newX = npc->x + dx;
        int newY = npc->y + dy;
        if (newX >= 1 && newX < ROW - 1 && newY >= 1 && newY < COL - 1 &&
            cost_map->map[newX][newY] < minCost && map->character_type[newX][newY] == -1) {
            minCost = cost_map->map[newX][newY];
            minDx = dx;
            minDy = dy;
        }
    }
    if (minCost == INT_MAX && (npcType == pacers || npcType == explorers || npcType == wanderers)) {
        int newDirectionIndex = rand() % 4;
        npc->direction[0] = directions[newDirectionIndex][0];
        npc->direction[1] = directions[newDirectionIndex][1];
    }
    int terrainType = map->terrain_type[npc->x + minDx][npc->y + minDy];
    int cost = movementCostLookup[getMovementCostLookupIndex(npcType)][terrainType];
    map->character_type[npc->x][npc->y] = -1;
    npc->x += minDx;
    npc->y += minDy;
    map->character_type[npc->x][npc->y] = npcType;
    return cost;
}


void mapGen(struct map_key *map, int x, int y) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++ ) {
            if (i == 0 || j == 0 || i == ROW-1 || j == COL-1) {
                map->terrain_type[i][j] = boulder;
            }
            else {
                map->terrain_type[i][j] = empty;
            }
            map->character_type[i][j] = empty;
        }
    }
    // Gates
    setGates(map, x, y);
    // Terrain
    terGen(map);
    // Path cost map
    setPathCost(map);
    // Paths
    setPaths(map);
    // Buildings
    placeBuildings(map, x, y);
    // Character cost Maps
    setCostMaps(map);
    // PC and NPCs
    placeCharacters(map);
    // Dijkstra cost map
    dijkstra(&hiker_cost_map, map->PC.x, map->PC.y);
    dijkstra(&rival_cost_map, map->PC.x, map->PC.y);
    dijkstra(&swimmer_cost_map, map->PC.x, map->PC.y);

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
        mapGen(world[newX + world_size_a][newY + world_size_a], newX, newY);
    }
    else {
        setCostMaps(world[newX + world_size_a][newY + world_size_a]);
        dijkstra(&hiker_cost_map, world[newX + world_size_a][newY + world_size_a]->PC.x,
                 world[newX + world_size_a][newY + world_size_a]->PC.y);
        dijkstra(&rival_cost_map, world[newX + world_size_a][newY + world_size_a]->PC.x,
                 world[newX + world_size_a][newY + world_size_a]->PC.y);
        dijkstra(&swimmer_cost_map, world[newX + world_size_a][newY + world_size_a]->PC.x,
                 world[newX + world_size_a][newY + world_size_a]->PC.y);

    }
    currentX = newX;
    currentY = newY;
    printMap(world[currentX + world_size_a][currentY + world_size_a]);
    printf("Current coordinates: (%d, %d)\n", currentX, currentY);
//    printCostMap(&hiker_cost_map);
//    printCostMap(&rival_cost_map);
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
    else {
        setCostMaps(world[x + world_size_a][y + world_size_a]);
        dijkstra(&rival_cost_map, world[x + world_size_a][y + world_size_a]->PC.x,
                 world[x + world_size_a][y + world_size_a]->PC.y);
        dijkstra(&hiker_cost_map, world[x + world_size_a][y + world_size_a]->PC.x,
                 world[x + world_size_a][y + world_size_a]->PC.y);
        dijkstra(&swimmer_cost_map, world[x + world_size_a][y + world_size_a]->PC.x,
                 world[x + world_size_a][y + world_size_a]->PC.y);
    }
    currentX = x;
    currentY = y;
    printMap(world[currentX + world_size_a][currentY + world_size_a]);
    printf("Current coordinates: (%d, %d)\n", x, y);
//    printCostMap(&hiker_cost_map);
//    printCostMap(&rival_cost_map);

}


/*
 *
 */
void gameLoop() {
    char command[10];
    int x;
    int y;
    if (world[currentX + world_size_a][currentY + world_size_a] == NULL) {
        world[currentX + world_size_a][currentY + world_size_a] = malloc(sizeof(struct map_key));
    }
    mapGen(world[currentX + world_size_a][currentY + world_size_a], currentX, currentY);
//    printMap(world[currentX + world_size_a][currentY + world_size_a]);
    //TODO decide how many NPC's are needed
    struct minHeap* turnHeap = createMinHeap(sizeof(NPC) + 1);
    struct gameCharacter* pc = newGameCharacter(-1, world[currentX + world_size_a][currentY + world_size_a]->PC.x,
            world[currentX + world_size_a][currentY + world_size_a]->PC.y, 0, 0);
    addCharacterToHeap(turnHeap, pc->id, 0);
    //TODO decide num npc
    for (int i = 0; i < 10; i++) {
        struct gameCharacter* npc = newGameCharacter(i, NPC[i].x, NPC[i].y, 1, 0);
        addCharacterToHeap(turnHeap, npc->id, 0);
    }
    while (1) {
        struct minHeapNode* minHeapNode = extractMin(turnHeap);
        int characterId = minHeapNode->v;
//        printCostMap(&hiker_cost_map);
        if (characterId == -1) {
            printMap(world[currentX + world_size_a][currentY + world_size_a]);
            printf("Enter command:\n");
            scanf("%s", command);
            if (strcmp(command, "k") == 0) {
                //move one up
                int cost = moveCharacter(world[currentX + world_size_a][currentY + world_size_a],
                                         &world[currentX + world_size_a][currentY + world_size_a]->PC, -1, 0);
                if (cost != -1) {
                    minHeapNode->distance += cost;
                }
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
//                printHeap(turnHeap);
            }
            else if (strcmp(command, "l") == 0) {
                //move one right
                int cost = moveCharacter(world[currentX + world_size_a][currentY + world_size_a],
                                         &world[currentX + world_size_a][currentY + world_size_a]->PC, 0, 1);
                if (cost != -1) {
                    minHeapNode->distance += cost;
                }
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
//                printHeap(turnHeap);
            }
            else if (strcmp(command, "j") == 0) {
                //move down one
                int cost = moveCharacter(world[currentX + world_size_a][currentY + world_size_a],
                                         &world[currentX + world_size_a][currentY + world_size_a]->PC, 1, 0);
                if (cost != -1) {
                    minHeapNode->distance += cost;
                }
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
//                printHeap(turnHeap);
            }
            else if (strcmp(command, "h") == 0) {
                //move left one
                int cost = moveCharacter(world[currentX + world_size_a][currentY + world_size_a],
                                         &world[currentX + world_size_a][currentY + world_size_a]->PC, 0, -1);
                if (cost != -1) {
                    minHeapNode->distance += cost;
                }
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
//                printHeap(turnHeap);
            }
            else if (strcmp(command, "y") == 0) {
                //move up left
                int cost = moveCharacter(world[currentX + world_size_a][currentY + world_size_a],
                                         &world[currentX + world_size_a][currentY + world_size_a]->PC, -1, -1);
                if (cost != -1) {
                    minHeapNode->distance += cost;
                }
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
            }
            else if (strcmp(command, "u") == 0) {
                //move up right
                int cost = moveCharacter(world[currentX + world_size_a][currentY + world_size_a],
                                         &world[currentX + world_size_a][currentY + world_size_a]->PC, -1, 1);
                if (cost != -1) {
                    minHeapNode->distance += cost;
                }
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
            }
            else if (strcmp(command, "n") == 0) {
                //move down right
                int cost = moveCharacter(world[currentX + world_size_a][currentY + world_size_a],
                                         &world[currentX + world_size_a][currentY + world_size_a]->PC, 1, 1);
                if (cost != -1) {
                    minHeapNode->distance += cost;
                }
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
            }
            else if (strcmp(command, "b") == 0) {
                //move down left
                int cost = moveCharacter(world[currentX + world_size_a][currentY + world_size_a],
                                         &world[currentX + world_size_a][currentY + world_size_a]->PC, 1, -1);
                if (cost != -1) {
                    minHeapNode->distance += cost;
                }
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
            }
            else if (strcmp(command, ">") == 0) {
                printf("Invalid command.\n");
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
            }
            else if (strcmp(command, "5") == 0 || strcmp(command, " ") == 0 || strcmp(command, ".") == 0 ) {
                printf("Invalid command.\n");
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
            }
            else if (strcmp(command, "t") == 0) {
                printf("Invalid command.\n");
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
            }
            else if (strcmp(command, "0") == 0) {
                move_maps(0, -1);
            }
            else if (strcmp(command, "9") == 0) {
                move_maps(0, 1);
            }
            else if (strcmp(command, "8") == 0) {
                move_maps(1, 0);
            }
            else if (strcmp(command, "7") == 0) {
                move_maps(-1, 0);
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
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
            }
            setCostMaps(world[currentX + world_size_a][currentY + world_size_a]);
            dijkstra(&hiker_cost_map, world[currentX + world_size_a][currentY + world_size_a]->PC.x, world[currentX + world_size_a][currentY + world_size_a]->PC.y);
            dijkstra(&rival_cost_map, world[currentX + world_size_a][currentY + world_size_a]->PC.x, world[currentX + world_size_a][currentY + world_size_a]->PC.y);
            dijkstra(&swimmer_cost_map, world[currentX + world_size_a][currentY + world_size_a]->PC.x, world[currentX + world_size_a][currentY + world_size_a]->PC.y);
            dijkstra(&pacer_cost_map, world[currentX + world_size_a][currentY + world_size_a]->PC.x, world[currentX + world_size_a][currentY + world_size_a]->PC.y);
            dijkstra(&wanderer_cost_map, world[currentX + world_size_a][currentY + world_size_a]->PC.x, world[currentX + world_size_a][currentY + world_size_a]->PC.y);
            dijkstra(&sentry_cost_map, world[currentX + world_size_a][currentY + world_size_a]->PC.x, world[currentX + world_size_a][currentY + world_size_a]->PC.y);
            dijkstra(&explorer_cost_map, world[currentX + world_size_a][currentY + world_size_a]->PC.x, world[currentX + world_size_a][currentY + world_size_a]->PC.y);

        }
        else {
            struct gameCharacter* npc = &NPC[characterId];
            cost_map_key_t* cost_map;
            int npcType;
            switch (npc->type) {
                case hiker:
                    cost_map = &hiker_cost_map;
                    npcType = hiker;
                    break;
                case rival:
                    cost_map = &rival_cost_map;
                    npcType = rival;
                    break;
                case swimmer:
                    cost_map = &swimmer_cost_map;
                    npcType = swimmer;
                    break;
                case pacers:
                    cost_map = &pacer_cost_map;
                    npcType = pacers;
                    break;
                case wanderers:
                    cost_map = &wanderer_cost_map;
                    npcType = wanderers;
                    break;
                case sentries:
                    cost_map = &sentry_cost_map;
                    npcType = sentries;
                    break;
                case explorers:
                    cost_map = &explorer_cost_map;
                    npcType = explorers;
                    break;
            }
            int cost = moveNPC(npc, cost_map, world[currentX + world_size_a][currentY + world_size_a], npcType);
            if (cost != -1) {
                minHeapNode->distance += cost;
                addCharacterToHeap(turnHeap, characterId, minHeapNode->distance);
            }
//            printHeap(turnHeap);
            printMap(world[currentX + world_size_a][currentY + world_size_a]);
//            usleep(250000);
        }
    }
}


int main() {
    srand(time(NULL));
    gameLoop();
    return 0;
}
