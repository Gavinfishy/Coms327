#ifndef CHARACTER_QUEUE_H
#define CHARACTER_QUEUE_H

struct character {
    int x;
    int y;
    int move_cost;
};

struct queue_node_character {
    struct queue_node_character *next;
    struct character ch;
};

struct queue_node* createNode(struct character ch);
void enqueueCharacter(struct queue_node** head, struct character ch);
void dequeueCharacter(struct queue_node** head);
void queue_free(struct queue_node** head);

#endif