#include <stdlib.h>

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

long long my_rdtsc() {
  struct timeval t;

  gettimeofday(&t, NULL);
  return (long long)t.tv_sec * 1000000ll + t.tv_usec;
}

double memory_read_bandwidth(int mem_size, int times) {
  mem_size <<= 8;
  int *mem = (int *)malloc(sizeof(int) * mem_size);

  for (int i = 0; i < mem_size; i++) {
    mem[i] = rand();
  }
  long long start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    int *p = mem;
    int sum = 0;
    while (p < mem + mem_size) {
      sum += p[0] + p[2] + p[4] + p[6] + p[8] + p[10] + p[12] + p[14] + p[16] +
             p[18] + p[20] + p[22] + p[24] + p[26] + p[28] + p[30] + p[32] +
             p[34] + p[36] + p[38] + p[40] + p[42] + p[44] + p[46] + p[48] +
             p[50] + p[52] + p[54] + p[56] + p[58] + p[60] + p[62] + p[64] +
             p[66] + p[68] + p[70] + p[72] + p[74] + p[76] + p[78] + p[80] +
             p[82] + p[84] + p[86] + p[88] + p[90] + p[92] + p[94] + p[96] +
             p[98] + p[100] + p[102] + p[104] + p[106] + p[108] + p[110] +
             p[112] + p[114] + p[116] + p[118] + p[120] + p[122] + p[124] +
             p[126];
      p += 128;
    }
  }
  long long end = my_rdtsc();
  return (double)(times * mem_size * 4 / 2) / (double)(end - start);
}

// mem_size Kbytes
double memory_write_bandwidth(int mem_size, int times) {
  mem_size <<= 8;
  int *mem = (int *)malloc(sizeof(int) * mem_size);
  long long start = my_rdtsc();

  for (int i = 0; i < times; i++) {
    int *p = mem;
    while (p < mem + mem_size) {
      p[0] = 0;
      p[2] = 2;
      p[4] = 4;
      p[6] = 6;
      p[8] = 8;
      p[10] = 10;
      p[12] = 12;
      p[14] = 14;
      p[16] = 16;
      p[18] = 18;
      p[20] = 20;
      p[22] = 22;
      p[24] = 24;
      p[26] = 26;
      p[28] = 28;
      p[30] = 30;
      p[32] = 32;
      p[34] = 34;
      p[36] = 36;
      p[38] = 38;
      p[40] = 40;
      p[42] = 42;
      p[44] = 44;
      p[46] = 46;
      p[48] = 48;
      p[50] = 50;
      p[52] = 52;
      p[54] = 54;
      p[56] = 56;
      p[58] = 58;
      p[60] = 60;
      p[62] = 62;
      p[64] = 64;
      p[66] = 66;
      p[68] = 68;
      p[70] = 70;
      p[72] = 72;
      p[74] = 74;
      p[76] = 76;
      p[78] = 78;
      p[80] = 80;
      p[82] = 82;
      p[84] = 84;
      p[86] = 86;
      p[88] = 88;
      p[90] = 90;
      p[92] = 92;
      p[94] = 94;
      p[96] = 96;
      p[98] = 98;
      p[100] = 100;
      p[102] = 102;
      p[104] = 104;
      p[106] = 106;
      p[108] = 108;
      p[110] = 110;
      p[112] = 112;
      p[114] = 114;
      p[116] = 116;
      p[118] = 118;
      p[120] = 120;
      p[122] = 122;
      p[124] = 124;
      p[126] = 126;
      p += 128;
    }
  }
  long long end = my_rdtsc();
  return (double)(times * mem_size * 4 / 2) / (double)(end - start);
}
int main(int argc, char const *argv[]) {
  long long mem_size = 1 << 14;
  // for (int k = 0; k < 10; ++k) {
  printf("R %f\n", memory_read_bandwidth(mem_size, 100));
  printf("W %f\n", memory_write_bandwidth(mem_size, 100));
  // }
  return 0;
}
