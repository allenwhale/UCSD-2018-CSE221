#include "utils.h"
#include "network.h"

void usage(){
    printf("./network echo <port>\n");
    printf("./network rtt <host> <port> <bytes> <times>\n");
    printf("./network bw_rd <port> <bytes>\n");
    printf("./network bw_wr <host> <port> <bytes> <times>\n");
    printf("./network setup_teardown <host> <port> <times>\n");
    printf("./network multi <port>\n");
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        usage();
        return 1;
    }
    if (strcmp(argv[1], "echo") == 0) {
        int port = atoi(argv[2]);
        echo_server(port);
    }else if (strcmp(argv[1], "rtt") == 0) {
        char const *host = argv[2];
        int port = atoi(argv[3]);
        int bytes = atoi(argv[4]);
        int times = atoi(argv[5]);
        double res = round_trip_time(host, port, bytes, times);
        printf("%f\n", res);
    }else if (strcmp(argv[1], "bw_wr") == 0) {
        char const *host = argv[2];
        int port = atoi(argv[3]);
        int bytes = atoi(argv[4]);
        int times = atoi(argv[5]);
        double res = bandwidth_write(host, port, bytes, times);
        printf("W %f\n", res);
    }else if (strcmp(argv[1], "bw_rd") == 0) {
        int port = atoi(argv[2]);
        int bytes = atoi(argv[3]);
        double res = bandwidth_read(port, bytes);
        printf("R %f\n", res);
    }else if (strcmp(argv[1], "setup_teardown") == 0) {
        char const *host = argv[2];
        int port = atoi(argv[3]);
        int times = atoi(argv[4]);
        setup_teardown res = setup_teardown_overhead(host, port, times);
        printf("setup=%f teardown=%f\n", res.setup, res.teardown);
    }else if (strcmp(argv[1], "multi") == 0) {
        int port = atoi(argv[2]);
        multi_connection_server(port);
    }else{
        usage();
    }
    return 0;
}
