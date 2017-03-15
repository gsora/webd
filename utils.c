#include "utils.h"

void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// detects what port was passed with the --port parameter
int detect_port_arg(int argc, char **argv) {
	int cycle;
	int g_port = 8080;
	for(cycle = 0; cycle < argc; cycle++) {
		if(strncmp(argv[cycle], "--port", 6) == 0) {
			// if we got a --port but no argument, break
			if(cycle+1 >= argc) {
				break;
			}

			g_port = atoi(argv[cycle+1]);

			if(g_port == 0) {		
				// did not got a valid port number, set global port to 8080
				g_port = 8080;	
			}
			break;
		}
	}

	return g_port;
}

char *intstr(int i) {
	int length = snprintf(NULL, 0, "%d", i) + 2;
	char *portstr = (char *)malloc(length);
	snprintf(portstr, length, "%d", i);
	return portstr;
}

char *trim_first_n_chars(char *str, int n) {
  if((int)strlen(str) < n) {
    return NULL;
  }
  char *dest = malloc((strlen(str) - n) * sizeof(char));
  sprintf(dest, "%.*s", (int)(n-strlen(str)), str+n);
  return dest;
}
