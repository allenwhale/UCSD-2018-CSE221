#include "fs.h"
#include "litc.h"
#include "utils.h"

void file_cache_runner(char* buf, int times) {
  for (long long s = 16; s < 4096; s <<= 1) {
    printf("%f\n", file_read_time(buf, "fsbench", s * KILOBYTE, 100));
  }
}
void file_read_runner(char* buf, int times) {
  for (int f = 0; f < 2; ++f) {
    for (long long s = 16; s < 4096; s <<= 1) {
      printf("%f\n", file_read_rand(buf, s * KILOBYTE, 100, f));
    }
  }
}

void file_contention_runner(int numOfProcesses, int times) {
  file_contention(numOfProcesses, times);
}
void usage() {
  printf("./fs cache <times>\n");
  printf("./fs rand <times>\n");
  printf("./fs content <numOfProcess> <times>\n");
}

int main(int argc, char** argv) {
  if (argc < 2) {
    usage();
    return 1;
  }
  char* buf = (char*)malloc(BLOCK_SIZE);
  if (strcmp(argv[1], "cache") == 0) {
    int times = atoi(argv[2]);
    file_cache_runner(buf, times);
  } else if (strcmp(argv[1], "rand") == 0) {
    int times = atoi(argv[2]);
    file_read_runner(buf, times);
  } else if (strcmp(argv[1], "content") == 0) {
    int numOfProcesses = atoi(argv[2]);
    int times = atoi(argv[3]);
    file_contention_runner(numOfProcesses, times);
  }
  free(buf);
  return 0;
}
