#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include "adjacency_list.h"
#include "queue.h"

int main() {
    int n = 3;
    int matrix[3][3] = {{0,5,1},{5,0,10},{1,10,0}};
    pthread_mutex_t *mutexes = malloc(n * sizeof(pthread_mutex_t));

    pthread_mutex_t *thread_count_mutex = malloc(sizeof(pthread_mutex_t));
    return 0;
}