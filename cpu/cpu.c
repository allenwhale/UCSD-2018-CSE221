#include "cpu.h"
#include "utils.h"
#include <pthread.h>


long long now(){
    struct timeval t;

    gettimeofday(&t, NULL);
    return (long long)t.tv_sec * 1000000ll + t.tv_usec;
}

double measure_overhead(int times){
    double total = 0.;

    for (int i = 0; i < times; i++) {
		// long long start = my_rdtsc();
		// long long end = my_rdtsc();
        long long start = now();
        long long end = now();
        total += end - start;
    }
    return total / (double)times;
}
double loop_overhead(int times){
    long long start = my_rdtsc();

    for (int i = 0; i < times; i++);
    long long end = my_rdtsc();
    return (double)(end - start) / (double)times;
}
void test0(){
}
void test1(int a1){
}
void test2(int a1, int a2){
}
void test3(int a1, int a2, int a3){
}
void test4(int a1, int a2, int a3, int a4){
}
void test5(int a1, int a2, int a3, int a4, int a5){
}
void test6(int a1, int a2, int a3, int a4, int a5, int a6){
}
void test7(int a1, int a2, int a3, int a4, int a5, int a6, int a7){
}
void procedure_call_overhead(int times, double *ans){
    double total = 0.;

    for (int i = 0; i < times; i++) {
        long long start = my_rdtsc();
        test0();
        long long end = my_rdtsc();
        total += end - start;
    }
    ans[0] = total / (double)times;

    total = 0.;
    for (int i = 0; i < times; i++) {
        long long start = my_rdtsc();
        test1(1);
        long long end = my_rdtsc();
        total += end - start;
    }
    ans[1] = total / (double)times;

    total = 0.;
    for (int i = 0; i < times; i++) {
        long long start = my_rdtsc();
        test2(1, 2);
        long long end = my_rdtsc();
        total += end - start;
    }
    ans[2] = total / (double)times;


    total = 0.;
    for (int i = 0; i < times; i++) {
        long long start = my_rdtsc();
        test3(1, 2, 3);
        long long end = my_rdtsc();
        total += end - start;
    }
    ans[3] = total / (double)times;

    total = 0.;
    for (int i = 0; i < times; i++) {
        long long start = my_rdtsc();
        test4(1, 2, 3, 4);
        long long end = my_rdtsc();
        total += end - start;
    }
    ans[4] = total / (double)times;

    total = 0.;
    for (int i = 0; i < times; i++) {
        long long start = my_rdtsc();
        test5(1, 2, 3, 4, 5);
        long long end = my_rdtsc();
        total += end - start;
    }
    ans[5] = total / (double)times;

    total = 0.;
    for (int i = 0; i < times; i++) {
        long long start = my_rdtsc();
        test6(1, 2, 3, 4, 5, 6);
        long long end = my_rdtsc();
        total += end - start;
    }
    ans[6] = total / (double)times;

    total = 0.;
    for (int i = 0; i < times; i++) {
        long long start = my_rdtsc();
        test7(1, 2, 3, 4, 5, 6, 7);
        long long end = my_rdtsc();
        total += end - start;
    }
    ans[7] = total / (double)times;
}

double syscall_time_overhead(int times){
    double total = 0.;

    for (int i = 0; i < times; i++) {
        long long start = my_rdtsc();
        time(0);
        long long end = my_rdtsc();
        total += end - start;
    }
    return total / (double)times;
}

double syscall_getpid_overhead(int times){
    double total = 0.;

    for (int i = 0; i < times; i++) {
        long long start = my_rdtsc();
        getpid();
        long long end = my_rdtsc();
        total += end - start;
    }
    return total / (double)times;
}

double create_process_overhead(int times){
    double total = 0.;

    for (int i = 0; i < times; i++) {
        long long start = my_rdtsc();
        int pid = fork();
        if (pid == 0) exit(0);
        wait(0);
        long long end = my_rdtsc();
        total += end - start;
    }
    return total / (double)times;

}

void* thread_function(void* a){
    return NULL;
}
double create_thread_overhead(int times){

    double total = 0.;
    pthread_t tid;

    for (int i = 0; i < times; i++) {
        long long start = my_rdtsc();
        pthread_create(&tid, (pthread_attr_t*)0, thread_function, 0);
        pthread_join(tid, 0);
        long long end = my_rdtsc();
        total += end - start;
    }
    return total / (double)times;
}

long long process_context_switch_once_overhead(){
    int fd[2];
    long long start, end;

    pipe(fd);
    if (fork() == 0) {
        end = my_rdtsc();
        write(fd[1], &end, sizeof(end));
        exit(0);
    }else{
        start = my_rdtsc();
        wait(0);
        read(fd[0], &end, sizeof(end));
    }
    return end - start;
}
