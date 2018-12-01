#include "network.h"
#include "utils.h"

int inet_addr(const char *host){
    int a, b, c, d;

    sscanf(host, "%d.%d.%d.%d", &a, &b, &c, &d);
    return htonl((a << 24) | (b << 16) | (c << 8) | d);
}

void echo_server(int port){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("socket");
        return;
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return;
    }
    if (listen(sockfd, 10) < 0) {
        perror("listen");
        return;
    }
    int clientfd;
    socklen_t slen = sizeof(sin);
    if ((clientfd = accept(sockfd, (struct sockaddr *)&sin, &slen)) < 0) {
        perror("client");
        return;
    }
    close(sockfd);
    char *buf = (char*)malloc(sizeof(char) * 100);
    int ret;
    while ((ret = read(clientfd, buf, 100)) > 0) {
        write(clientfd, buf, ret * sizeof(char));
    }
    close(clientfd);
    free(buf);
}

double round_trip_time(char const *host, int port, int bytes, int times){
    char *msg = (char*)malloc(sizeof(char) * bytes);
    char *buf = (char*)malloc(sizeof(char) * bytes);

    memset(msg, 'a', sizeof(char) * bytes);
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 0;
    }
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        return 0;
    }

    long long start = my_rdtsc();
    for (int i = 0; i < times; i++) {
        write(sockfd, msg, bytes);
        read(sockfd, buf, bytes);
    }
    long long end = my_rdtsc();
    free(msg);
    free(buf);
    return (double)(end - start) / times;
}
double bandwidth_write(const char *host, int port, int bytes, int times){
    struct sockaddr_in server;

    server.sin_addr.s_addr = inet_addr(host);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 0;
    }
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        return 0;
    }
    char *msg = (char*)malloc(sizeof(char) * bytes);
    memset(msg, 'a', sizeof(char) * bytes);
    long long start = my_rdtsc();
    for (int i = 0; i < times; i++) {
        write(sockfd, msg, sizeof(char) * bytes);
    }
    long long end = my_rdtsc();
    free(msg);
    close(sockfd);
    return (double)(bytes * times) / (end - start);
}
double bandwidth_read(int port, int bytes){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("socket");
        return 0;
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return 0;
    }
    if (listen(sockfd, 10) < 0) {
        perror("listen");
        return 0;
    }
    int clientfd;
    socklen_t slen = sizeof(sin);
    if ((clientfd = accept(sockfd, (struct sockaddr *)&sin, &slen)) < 0) {
        perror("client");
        return 0;
    }
    close(sockfd);
    char *buf = (char*)malloc(sizeof(char) * 100);
    int ret, sum = 0;
    long long start = my_rdtsc();
    while ((ret = read(clientfd, buf, 100)) > 0) {
        sum += ret;
    }
    long long end = my_rdtsc();
    close(clientfd);
    free(buf);
    return (double)sum / (end - start);
}
void multi_connection_server(int port){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("socket");
        return;
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return;
    }
    if (listen(sockfd, 10) < 0) {
        perror("listen");
        return;
    }
    int clientfd;
    socklen_t slen = sizeof(sin);
    while ((clientfd = accept(sockfd, (struct sockaddr *)&sin, &slen)) > 0);
    close(sockfd);
}
setup_teardown setup_teardown_overhead(const char *host, int port, int times){
    int *fds = (int*)malloc(sizeof(int) * times);
    int *connect_ret = (int*)malloc(sizeof(int) * times);
    int *close_ret = (int*)malloc(sizeof(int) * times);

    struct sockaddr_in server;

    server.sin_addr.s_addr = inet_addr(host);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    long long start, end;
    start = my_rdtsc();
    for (int i = 0; i < times; i++) {
        fds[i] = socket(AF_INET, SOCK_STREAM, 0);
        connect_ret[i] = connect(fds[i], (struct sockaddr *)&server, sizeof(server));
    }
    end = my_rdtsc();
    double setup = (double)(end - start) / times;
    start = my_rdtsc();
    for (int i = 0; i < times; i++) {
        close_ret[i] = close(fds[i]);
    }
    end = my_rdtsc();
    for (int i = 0; i < times; i++) {
        if (connect_ret[i] < 0) perror("connect");
        if (close_ret[i] < 0) perror("close");
    }
    double teardown = (double)(end - start) / times;
    setup_teardown ret;
    ret.setup = setup;
    ret.teardown = teardown;
    return ret;
}
