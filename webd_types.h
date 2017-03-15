typedef struct recv_data {
	char *data;
	int recv_chars;
} recv_data;

typedef struct parameter {
	char *param_name;
	char * content;
} parameter;

typedef struct parameter_container {
	char *http_request_header;
	char *http_method;
	char *request_path;
	int size;
	parameter **parameters;
} parameter_container;
