#include <stdlib.h>
#include "character_queue.h"
struct queue_node_character* createNode(struct character ch) {
    struct queue_node_character* newNode = (struct queue_node*)malloc(sizeof(struct queue_node_character));
    newNode->ch = ch;
    newNode->next = NULL;
    return newNode;
}


void enqueueCharacter(struct queue_node_character** head, struct character ch) {
    struct queue_node_character* tmp = *head;
    struct queue_node_character* newNode = createNode(ch);
    if (*head == NULL || (*head)->ch.move_cost > ch.move_cost) {
        newNode->next = *head;
        *head = newNode;
    }
    else {
        while (tmp->next != NULL && tmp->next->ch.move_cost < ch.move_cost) {
            tmp = tmp->next;
        }
        newNode->next = tmp->next;
        tmp->next = newNode;
    }
}


void dequeueCharacter(struct queue_node_character** head) {
    if (*head == NULL) {
        return;
    }
    else {
        struct queue_node_character* tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}












