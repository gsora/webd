/* webd header */

#define BACKLOG 20

#include "utils.h"

typedef struct recv_data {
	char *data;
	int recv_chars;
} recv_data;

typedef struct parameter {
	char *param_name;
	char * content;
} parameter;

typedef struct parameter_container {
	char *http_header;
	int size;
	parameter **parameters;
} parameter_container;

// socket headers
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

struct addrinfo *setup_server();
void handle_connection(int *);
int send_data(int *, char *);
recv_data *recive(int *);
parameter_container *split_headers(char *);
