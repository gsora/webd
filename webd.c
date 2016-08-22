#include "webd.h"

int g_port;
struct addrinfo *server_info;
int socket_fd, new_fd;
struct sockaddr_storage input_addr;
char s[INET6_ADDRSTRLEN];
socklen_t sin_size;

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

	struct addrinfo *moment;
	for(moment = server_info; moment != NULL; moment = moment->ai_next) {
		// creating a socket
		socket_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
		if(socket_fd == -1) {
			fprintf(stderr, "error :: socket creation failed with error -> %s\n", strerror(errno));
			continue;
		}

		int k = 1;
		if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &k, sizeof(int)) == -1) {
			fprintf(stderr, "error :: setsockopt failed with error -> %s\n", strerror(errno));
			exit(1); 
		}

		// bind that socket to our port
		int bind_ret = bind(socket_fd, server_info->ai_addr, server_info->ai_addrlen);
		if(bind_ret == -1) {
			fprintf(stderr, "error :: bind failed with error -> %s\n", strerror(errno));
			continue;
		}

		break;
	}

	if(moment == NULL) {
		fprintf(stderr, "error :: failed to bind, gotta exit.\n");
		exit(1);
	}

	printf("status :: server setup completed, listening...\n");

	int listen_s = listen(socket_fd, BACKLOG);
	if(listen_s == -1) {
		fprintf(stderr, "error :: listen failed with error -> %s\n", strerror(errno));
		exit(1);
	}

	// done with server_info
	freeaddrinfo(server_info);

	// main loop
	while(1) {
		sin_size = sizeof input_addr;
		int new_fd = accept(socket_fd, (struct sockaddr *)&input_addr, &sin_size);

		if (new_fd == -1) {
			perror("accept");
			continue; 
		}

	inet_ntop(input_addr.ss_family, get_in_addr((struct sockaddr *)&input_addr), s, sizeof s);

		if (!fork()) {
			handle_connection(&new_fd);
		}
		close(new_fd);
	}

}

void handle_connection(int *fd) {

	printf("server: got connection from %s\n", s);

	if (send(*fd, "Hello, world!", 13, 0) == -1)
		perror("send");

	close(*fd);

	exit(0); 
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
	int length = snprintf(NULL, 0, "%d", g_port) + 2;
	char *portstr = malloc(length);
	snprintf(portstr, length, "%d", g_port);
	return portstr;
}
