#include "webd_types.h"
#include <stdlib.h>
#include <string.h>

void pc_write_http_request_header(parameter_container *, char *);
void pc_write_request_path(parameter_container *, char *);
void pc_write_http_method(parameter_container *, char *);
void pc_write_header(parameter_container *, char *, char *, int);

parameter_container *pc_new_empty_container();
//parameter_container *pc_split_headers(char *);
parameter_container *split_headers(char *);

parameter *pc_new_empty_parameter();
void pc_parameter_write_param_name(parameter *, char *);
void pc_parameter_write_content(parameter *, char *);
