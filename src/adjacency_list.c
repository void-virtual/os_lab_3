#include "adjacency_list.h"

bool v_empty(vector* v) {
    return v->size == 0;
}

size_t v_size(vector* v) {
    return v->size;
}

void v_init(vector* v) {
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

pair v_get(vector* v, size_t index) {
    if (index < v->size) {
        return v->data[index];
    }
}

void v_put(vector* v, size_t index, pair elem) {
    if (index < v->size) {
        v->data[index] = elem;
    }
}

void v_resize(vector* v, size_t new_size) {
    if (new_size == 0) {
        v->capacity = 0;
        v->size = 0;
        free(v->data);
        v->data = NULL;
    }  else {
        v->data = realloc(v->data, new_size * sizeof(pair));
        v->size = new_size;
        v->capacity = new_size;
    }
}

void v_push(vector* v, pair elem) {
    if (v->size == v->capacity) {
        v->capacity = v->capacity == 0 ? 1 : v->capacity * 2;
        v->data = realloc(v->data, v->capacity * sizeof(pair));
    }
    v->data[v->size] = elem;
    v->size++;
}

void v_destroy(vector* v) {
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

void a_init(adjacency_list* list) {
    list->size = 0;
    list->vecs = NULL;
}

void a_resize(adjacency_list* list, size_t new_size) {
    size_t old_size = list->size;
    if (old_size == new_size) {
        return;
    }
    if (old_size > new_size) {
        for (int i = new_size; i < old_size; ++i) {
            v_destroy(&list->vecs[i]);
        }
        list->vecs = realloc(list->vecs, new_size * sizeof(vector));
    } else {
        list->vecs = realloc(list->vecs, new_size * sizeof(vector));
        for (int i = old_size; i < new_size; ++i) {
            v_init(&list->vecs[i]);
        }
    }
    list->size = new_size;
}

void a_destroy(adjacency_list* list) {
    for (int i = 0; i < list->size; ++i) {
        v_destroy(&list->vecs[i]);
    }
    free(list->vecs);
    list->size = 0;
    list->vecs = NULL;
}
