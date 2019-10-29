#ifndef OS_LAB_3_QUEUE_H
#define OS_LAB_3_QUEUE_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct QueueItem queue_item;
struct QueueItem {
    struct QueueItem* next;
    struct QueueItem* prev;
    int value;
};

typedef struct Queue queue;
struct Queue {
    queue_item* head;
    queue_item* tail;
    size_t size;
};

void q_init(queue* q);
int q_top(queue* q);
int q_pop(queue* q);
size_t q_size(queue* q);
void q_push(queue* q, int elem);
bool q_empty(queue* q);
void q_destroy(queue* q);

#endif //OS_LAB_3_QUEUE_H
