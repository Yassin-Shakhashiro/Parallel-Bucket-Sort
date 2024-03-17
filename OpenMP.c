#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARRAY_SIZE 100
#define NUM_BUCKETS 10

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

// Function to perform bucket sort
void bucketSort(int *arr, int n) {
    // Create buckets
    int buckets[NUM_BUCKETS][ARRAY_SIZE] = {0};
    
    // Distribute elements into buckets
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int index = arr[i] / (ARRAY_SIZE / NUM_BUCKETS);
        buckets[index][i] = arr[i];
    }
    
    // Sort individual buckets
    #pragma omp parallel for
    for (int i = 0; i < NUM_BUCKETS; i++) {
        insertionSort(buckets[i], n);
    }
    
    // Merge buckets
    int k = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < n; j++) {
            if (buckets[i][j] != 0) {
                arr[k++] = buckets[i][j];
            }
        }
    }
}

