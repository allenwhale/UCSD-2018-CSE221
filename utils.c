#include "utils.h"
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

long long my_rdtsc(){
    struct timeval t;

    gettimeofday(&t, NULL);
    return (long long)t.tv_sec * 1000000ll + t.tv_usec;
    int lo, hi;

    __asm__ __volatile__ (
        "xor %%eax, %%eax;"
        "cpuid;"
        "rdtsc;" : "=a" (lo), "=d" (hi));
    return(((long long)hi << 32) | lo);
}
