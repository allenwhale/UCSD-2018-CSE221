#include "cpu.h"
#include <pthread.h>
#include "utils.h"

double measure_overhead(int times) {
  double total = 0.;

  for (int i = 0; i < times; i++) {
    long long start = my_rdtsc();
    long long end   = my_rdtsc();
    total += end - start;
  }
  return total / (double)times;
}
double loop_overhead(int times) {
  long long start = my_rdtsc();

  for (int i = 0; i < times; i++)
    ;
  long long end = my_rdtsc();

  return (double)(end - start) / (double)times;
}
void test0() {}
void test1(int a1) {}
void test2(int a1, int a2) {}
void test3(int a1, int a2, int a3) {}
void test4(int a1, int a2, int a3, int a4) {}
void test5(int a1, int a2, int a3, int a4, int a5) {}
void test6(int a1, int a2, int a3, int a4, int a5, int a6) {}
void test7(int a1, int a2, int a3, int a4, int a5, int a6, int a7) {}
void procedure_call_overhead(int times, double* ans) {
  long long start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    test0();
  }
  long long end = my_rdtsc();
  ans[0]        = (end - start) / (double)times;

  start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    test1(1);
  }
  end    = my_rdtsc();
  ans[1] = (end - start) / (double)times;

  start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    test2(1, 2);
  }
  end    = my_rdtsc();
  ans[2] = (end - start) / (double)times;

  start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    test3(1, 2, 3);
  }
  end    = my_rdtsc();
  ans[3] = (end - start) / (double)times;

  start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    test4(1, 2, 3, 4);
  }
  end    = my_rdtsc();
  ans[4] = (end - start) / (double)times;

  start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    test5(1, 2, 3, 4, 5);
  }
  end    = my_rdtsc();
  ans[5] = (end - start) / (double)times;

  start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    test6(1, 2, 3, 4, 5, 6);
  }
  end    = my_rdtsc();
  ans[6] = (end - start) / (double)times;

  start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    test7(1, 2, 3, 4, 5, 6, 7);
  }
  end    = my_rdtsc();
  ans[7] = (end - start) / (double)times;
}

double syscall_time_overhead(int times) {
  long long start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    time(0);
  }
  long long end = my_rdtsc();
  return (end - start) / (double)times;
}

double syscall_getpid_overhead(int times) {
  long long start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    getpid();
  }
  long long end = my_rdtsc();
  return (end - start) / (double)times;
}

double create_process_overhead(int times) {
  long long start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    int pid = fork();
    if (pid == 0) exit(0);
    wait(0);
  }
  long long end = my_rdtsc();
  return (end - start) / (double)times;
}

void* thread_function(void* a) { return NULL; }
double create_thread_overhead(int times) {
  long long start = my_rdtsc();
  pthread_t tid;
  for (int i = 0; i < times; i++) {
    pthread_create(&tid, (pthread_attr_t*)0, thread_function, 0);
    pthread_join(tid, 0);
  }
  long long end = my_rdtsc();
  return (end - start) / (double)times;
}

long long process_context_switch_once_overhead() {
  int fd[2];
  long long start, end;

  pipe(fd);
  if (fork() == 0) {
    end = my_rdtsc();
    write(fd[1], &end, sizeof(end));
    close(fd[0]), close(fd[1]);
    exit(0);
  } else {
    start = my_rdtsc();
    wait(0);
    read(fd[0], &end, sizeof(end));
    close(fd[0]), close(fd[1]);
  }
  return end - start;
}

double process_context_switch_overhead(int times) {
  int i        = 0;
  double total = 0.;

  while (i < times) {
    int res = process_context_switch_once_overhead();
    if (res > 0) {
      total += res;
      i++;
    }
  }
  return total / (double)times;
}
void* thread_context_switch(void* arg) {
  int* fd       = (int*)arg;
  long long end = my_rdtsc();

  write(fd[1], &end, sizeof(end));
  return NULL;
}
long long thread_context_switch_once_overhead() {
  long long start, end = 1;
  int fd[2];
  pthread_t tid;

  pipe(fd);
  pthread_create(&tid, 0, thread_context_switch, (void*)fd);
  start = my_rdtsc();
  pthread_join(tid, 0);
  read(fd[0], &end, sizeof(end));
  close(fd[0]), close(fd[1]);
  return end - start;
}
double thread_context_switch_overhead(int times) {
  int i        = 0;
  double total = 0.;

  while (i < times) {
    int res = thread_context_switch_once_overhead();
    if (res > 0) {
      total += res;
      i++;
    }
  }
  return total / (double)times;
}
