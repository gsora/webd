#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int detect_port_arg(int argc, char **argv);
void *get_in_addr(struct sockaddr *sa); 
char *intstr(int);
char *trim_first_n_chars(char *str, int n);
