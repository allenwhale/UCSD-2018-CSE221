#ifndef __FS__H__
#define __FS__H__
#include "utils.h"

#define MEGABYTE 1 << 20
#define BUFFER_SIZE 100 * (1 << 20)

double file_read_time(char*, long long, int);

#endif
