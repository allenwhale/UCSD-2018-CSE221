#include "utils.h"
#include "mem.h"
int main(){
	// printf("%f\n", memory_access_latency(4, 16, 10000));
	// for (int i = 4; i <= 18; i++ ) {
	//     double sum = 0;
	//     int times = 50;
	//     for (int k = 0; k < times + 4; k++) {
	//         double n = memory_access_latency(1 << i, 64, 10000);
	//         if (k > 4) sum += n;
	//     }
	//     printf("%f\n", sum / times);
	// }
	// printf("%f\n", memory_write_bandwidth(1 << 23, 10));
	// printf("%d\n", getpagesize());
	// printf("%f\n", memory_pagefault(8 << 10, 10));
    int times = 20;

    for (int j = 10; j <= 12; j += 2) {
        for (int i = 4; i <= 18; i++) {
            double sum = 0;
            for (int k = 0; k < times; k++) {
                int fd[2];
                pipe(fd);
                if (fork() == 0) {
                    double l = memory_access_latency(1 << i, 1 << j, max(10000, 2 << (i + 10 - j)));
                    write(fd[1], &l, sizeof(l));
                    exit(0);
                }else{
                    double l;
                    read(fd[0], &l, sizeof(l));
                    sum += l;
                }
                close(fd[0]); close(fd[1]);
            }
            printf("%d %d %f\n", i, j, sum / times);
        }
    }
    return 0;
}
