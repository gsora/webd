#include "utils.h"

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

