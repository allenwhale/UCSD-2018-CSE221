#include "fs.h"
#include "litc.h"
#include "utils.h"

void file_cache_runner(char* buf) {
  for (long long s = 16; s < 4096; s <<= 1) {
    printf("%f\n", file_read_time(buf, s * KILOBYTE, 100));
  }
}
void file_read_runner(char* buf) {
  for (int f = 0; f < 2; ++f) {
    for (long long s = 16; s < 4096; s <<= 1) {
      printf("%f\n", file_read_rand(buf, s * KILOBYTE, 100, f));
    }
  }
}

int main(int argc, char** argv) {
  char* buf = (char*)malloc(BLOCK_SIZE);
  // file_cache_runner(buf);
  file_read_runner(buf);
  free(buf);
  return 0;
}
