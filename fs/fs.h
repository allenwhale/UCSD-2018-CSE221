#ifndef __FS__H__
#define __FS__H__
#include "utils.h"

#define MEGABYTE (1 << 20)
#define KILOBYTE (1 << 10)
#define BUFFER_SIZE (1 << 14)
#define BLOCK_SIZE (1 << 12)
#define F_NOCACHE 0x30

double file_read_time(char*, char*, long long, int);
double file_read_rand(char*, long long, int, int);
void file_contention(int, int);
#endif
