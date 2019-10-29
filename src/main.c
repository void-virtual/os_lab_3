#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include "adjacency_list.h"
#include "queue.h"
// лаба 3, вариант 21
// поиск кратчайшего пути в графе поиском в ширину
// граф задается матрицей смежности

typedef struct ThreadParams thread_params;
struct ThreadParams {
    int current_vertex;
    adjacency_list *list;
    int *max_thread_count;
    pthread_mutex_t *mutexes;
    int *result;
    pthread_mutex_t *thread_count_mutex;
};

int min(int lhs, int rhs) {
    return lhs > rhs ? rhs : lhs;
}

//каждый раз новый поток принимает thread_params
//current_vertex - текущая вершина в обходе
//result - там хранятся подсчитанные расстояния до вершин, или -1 если не подсчитано
//max_thread_count - эта переменная уменьшается при создании нового потока,
//таким образом будет создано столько потоков сколько нужно
//mutexes - мьютексы для синхронизации доступа к result
//list - список смежности для графа, будем считать, что граф связный
void *bfs(void *args) {

    thread_params params = *((thread_params *) args);
    int current_vertex = params.current_vertex;
    int *max_thread_count = params.max_thread_count;
    adjacency_list *list = params.list;
    pthread_mutex_t *mutexes = params.mutexes;
    pthread_mutex_t *thread_count_mutex = params.thread_count_mutex;
    int *result = params.result;


    for (int i = 0; i < list->vecs[current_vertex].size; ++i) {
        int neighbour_vertex = list->vecs[current_vertex].data[i].vertex_number;
        int neighbour_vertex_dist = list->vecs[current_vertex].data[i].length;
        pthread_mutex_lock(&mutexes[neighbour_vertex]);
        if (result[neighbour_vertex] == -1 ||
            result[neighbour_vertex] > result[current_vertex] + neighbour_vertex_dist) {
            result[neighbour_vertex] = result[current_vertex] + neighbour_vertex_dist;
        }
        pthread_mutex_unlock(&mutexes[neighbour_vertex]);
    }

    pthread_mutex_lock(thread_count_mutex);
    int can_create_threads = min(*max_thread_count, list->vecs[current_vertex].size);
    (*max_thread_count) -= can_create_threads;
    pthread_mutex_unlock(thread_count_mutex);
    pthread_t threads[can_create_threads];
    for (int i = 0; i < can_create_threads; ++i) {
        params.current_vertex = list->vecs[current_vertex].data[i].vertex_number;
        pthread_create(&threads[i], NULL, bfs, &params);
    }

    queue q;
    q_init(&q);
    for (int i = can_create_threads; i < list->vecs[current_vertex].size; ++i) {
        q_push(&q, list->vecs[current_vertex].data[i].vertex_number);
    }

    while (!q_empty(&q)) {
        int cur = q_pop(&q);
        for (int i = 0; i < list->vecs[cur].size; ++i) {
            int neighbour_vertex = list->vecs[cur].data[i].vertex_number;
            int neighbour_vertex_dist = list->vecs[cur].data[i].length;
            pthread_mutex_lock(&mutexes[neighbour_vertex]);
            if (result[neighbour_vertex] == -1 ||
                result[neighbour_vertex] > result[current_vertex] + neighbour_vertex_dist) {
                result[neighbour_vertex] = result[current_vertex] + neighbour_vertex_dist;
                q_push(&q, neighbour_vertex);
            }
            pthread_mutex_unlock(&mutexes[neighbour_vertex]);
        }
    }
    for (int i = 0; i < can_create_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    pthread_exit(NULL);
}

int main() {
    int n;
    printf("Enter size of matrix\n");
    scanf("%d", &n);
    int matrix[n][n];
    printf("Enter matrix\n");
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%d", &matrix[i][j]);
        }
    }

    adjacency_list *list = malloc(sizeof(adjacency_list));
    a_init(list);
    a_resize(list, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] != 0) {
                v_push(&list->vecs[i], (pair) {j, matrix[i][j]});
            }
        }
    }
    size_t vertex_number;
    int *max_thread_count = malloc(sizeof(int));
    int *result = malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i) {
        result[i] = -1;
    }
    pthread_mutex_t *mutexes = malloc(n * sizeof(pthread_mutex_t));
    for (int i = 0; i < n; ++i) {
        pthread_mutex_init(&mutexes[i], NULL);
    }
    printf("Enter vertex to start search\n");
    scanf("%ld", &vertex_number);
    while (vertex_number > n || vertex_number == 0) {
        printf("Vertex number must be < %d\n and > 0", n);
        printf("Enter vertex to start search\n");
        scanf("%ld", &vertex_number);
    }
    vertex_number--;
    result[vertex_number] = 0;
    printf("Enter max thread count\n");
    scanf("%d", max_thread_count);
    while (*max_thread_count < 1 || *max_thread_count > n) {
        printf("Max thread count must be between 1 and %d", n);
        printf("Enter max thread count\n");
        scanf("%d", max_thread_count);
    }
    pthread_mutex_t* thread_count_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(thread_count_mutex, NULL);
    (*max_thread_count)--;
    thread_params parameters = (thread_params) {.max_thread_count = max_thread_count,
            .result = result,
            .current_vertex = vertex_number,
            .list = list,
            .mutexes = mutexes,
            .thread_count_mutex = thread_count_mutex};
    pthread_t first_thread;
    pthread_create(&first_thread, NULL, bfs, &parameters);
    pthread_join(first_thread, NULL);
    for (int i = 0; i < n; ++i) {
        printf("%d:%d ", i + 1, result[i]);
    }
    return 0;
}