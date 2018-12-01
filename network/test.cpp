#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
using namespace std;
long long my_rdtsc() {
    struct timeval t;

    gettimeofday(&t, NULL);
    return (long long)t.tv_sec * 1000000ll + t.tv_usec;
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
    printf("%d\n", ret);
    close(clientfd);
    free(buf);
}

double round_trip_time(char const *host, int port, int times){
    char *msg = (char*)malloc(sizeof(char) * 56);
    char *buf = (char*)malloc(sizeof(char) * 56);

    memset(msg, 'a', sizeof(char) * 56);
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
        write(sockfd, msg, 56);
        read(sockfd, buf, 56);
    }
    long long end = my_rdtsc();
    free(msg);
    free(buf);
    return (double)(end - start) / times;
}
int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("rtt\n");
        return 1;
    }
    if (strcmp(argv[1], "echo") == 0) {
        int port = atoi(argv[2]);
        echo_server(port);
    }
    if (strcmp(argv[1], "rtt") == 0) {
        char const *host = argv[2];
        int port = atoi(argv[3]);
        printf("%s %d\n", host, port);
        double res = round_trip_time(host, port, 100);
        printf("%f\n", res);
    }
    return 0;
}
