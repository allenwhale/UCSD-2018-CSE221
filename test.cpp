#include <stdlib.h>

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <vector>
using namespace std;

long long my_rdtsc() {
  struct timeval t;

  gettimeofday(&t, NULL);
  return (long long)t.tv_sec * 1000000ll + t.tv_usec;
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

int main(int argc, char const *argv[]) {
  long long mem_size = 1 << 13;
  int times = 100;
  for (int s = 1; s <= 64; s <<= 1) {
    vector<double> v;
    for (int k = 0; k < times; ++k) {
      v.push_back(memory_read_bandwidth(mem_size, s, 1));
    }
    sort(v.begin(), v.end());
    double sum = 0;
    for (int i = 25; i < 75; ++i) {
      sum += v[i];
    }
    printf("%f\n", sum / 50);
  }

  for (int s = 1; s <= 64; s <<= 1) {
    vector<double> v;
    for (int k = 0; k < times; ++k) {
      v.push_back(memory_write_bandwidth(mem_size, s, 1));
    }
    sort(v.begin(), v.end());
    double sum = 0;
    for (int i = 25; i < 75; ++i) {
      sum += v[i];
    }
    printf("%f\n", sum / 50);
  }
  return 0;
}
