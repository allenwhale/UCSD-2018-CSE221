#include "fs.h"

double file_read_time(char *buf, char *filename, long long size, int times) {
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

  long long *rbw = (long long *)malloc(sizeof(long long) * (times + 20));

  for (int i = 0; i < times + 20; ++i) {
    fd = open(filename, O_RDONLY);
    long long start = my_rdtsc();
    while (read(fd, buf, BLOCK_SIZE) > 0) {
    }
    long long end = my_rdtsc();
    rbw[i] = end - start;
    close(fd);
  }
  qsort(rbw, times + 20, sizeof(long long), cmp_ll);
  double res = 0;
  for (int i = 10; i < times + 10; i++) {
    res += (double)size / rbw[i] / times;
  }
  unlink(filename);
  free(rbw);
  return res;
}
double file_read_rand(char *buf, long long size, int times, int flag) {
  char filename[] = "fsbench";
  // Create file of size (B)
  int fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, (mode_t)0600);
  if (fd == -1) {
    perror("Error opening file for writing");
    return -1;
  }
  // if (fcntl(fd, 0x30, 1) == -1) {
  //   printf("File cache disabled failed\n");
  // }
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

  long long *rbw = (long long *)malloc(sizeof(long long) * (times + 20));
  int blknums = size / BLOCK_SIZE;
  int *offsets = (int *)malloc(sizeof(int) * (blknums));
  for (int i = 0; i < times + 20; ++i) {
    fd = open(filename, O_RDONLY);
    if (flag) {
      for (int b = 0; b < blknums; ++b) {
        offsets[b] = rand() % blknums;
      }
      long long start = my_rdtsc();
      for (int b = 0; b < blknums; ++b) {
        lseek(fd, offsets[b] * BLOCK_SIZE, SEEK_SET);
        read(fd, buf, BLOCK_SIZE);
      }
      long long end = my_rdtsc();
      rbw[i] = end - start;
    } else {
      long long start = my_rdtsc();
      for (int b = 0; b < blknums; ++b) {
        read(fd, buf, BLOCK_SIZE);
      }
      long long end = my_rdtsc();
      rbw[i] = end - start;
    }
    close(fd);
  }
  qsort(rbw, times + 20, sizeof(long long), cmp_ll);
  double res = 0;
  for (int i = 10; i < times + 10; i++) {
    res += (double)blknums / rbw[i] / times;
  }
  unlink(filename);
  free(rbw);
  free(offsets);
  return res;
}

void file_contention(int numOfProcesses, int times) {
  char **fnames = (char **)malloc(sizeof(char *) * numOfProcesses);
  for (int i = 0; i < numOfProcesses; ++i) {
    fnames[i] = (char *)malloc(sizeof(char) * 128);
    sprintf(fnames[i], "fs_%d", i);
  }
  for (int i = 0; i < numOfProcesses; ++i) {
    pid_t pid;
    if ((pid = fork()) == 0) {
      char *buf = (char *)malloc(sizeof(char) * BLOCK_SIZE);
      printf("%f\n", file_read_time(buf, fnames[i], BLOCK_SIZE, times));
      exit(0);
    } else {
    }
  }
  wait(0);
  for (int i = 0; i < numOfProcesses; ++i) {
    free(fnames[i]);
  }
  free(fnames);
}