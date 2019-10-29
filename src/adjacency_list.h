#ifndef OS_LAB_3_ADJACENCY_LIST_H
#define OS_LAB_3_ADJACENCY_LIST_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Pair pair;
struct Pair {
    int vertex_number;
    int length;
};

typedef struct Vector vector;
struct Vector {
    size_t size;
    size_t capacity;
    pair* data;
};

void v_init(vector* v);
bool v_empty(vector* v);
size_t v_size(vector* v);
pair v_get(vector* v, size_t index);
void v_put(vector* v, size_t index, pair elem);
void v_resize(vector* v, size_t new_size);
void v_push(vector* v, pair elem);
void v_destroy(vector* v);

typedef struct Adjacency_list adjacency_list;
struct Adjacency_list {
    size_t size;
    vector* vecs;
};

void a_init(adjacency_list* list);
void a_resize(adjacency_list* list, size_t new_size);
void a_destroy(adjacency_list* list);

#endif //OS_LAB_3_ADJACENCY_LIST_H
