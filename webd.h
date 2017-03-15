/* webd header */

#define BACKLOG 20

#include "utils.h"
#include "parameter_container.h"

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
