#include "mem.h"

// mem_size Kbytes
// step_size bytes
double memory_access_latency(int mem_size, int step_size, int times) {
  mem_size *= 256;
  step_size /= 4;
  int *mem = (int *)malloc(sizeof(int) * mem_size);
  for (int i = 0; i < mem_size; i++) {
    mem[i] = (i + step_size) % mem_size;
  }
  long long start = my_rdtsc();
  int index       = 0;
  for (int i = 0; i < times; i++) {
    index = mem[index];
  }
  long long end = my_rdtsc();
  free(mem);
  return (double)(end - start) / (double)times;
}
