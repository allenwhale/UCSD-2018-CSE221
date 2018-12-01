#include "fs.h"
#include "litc.h"
#include "utils.h"

int main() {
  char* buf = (char*)malloc(BUFFER_SIZE);
  for (long long s = 32; s < 1024; s <<= 1) {
    printf("%f\n", file_read_time(buf, s * MEGABYTE, 1));
  }
  free(buf);
  return 0;
}
