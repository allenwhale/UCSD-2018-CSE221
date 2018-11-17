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
  int index = 0;
  for (int i = 0; i < times; i++) {
    index = mem[index];
  }
  long long end = my_rdtsc();
  free(mem);
  return (double)(end - start) / (double)times;
}

// mem_size Kbytes
double memory_read_bandwidth(int mem_size, int times) {
  mem_size *= 1024;
  char *mem = (char *)malloc(sizeof(char) * mem_size);

  for (int i = 0; i < mem_size; i++) {
    mem[i] = rand();
  }
  long long start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    char *p = mem;
    int sum = 0;
    while (p < mem + mem_size) {
#define RP0(i) p[i]
#define RP1(i) RP0(i) + RP0(i + 1)
#define RP2(i) RP1(i) + RP1(i + 2)
#define RP3(i) RP2(i) + RP2(i + 4)
#define RP4(i) RP3(i) + RP3(i + 8)
#define RP5(i) RP4(i) + RP4(i + 16)
#define RP6(i) RP5(i) + RP5(i + 32)
#define RP7(i) RP6(i) + RP6(i + 64)
      sum += RP7(0);
      p += 128;
    }
  }
  long long end = my_rdtsc();
  return (double)(times * mem_size) / (double)(end - start);
}

// mem_size Kbytes
double memory_write_bandwidth(int mem_size, int times) {
  mem_size *= 1024;
  char *mem = (char *)malloc(sizeof(char) * mem_size);
  long long start = my_rdtsc();

  for (int i = 0; i < times; i++) {
    char *p = mem;
    while (p < mem + mem_size) {
#define WP0(i) p[i] = i;
#define WP1(i) WP0(i) WP0(i + 1)
#define WP2(i) WP1(i) WP1(i + 2)
#define WP3(i) WP2(i) WP2(i + 4)
#define WP4(i) WP3(i) WP3(i + 8)
#define WP5(i) WP4(i) WP4(i + 16)
#define WP6(i) WP5(i) WP5(i + 32)
#define WP7(i) WP6(i) WP6(i + 64)
      WP7(0)
      p += 128;
    }
  }
  long long end = my_rdtsc();
  return (double)(times * mem_size) / (double)(end - start);
}

// mem_size Kbytes
double memory_pagefault(int mem_size, int times) {
  mem_size <<= 10;
  int fd = open("/tmp/tmpfile", O_RDWR | O_CREAT | O_TRUNC, 0600);
  lseek(fd, mem_size - 1, SEEK_SET);
  write(fd, "", 1);
  close(fd);
  int page_size = getpagesize();
  long long total = 0;
  for (int i = 0; i < times; i++) {
    // int sum = 0;
    fd = open("/tmp/tmpfile", O_RDWR);
    char *mem =
        (char *)mmap(0, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    int offset = i & (page_size - 1);
    long long start = my_rdtsc();
    while (offset < mem_size) {
      // sum += mem[offset];
      mem[offset] = offset;
      offset += page_size;
    }
    long long end = my_rdtsc();
    munmap(mem, mem_size);
    close(fd);
    total += end - start;
  }
  return (double)total / (double)(times * (mem_size / page_size));
}
