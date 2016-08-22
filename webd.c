#include "webd.h"

int g_port;
struct addrinfo *server_info;
int socket_fd;

int main(int argc, char **argv) {
	// detect if a port number was passed as "--port" parameter
	// if yes, cast it as int and set it to the global "port" variable
	g_port = detect_port_arg(argc, argv);

	printf("status :: webd started, listening on port %d\n", g_port);
	printf("status :: setup server ongoing...\n");
	server_info = setup_server();

	if(server_info == NULL) {
		exit(1);
	}

	// creating a socket
	socket_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	if(socket_fd == -1) {
		fprintf(stderr, "error :: socket creation failed with error -> %s\n", strerror(errno));
	}
	
	// bind that socket to our port
	int bind_ret = bind(socket_fd, server_info->ai_addr, server_info->ai_addrlen);
	if(bind_ret == -1) {
		fprintf(stderr, "error :: bind failed with error -> %s\n", strerror(errno));
	}
	
	printf("status :: server setup completed, listening...\n");

	listen(socket_fd, BACKLOG);
	

}

struct addrinfo *setup_server() {
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo; 

	memset(&hints, 0, sizeof hints);
	
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if ((status = getaddrinfo(NULL, port_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "error :: getaddrinfo error -> %s\n", gai_strerror(status));
		return NULL;
	}

	return servinfo;
}

char *port_str() {
	int length = snprintf(NULL, 0, "%d", g_port);
	char *portstr = malloc(length + 1);
	snprintf(portstr, length, "%d", g_port);
	return portstr;
}
