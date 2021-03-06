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

  // create a new empty parameter
  parameter *p = pc_new_empty_parameter();

  // write the parameter's name
  pc_parameter_write_param_name(p, header_name);

  // write the parameter's content
  pc_parameter_write_content(p, header_content);

  // write the newly-created parameter inside the container
  pc->parameters[index] = p;
}

parameter_container *pc_split_headers(char *headers) {

  // initialize an empty container
  parameter_container *container = pc_new_empty_container();

  // create a copy of the headers
  char *w_raw_headers = malloc(strlen(headers) + 1);
  strncpy(w_raw_headers, headers, strlen(headers));

  // initialize all the needed variables to iterate through the string
  // using strsep
  char *working_copy = w_raw_headers, *end_str = w_raw_headers;
  int header_number = 0, rn = 0, empty_flag = 0;

  while(working_copy != NULL) {
    // split the line
    strsep(&end_str, "\r\n");

    // if it's an empty line, increment rn and set empty_flag to 1
    //
    // empty_flag will be used to see whether to split or not, otherwise
    // just reset rn and empty_flag to 0
    if(strcmp(working_copy, "") == 0) {
	rn++;
	empty_flag = 1;
    } else {
      rn = 0;
      empty_flag = 0;
    }

    // if we found 2 consecutive empty lines, exit: we found the http request body
    if(rn == 2) {
      break;
    }

    // if the current line is the first of an http request, save it to the appropriate
    // location in the container
    if(strstr(working_copy, "HTTP/1.1")) {
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
      
    } else if(empty_flag != 1) { // otherwise, do the same for an header part
      char *header_part = working_copy;
      char *end_inner_str = working_copy;
      
      char *header_name = NULL;
      char *header_content = NULL;
      int a = 0;
      while(header_part != NULL) {
	strsep(&end_inner_str, ":");
	if(a == 0) {
	  if(header_name == NULL) {
	    header_name = strdup(header_part);
	  } else {
	    strcat(header_name, header_part);
	  }
	} else {
	  if(header_content == NULL) {
	    header_content = strdup(header_part);
	  } else {
	    strcat(header_content, header_part);
	  }

	}
	header_part = end_inner_str;
	a++;
      }
      
      pc_write_header(container, header_name, header_content, header_number);
      header_number++;

      free(header_name);
      free(header_content);
    }
    
    working_copy = end_str;
  }

  container->size = header_number;

  free(w_raw_headers);
  
  return container;

}

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
  parameter *p = malloc(sizeof(parameter *));

  // return it
  return p;
}

void pc_parameter_write_param_name(parameter *p, char *param_name) {
  p->param_name = strdup(param_name);
}

void pc_parameter_write_content(parameter *p, char *content) {
  p->content = strdup(content);
}

void pc_free(parameter_container *pc) {
  free(pc->http_request_header);
  free(pc->http_method);
  free(pc->request_path);

  int i = 0;
  for(; i != pc->size; i++) {
    free(pc->parameters[i]->param_name);
    free(pc->parameters[i]->content);
    free(pc->parameters[i]);
  }

  free(pc->parameters);
  free(pc);
}
