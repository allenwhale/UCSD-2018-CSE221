#include "utils.h"
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
long long my_rdtsc() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (long long)t.tv_sec * 1000000ll + t.tv_usec;
}
int cmp_ll(const void *a, const void *b) {
  return *((long long *)a) - *((long long *)b);
}

void sort(double *arr, int size) {
  // Bubble sort
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size - i - 1; ++j) {
      if (arr[j] > arr[j + 1]) {
        double lt = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = lt;
      }
    }
  }
}
