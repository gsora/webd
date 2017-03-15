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
      // new connection, increment
      handle_connection(&new_fd);
    }
    close(new_fd);
  }

}

// send_data sends data to the socket associated with the file descriptor
int send_data(int *fd, char *data) {
  int sVal = send(*fd, (const void*)data, strlen(data), 0);
  return sVal;
}

// recive recives data from the socket associated with the file descriptor
recv_data *recive(int *fd) {
  recv_data *session = (recv_data *)malloc(sizeof(recv_data));

  void *buf = (void *)malloc(3000);
  int recvd = recv(*fd, buf, 3000, 0);
  
  session->data = (char *)buf;
  session->recv_chars = recvd;

  free(buf);

  return session;
}

void handle_connection(int *fd) {
  printf("status :: got connection from %s\n", s);

  if(send_data(fd, "<h1>Hello, world!</h1>\n") == -1) {
    fprintf(stderr, "error :: send_data failed with error -> %s\n", strerror(errno));
  }

  recv_data *session = recive(fd);

  if(strlen(session->data) == 1) {
    fprintf(stdout, "status :: peer tried to send a message, but nothing was recv'd\n");
  } else {
    parameter_container *ret = split_headers(session->data);
    printf("status :: request type string -> %s\n", ret->http_request_header);
    printf("status :: request method -> %s\n", ret->http_method);
    printf("status :: request path -> %s\n", ret->request_path);
    printf("status :: headers ->\n");
    int i;
    for(i = 0; i < ret->size; i++) {
      printf("\theader :: %s: %s\n", ret->parameters[i]->param_name, ret->parameters[i]->content);
    }
  }

  close(*fd);
  exit(0); 
}

parameter_container *split_headers(char *raw_headers) {

  parameter_container *container = pc_new_empty_container();

  char *w_raw_headers = malloc(strlen(raw_headers) + 1);
  strncpy(w_raw_headers, raw_headers, strlen(raw_headers));

  char *working_copy = w_raw_headers, *end_str = w_raw_headers;
  //char *end_str;
  int i = 0, rn = 0, empty_flag = 0;

  //for(i = 0, working_copy = strtok_r(w_raw_headers, "\r\n", &end_str); working_copy && *working_copy; i++, working_copy = strtok_r(NULL, "\r\n", &end_str)) {
  //for(i = 0, working_copy = strsep(&w_raw_headers, "\r\n"); i++;) {
  while(working_copy != NULL) {
    strsep(&end_str, "\r\n");

    if(strcmp(working_copy, "") == 0) {
	rn++;
	empty_flag = 1;
    } else {
      rn = 0;
      empty_flag = 0;
    }

    if(rn == 2) {
      break;
    }
    
    if(strstr(working_copy, "HTTP/1.1")) { // if it's the HTTP header
      pc_write_http_request_header(container, working_copy);

      char *http_request_end;
      char *req_data;
      int k = 0;
      for(req_data = strtok_r(working_copy, " ", &http_request_end); http_request_end && *http_request_end; k++, req_data = strtok_r(NULL, " ", &http_request_end)) {
	if(k == 0) {
	  pc_write_http_method(container, req_data);
	} else {
	  pc_write_request_path(container, req_data);
	}
      }
    } else if(empty_flag != 1) { // it's an HTTP header
      char *header_part = working_copy;
      char *end_inner_str = working_copy;
      
      char *header_name;
      char *header_content;
      int a = 0;
      while(header_part != NULL) {
	strsep(&end_inner_str, ":");
	if(a == 0) {
	  header_name = strdup(header_part);
	} else {
	  header_content = strdup(header_part);
	}
	header_part = end_inner_str;
	a++;
      }
      
      pc_write_header(container, header_name, header_content, i);
      i++;
    }
    
    working_copy = end_str;
  }

  container->size = i;

  //free(w_raw_headers);
  return container;
}

struct addrinfo *setup_server() {
  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo; 

  memset(&hints, 0, sizeof hints);

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(NULL, intstr(g_port), &hints, &servinfo)) != 0) {
    fprintf(stderr, "error :: getaddrinfo error -> %s\n", gai_strerror(status));
    return NULL;
  }

  return servinfo;
}


