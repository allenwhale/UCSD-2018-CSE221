#include "litc.h"
#include "mem.h"
#include "utils.h"

long long max(long long a, long long b) { return a > b ? a : b; }

void memory_access_runner() {
  for (int j = 4; j < 11; j += 2) {
    for (int i = 4; i < 15; ++i) {
      double latency = 0;
      int times = 100;
      for (int k = 0; k < times + 4; ++k) {
        int fd[2];
        double l;
        pipe(fd);
        if (fork() == 0) {
          long long mem_size = 1 << i;
          long long stride = 1 << j;
          l = memory_access_latency(mem_size, stride,
                                    max(10000, mem_size / stride));
          write(fd[1], &l, sizeof(l));
          for (int m = 0; m < 2; ++m) close(fd[m]);
          exit(0);
        } else {
          wait(0);
          read(fd[0], &l, sizeof(l));
          for (int m = 0; m < 2; ++m) close(fd[m]);
          if (k >= 4) {
            latency += l;
          }
        }
      }
      printf("%f\n", latency / times);
    }
  }
}

int main() {
  memory_access_runner();
  return 0;
}
