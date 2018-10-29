#ifndef __CPU__H__
#define __CPU__H__

#include "litc.h"

double measure_overhead(int);
double loop_overhead(int);
void test0();
void test1(int);
void test2(int, int);
void test3(int, int, int);
void test4(int, int, int, int);
void test5(int, int, int, int, int);
void test6(int, int, int, int, int, int);
void test7(int, int, int, int, int, int, int);
void procedure_call_overhead(int, double*);
double syscall_time_overhead(int);
double syscall_getpid_overhead(int);
double create_process_overhead(int);
void* thread_function(void*);
double create_thread_overhead(int);
long long process_context_switch_once_overhead();
double process_context_switch_overhead(int);
long long thread_context_switch_once_overhead();
double thread_context_switch_overhead(int);


#endif
