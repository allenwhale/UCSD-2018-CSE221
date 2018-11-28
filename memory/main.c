#include "litc.h"
#include "mem.h"
#include "utils.h"

void memory_access_runner() {
  for (int j = 4; j < 11; j += 2) {
    for (int i = 4; i < 15; ++i) {
      int times = 100;
      double latency[100];
      for (int k = 0; k < times; ++k) {
        int fd[2];
        double l;
        pipe(fd);
        if (fork() == 0) {
          long long mem_size = 1 << i;
          long long stride = 1 << j;
          l = memory_access_latency(mem_size, stride,
                                    max(1000, mem_size / stride));
          write(fd[1], &l, sizeof(l));
          for (int m = 0; m < 2; ++m) close(fd[m]);
          exit(0);
        } else {
          wait(0);
          read(fd[0], &l, sizeof(l));
          for (int m = 0; m < 2; ++m) close(fd[m]);
          latency[k] = l;
        }
      }
      // ignore outliers (perhaps because of GC)
      sort(latency, 100);
      double suml = 0;
      for (int m = 25; m < 75; ++m) {
        suml += latency[m];
      }
      printf("%f\n", suml / 50);
    }
  }
}

void bandwidth_runner() {
// testing 8MB,
#define TIMES 100
  long long mem_size = 1 << 13;
  double bandwidth[TIMES];
  for (int s = 1; s < 7; s++) {
    for (int k = 0; k < TIMES + 4; ++k) {
      if (k >= 4) {
        bandwidth[k - 4] = memory_read_bandwidth(mem_size, 1 << s, 1);
      }
    }
    sort(bandwidth, TIMES);
    double sum = 0;
    for (int m = 25; m < 75; ++m) {
      sum += bandwidth[m];
    }
    printf("%f\n", sum / 50);
  }

  for (int s = 1; s < 7; s++) {
    for (int k = 0; k < TIMES + 4; ++k) {
      if (k >= 4) {
        bandwidth[k - 4] = memory_write_bandwidth(mem_size, 1 << s, 1);
      }
    }
    sort(bandwidth, TIMES);
    sum = 0;
    for (int m = 25; m < 75; ++m) {
      sum += bandwidth[m];
    }
    printf("%f\n", sum / 50);
  }
}

void pagefault_runner() {
  // testing 8MB,
  int times = 100;
  long long mem_size = 1 << 13;
  for (int k = 0; k < times + 4; ++k) {
    if (fork() == 0) {
      if (k >= 4) {
        printf("%f\n", memory_pagefault(mem_size, 1));
      }
      exit(0);
    } else {
      wait(0);
    }
  }
}

int main() {
  bandwidth_runner();
  return 0;
}
