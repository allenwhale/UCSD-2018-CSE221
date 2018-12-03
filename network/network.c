#include "network.h"
#include "utils.h"

int inet_addr(const char *host) {
  int a, b, c, d;

  sscanf(host, "%d.%d.%d.%d", &a, &b, &c, &d);
  return htonl((a << 24) | (b << 16) | (c << 8) | d);
}

void echo_server(int port) {
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
  char *buf = (char *)malloc(sizeof(char) << 20);
  while ((clientfd = accept(sockfd, (struct sockaddr *)&sin, &slen)) > 0) {
    int ret;
    while ((ret = read(clientfd, buf, 1 << 20)) > 0) {
      write(clientfd, buf, ret * sizeof(char));
    }
    close(clientfd);
  }
  close(sockfd);
  free(buf);
}

double round_trip_time(char const *host, int port, int bytes, int times) {
  char *msg = (char *)malloc(sizeof(char) * bytes);
  char *buf = (char *)malloc(sizeof(char) * bytes);

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
  long long *overhead = (long long *)malloc(sizeof(long long) * (times + 20));
  for (int i = 0; i < times + 20; i++) {
    long long start = my_rdtsc();
    write(sockfd, msg, bytes);
    read(sockfd, buf, bytes);
    long long end = my_rdtsc();
    overhead[i] = end - start;
  }
  qsort(overhead, times + 20, sizeof(long long), cmp_ll);
  double res = 0;
  for (int i = 10; i < times + 10; i++) {
    res += (double)overhead[i] / times;
  }
  free(overhead);
  free(msg);
  free(buf);
  return res;
}
double bandwidth_write(const char *host, int port, int bytes, int times) {
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
  char *msg = (char *)malloc(sizeof(char) * bytes);
  memset(msg, 'a', sizeof(char) * bytes);

  long long start = my_rdtsc();
  for (int i = 0; i < times; i++) {
    write(sockfd, msg, sizeof(char) * bytes);
  }
  close(sockfd);
  long long end = my_rdtsc();
  free(msg);

  return (double)(bytes * times) / (double)(end - start);
}
double bandwidth_read(int port, int bytes) {
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
  char *buf = (char *)malloc(sizeof(char) * bytes);
  int ret, sum = 0;
  long long start = my_rdtsc();
  while ((ret = read(clientfd, buf, bytes)) > 0) {
    sum += ret;
  }
  long long end = my_rdtsc();
  close(clientfd);
  free(buf);
  return (double)sum / (end - start);
}
void multi_connection_server(int port) {
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
  while ((clientfd = accept(sockfd, (struct sockaddr *)&sin, &slen)) > 0) {
    usleep(500);
    close(clientfd);
  }
  // ;
  close(sockfd);
}
setup_teardown setup_teardown_overhead(const char *host, int port, int times) {
  int *fds = (int *)malloc(sizeof(int) * (times + 20));
  int *connect_ret = (int *)malloc(sizeof(int) * (times + 20));
  int *close_ret = (int *)malloc(sizeof(int) * (times + 20));
  long long *overhead_setup =
      (long long *)malloc(sizeof(long long) * (times + 20));
  long long *overhead_teardown =
      (long long *)malloc(sizeof(long long) * (times + 20));
  char buf[10];

  struct sockaddr_in server;

  server.sin_addr.s_addr = inet_addr(host);
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  long long start, end;
  for (int i = 0; i < times + 20; i++) {
    start = my_rdtsc();
    fds[i] = socket(AF_INET, SOCK_STREAM, 0);
    connect_ret[i] =
        connect(fds[i], (struct sockaddr *)&server, sizeof(server));
    end = my_rdtsc();
    overhead_setup[i] = end - start;
    start = my_rdtsc();
    while (read(fds[i], buf, 10) > 0)
      ;
    end = my_rdtsc();
    overhead_teardown[i] = end - start - 500;
  }
  qsort(overhead_setup, times + 20, sizeof(long long), cmp_ll);
  double setup = 0;
  for (int i = 10; i < times + 10; i++) {
    setup += (double)overhead_setup[i] / times;
  }
  qsort(overhead_teardown, times + 20, sizeof(long long), cmp_ll);
  double teardown = 0;
  for (int i = 10; i < times + 10; i++) {
    teardown += (double)overhead_teardown[i] / times;
  }
  setup_teardown ret;
  ret.setup = setup;
  ret.teardown = teardown;
  free(fds);
  free(connect_ret);
  free(close_ret);
  free(overhead_setup);
  free(overhead_teardown);
  return ret;
}
