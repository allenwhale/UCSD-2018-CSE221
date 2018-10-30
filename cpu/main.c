#include "cpu.h"
#include "utils.h"

int main(void){
    
    for (int i = 0; i < 104; ++i) {
        printf("%f\n", thread_context_switch_overhead(100));
    }
    return 0;
}
