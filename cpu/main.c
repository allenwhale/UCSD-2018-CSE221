#include "cpu.h"
#include "utils.h"

int main(void){
    printf("%f\n", loop_overhead(100000));
    printf("%f\n", measure_overhead(100000));
    double ans[10];
    procedure_call_overhead(100000, ans);
    for (int i = 0; i < 7; i++)
        printf("%f ", ans[i]);
    printf("\n");
    printf("%f\n", syscall_time_overhead(100000));
    printf("%f\n", syscall_getpid_overhead(100000));
    printf("%f\n", create_process_overhead(1000));
    printf("%f\n", create_thread_overhead(1000));
    printf("%f\n", process_context_switch_overhead(1000));
    printf("%f\n", thread_context_switch_overhead(1000));
    return 0;
}
