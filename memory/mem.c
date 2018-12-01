#include "mem.h"

// mem_size Kbytes
// step_size bytes
double memory_access_latency(int mem_size, int step_size, int times) {
  mem_size *= 1024 / 4;
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
  printf("%lld %lld %lld\n", start, end, end - start);
  return (double)(end - start) / (double)times;
}

#define RP0(i, s) p[i + s]
#define RP1(i, s) RP0(i, s) + RP0(i + 1 * s, s)
#define RP2(i, s) RP1(i, s) + RP1(i + 2 * s, s)
#define RP3(i, s) RP2(i, s) + RP2(i + 4 * s, s)
#define RP4(i, s) RP3(i, s) + RP3(i + 8 * s, s)
#define RP5(i, s) RP4(i, s) + RP4(i + 16 * s, s)
#define RP6(i, s) RP5(i, s) + RP5(i + 32 * s, s)
#define RP7(i, s) RP6(i, s) + RP6(i + 64 * s, s)
// mem_size Kbytes
double memory_read_bandwidth(long long mem_size, long long step,
                             long long times) {
  mem_size <<= 10;
  char *mem = (char *)malloc(sizeof(char) * mem_size);

  for (int i = 0; i < mem_size; i++) {
    mem[i] = rand();
  }
  long long start = my_rdtsc();
  for (int i = 0; i < times * step; i++) {
    char *p = mem;
    int sum = 0;
    while (p + 127 < mem + mem_size) {
      if (step == 1) {
        sum += RP7(0, 1);
      } else if (step == 2) {
        sum += RP6(0, 2);
      } else if (step == 4) {
        sum += RP5(0, 4);
      } else if (step == 8) {
        sum += RP4(0, 8);
      } else if (step == 16) {
        sum += RP3(0, 16);
      } else if (step == 32) {
        sum += RP2(0, 32);
      } else if (step == 64) {
        sum += RP1(0, 64);
      }
      p += 128;
    }
  }
  long long end = my_rdtsc();
  free(mem);
  return (double)(times * mem_size) / (double)(end - start);
}

#define WP0(i, s) p[i] = i;
#define WP1(i, s) WP0(i, s) WP0(i + 1 * s, s)
#define WP2(i, s) WP1(i, s) WP1(i + 2 * s, s)
#define WP3(i, s) WP2(i, s) WP2(i + 4 * s, s)
#define WP4(i, s) WP3(i, s) WP3(i + 8 * s, s)
#define WP5(i, s) WP4(i, s) WP4(i + 16 * s, s)
#define WP6(i, s) WP5(i, s) WP5(i + 32 * s, s)
#define WP7(i, s) WP6(i, s) WP6(i + 64 * s, s)
// mem_size Kbytes
double memory_write_bandwidth(long long mem_size, long long step,
                              long long times) {
  mem_size <<= 10;
  char *mem = (char *)malloc(sizeof(char) * mem_size);
  long long start = my_rdtsc();

  for (int i = 0; i < times * step; i++) {
    char *p = mem;
    while (p + 127 < mem + mem_size) {
      if (step == 1) {
        WP7(0, 1);
      } else if (step == 2) {
        WP6(0, 2);
      } else if (step == 4) {
        WP5(0, 4);
      } else if (step == 8) {
        WP4(0, 8);
      } else if (step == 16) {
        WP3(0, 16);
      } else if (step == 32) {
        WP2(0, 32);
      } else if (step == 64) {
        WP1(0, 64);
      }
      p += 128;
    }
  }
  long long end = my_rdtsc();
  free(mem);
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
