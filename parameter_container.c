#include "parameter_container.h"

void pc_write_http_request_header(parameter_container *pc, char *request_header) {
  // copy the request header in the appropriate location
  pc->http_request_header = strdup(request_header);
}

void pc_write_request_path(parameter_container *pc, char *request_path) {
  // copy the request path in the appropriate location
  pc->request_path = strdup(request_path);
}

void pc_write_http_method(parameter_container *pc, char *http_method) {
  // copy the http method in the appropriate location
  pc->http_method = strdup(http_method);
}

void pc_write_header(parameter_container *pc, char *header_name, char *header_content, int index) {
  //reallocate the parameters array to accomodate another entry
  pc->parameters = realloc(pc->parameters, (sizeof(parameter) * sizeof(pc->parameters) + 1));

  // allocate space for a parameter struct in the newly-allocated space
  pc->parameters[index] = malloc(sizeof(parameter));

  // create a new empty parameter
  parameter *p = pc_new_empty_parameter();

  // write the parameter's name
  pc_parameter_write_param_name(p, header_name);

  // write the parameter's content
  pc_parameter_write_content(p, header_content);

  // write the newly-created parameter inside the container
  pc->parameters[index] = p;
}

/*parameter_container *pc_split_headers(char *headers) {

  }*/

parameter_container *pc_new_empty_container() {
  // initialize a parameter_container on the size of its content
  parameter_container *c = malloc(sizeof(parameter_container));

  // initialize the struct to the size of its inner content
  c->parameters = malloc(sizeof(parameter));

  // return the initialized parameter_container
  return c;
}

parameter *pc_new_empty_parameter() {
  // initialize a parameter p to its size
  parameter *p = malloc(sizeof(parameter));

  // return it
  return p;
}

void pc_parameter_write_param_name(parameter *p, char *param_name) {
  p->param_name = strdup(param_name);
}

void pc_parameter_write_content(parameter *p, char *content) {
  p->content = strdup(content);
}
