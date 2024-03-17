#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 100
#define NUM_BUCKETS 10
#define NUM_THREADS 4

// Bucket structure
typedef struct {
    int count;
    int* values;
} Bucket;

// Function to perform insertion sort
void insertionSort(int *arr, int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// Function executed by each thread to sort its assigned bucket
void* threadFunction(void* arg) {
    Bucket* bucket = (Bucket*)arg;
    insertionSort(bucket->values, bucket->count);
    return NULL;
}

// Function to perform bucket sort
void bucketSort(int *arr, int n) {
    // Create buckets
    Bucket buckets[NUM_BUCKETS];

    // Initialize buckets
    for (int i = 0; i < NUM_BUCKETS; i++) {
        buckets[i].count = 0;
        buckets[i].values = (int*)malloc(sizeof(int) * ARRAY_SIZE);
    }

    // Distribute elements into buckets
    for (int i = 0; i < n; i++) {
        int index = arr[i] / (ARRAY_SIZE / NUM_BUCKETS);
        buckets[index].values[buckets[index].count++] = arr[i];
    }

    // Create threads for sorting buckets
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_BUCKETS; i++) {
        pthread_create(&threads[i], NULL, threadFunction, (void*)&buckets[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_BUCKETS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Merge buckets
    int k = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < buckets[i].count; j++) {
            arr[k++] = buckets[i].values[j];
        }
        free(buckets[i].values);
    }
}


