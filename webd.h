/* webd header */

#define BACKLOG 20

#include "utils.h"

// socket headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

extern int g_port;
extern struct addrinfo *server_info;
extern int socket_fd;

struct addrinfo *setup_server();
char *port_str();
