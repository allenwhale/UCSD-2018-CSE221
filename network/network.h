#ifndef __NETWORK__H__
#define __NETWORK__H__
int inet_addr(const char *);
double round_trip_time(char const *, int, int, int);
void echo_server(int);
double bandwidth_write(const char *, int, int, int);
double bandwidth_read(int, int);
void multi_connection_server(int);
typedef struct {
  double setup, teardown;
} setup_teardown;
setup_teardown setup_teardown_overhead(const char *, int, int);
#endif
