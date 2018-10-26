#include "utils.h"

long long my_rdtsc(){
    int lo, hi;

    __asm__ __volatile__ (
        "xor %%eax, %%eax;"
        "cpuid;"
        "rdtsc;" : "=a" (lo), "=d" (hi));
    return(((long long)hi << 32) | lo);
}
