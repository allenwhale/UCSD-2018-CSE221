#include <stdlib.h>

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <vector>

#define MEGABYTE 1 << 20
#define BUFFER_SIZE 100 * (1 << 20)
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

void mem_test() {
  long long mem_size = 1 << 13;
  int times = 10;
  for (int s = 1; s <= 64; s <<= 1) {
    vector<double> v;
    for (int k = 0; k < times; ++k) {
      v.push_back(memory_read_bandwidth(mem_size, s, 1));
    }
    sort(v.begin(), v.end());
    double sum = 0;
    for (int i = 3; i < 8; ++i) {
      sum += v[i];
    }
    printf("%f\n", sum / 5);
  }

  for (int s = 1; s <= 64; s <<= 1) {
    vector<double> v;
    for (int k = 0; k < times; ++k) {
      v.push_back(memory_write_bandwidth(mem_size, s, 1));
    }
    sort(v.begin(), v.end());
    double sum = 0;
    for (int i = 3; i < 8; ++i) {
      sum += v[i];
    }
    printf("%f\n", sum / 5);
  }
}

double file_read_time(char *buf, long long size, int times) {
  printf("Size: %lld\n", size);
  char filename[] = "fsbench";
  // Create file of size (B)
  int fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, (mode_t)0600);
  if (fd == -1) {
    perror("Error opening file for writing");
    return -1;
  }

  int re = lseek64(fd, size - 1, SEEK_SET);
  if (re == -1) {
    close(fd);
    perror("Error calling lseek64() to 'stretch' the file");
    return -1;
  }
  re = write(fd, "", 1);
  if (re < 0) {
    close(fd);
    perror("Error writing a byte at the end of the file");
    return -1;
  }
  close(fd);

  long long duration = 1;
  for (int i = 0; i < times; ++i) {
    fd = open(filename, O_RDONLY);
    long long cur_size = 0;
    long long start = my_rdtsc();
    while (cur_size < size) {
      cur_size += read(fd, buf, BUFFER_SIZE);
    }
    long long end = my_rdtsc();
    duration += end - start;
    close(fd);
  }
  unlink(filename);
  return (double)(times * size) / (double)(duration);
}

void fs_test() {
  char *buf = (char *)malloc(BUFFER_SIZE);
  for (long long s = 32; s < 8192; s <<= 1) {
    printf("%f\n", file_read_time(buf, s * MEGABYTE, 1));
  }
  free(buf);
}

int main(int argc, char const *argv[]) {
  fs_test();
  return 0;
}
