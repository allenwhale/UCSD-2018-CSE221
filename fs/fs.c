#include "fs.h"

double file_read_time(char* buf, long long size, int times) {
  printf("Size: %lld\n", size);
  char filename[] = "fsbench";
  // Create file of size (B)
  int fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, (mode_t)0600);
  if (fd == -1) {
    perror("Error opening file for writing");
    return -1;
  }
  int re = lseek(fd, size - 1, SEEK_SET);
  if (re == -1) {
    close(fd);
    perror("Error calling lseek() to 'stretch' the file");
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
      printf("%lld\n", cur_size);
    }
    long long end = my_rdtsc();
    duration += end - start;
    close(fd);
  }
  unlink(filename);
  return (double)(times * size) / (double)(duration);
}