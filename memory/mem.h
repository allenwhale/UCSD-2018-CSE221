#ifndef __MEM__H__
#define __MEM__H__
#include "utils.h"

double memory_access_latency(int, int, int);
double memory_read_bandwidth(long long, long long);
double memory_write_bandwidth(long long, long long);
double memory_pagefault(int, int);
#endif
