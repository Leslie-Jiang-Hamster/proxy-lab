#include <stdio.h>
#include <pcre.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "parse.h"

char *substr(const char *original_string, int l, int r) {
  assert(original_string != NULL);
  assert(0 <= l && l <= r && r <= strlen(original_string));

  int new_string_size = r - l + PADDING;
  char *new_string = calloc(new_string_size, sizeof(char));
  strncpy(new_string, original_string + l, r - l);
  return new_string;
}

pcre *Pcre_compile(const char *pattern) {
  #define SCOPE "Pcre_compile: "

  const char *error;
  int error_offset;

  pcre *compile_result = pcre_compile(pattern, 0, &error, &error_offset, NULL);
  // if (compile_result == NULL) {
  //   puts(SCOPE "compile error");
  //   puts(error);
  // }
  assert(compile_result != NULL);
  return compile_result;
  #undef SCOPE
}

bool Pcre_match(const char *pattern, const char *target) {
  #define SCOPE "Pcre_exec: "
  
  const int OVECTOR_SIZE = 30;
  int ovector[OVECTOR_SIZE];

  pcre *compile_result = Pcre_compile(pattern);
  if (compile_result == NULL) {
    return false;
  }

  int exec_result = pcre_exec(compile_result, NULL, target, strlen(target), 0, 0, ovector, OVECTOR_SIZE);
  pcre_free(compile_result);
  if (exec_result < 0 && exec_result != PCRE_ERROR_NOMATCH) {
    printf(SCOPE "error: target: %s\n", target);
    return false;
  }
  if (exec_result == PCRE_ERROR_NOMATCH) {
    return false;
  }
  return true;
  #undef SCOPE
}

char *Pcre_capture(const char *pattern, const char *target) {
  assert(Pcre_match(pattern, target));

  #define SCOPE "Pcre_capture: "

  const int OVECTOR_SIZE = 30;
  int ovector[OVECTOR_SIZE];

  pcre *compile_result = Pcre_compile(pattern);
  pcre_exec(compile_result, NULL, target, strlen(target), 0, 0, ovector, OVECTOR_SIZE);
  pcre_free(compile_result);

  char *captured_string = substr(target, ovector[0], ovector[1]);

  return captured_string;
  #undef SCOPE
}

bool is_method(const char *method) {
  if (method == NULL) {
    return false;
  }
  if (strcmp(method, "GET") == 0) {
    return true;
  }
  return false;
}

bool is_relative_url(const char *maybe_url) {
  if (maybe_url == NULL) {
    return false;
  }
  return Pcre_match("^" RELATIVE_STRICT "$", maybe_url);
}

bool is_absolute_url(const char *maybe_url) {
  if (maybe_url == NULL) {
    return false;
  }
  return Pcre_match("^" ABSOLUTE "(" RELATIVE ")?$", maybe_url);
}

bool is_version(const char *maybe_version) {
  if (maybe_version == NULL) {
    return false;
  }
  return Pcre_match("^" VERSION "$", maybe_version);
}

bool is_triple(const char *maybe_triple) {
  if (maybe_triple == NULL) {
    return false;
  }
  return Pcre_match("^" TRIPLE "$", maybe_triple);
}

char *get_maybe_method(const char *triple) {
  assert(is_triple(triple));

  char *maybe_method = calloc(METHOD_SIZE + PADDING, sizeof(char));
  sscanf(triple, "%s %*s %*s", maybe_method);
  return maybe_method;
}

char *get_maybe_url(const char *triple) {
  assert(is_triple(triple));

  char *maybe_url = calloc(URL_SIZE + PADDING, sizeof(char));
  sscanf(triple, "%*s %s %*s", maybe_url);
  return maybe_url;
}

char *get_maybe_version(const char *triple) {
  assert(is_triple(triple));

  char *maybe_version = calloc(VERSION_SIZE + PADDING, sizeof(char));
  sscanf(triple, "%*s %*s %s", maybe_version);
  return maybe_version;
}

bool is_request_line(const char *line) {
  assert(is_line(line));

  if (!is_triple(line)) {
    return false;
  }

  char *maybe_method = get_maybe_method(line);
  if (!is_method(maybe_method)) {
    free(maybe_method);
    return false;
  }
  free(maybe_method);

  char *maybe_url = get_maybe_url(line);
  if (!is_absolute_url(maybe_url)) {
    free(maybe_url);
    return false;
  }
  free(maybe_url);

  char *maybe_version = get_maybe_version(line);
  if (!is_version(maybe_version)) {
    free(maybe_version);
    return false;
  }
  free(maybe_version);

  return true;
}

char *absolute_to_relative(char *absolute_url) {
  assert(is_absolute_url(absolute_url));

  char *protocol_ripped;

  if (Pcre_match("//", absolute_url)) {
    protocol_ripped = Pcre_capture("(?<=//).*", absolute_url);
  }
  else {
    protocol_ripped = strdup(absolute_url);
  }

  assert(protocol_ripped != NULL);

  char *relative_url = Pcre_capture(RELATIVE "$", protocol_ripped);
  free(protocol_ripped);

  assert(relative_url != NULL);

  if (Pcre_match("^/", relative_url)) {
    assert(is_relative_url(relative_url));
    return relative_url;
  }

  char *relative_url_ = concat("/", relative_url);
  free(relative_url);
  assert(is_relative_url(relative_url_));
  return relative_url_;
}

char *map_request_line(const char *line) {
  assert(is_request_line(line));
  
  char *absolute_url = get_maybe_url(line);
  char *relative_url = absolute_to_relative(absolute_url);
  free(absolute_url);
  char *new_line = make_request_line(relative_url);
  free(relative_url);

  return new_line;
}

char *make_request_line(const char *relative_url) {
  assert(is_relative_url(relative_url));

  char *request_line = calloc(LINE_SIZE + PADDING, strlen(relative_url));
  sprintf(request_line, "GET %s HTTP/1.0", relative_url);

  return request_line;
}

char *get_hostname(const char *absolute_url) {
  assert(is_absolute_url(absolute_url));

  return Pcre_capture(HOSTNAME, absolute_url);
}

char *concat(const char *string_1, const char *string_2) {
  if (string_1 == NULL || string_2 == NULL) {
    return NULL;
  }

  const int new_string_size = strlen(string_1) + strlen(string_2) + PADDING;
  char *new_string = calloc(new_string_size, sizeof(char));
  sprintf(new_string, "%s%s", string_1, string_2);
  return new_string;
}

bool is_line(const char *string) {
  if (string == NULL) {
    return false;
  }
  return !Pcre_match("\r\n", string);
}

bool is_header(const char *string) {
  assert(string != NULL);
  assert(is_line(string));

  if (Pcre_match("^" HEADER "$", string)) {
    return true;
  }
  return false;
}

char *get_header_key(const char *header) {
  assert(is_header(header));

  return Pcre_capture("^[A-Za-z-]+(?=: )", header);
}

char *get_header_value(const char *header) {
  assert(is_header(header));

  return Pcre_capture("(?<=: ).*$", header);
}

char *make_header(const char *key, const char *value) {
  assert(Pcre_match("^[A-Z][A-Za-z-]+[a-z]$", key));

  char *header = calloc(LINE_SIZE + PADDING, sizeof(char));
  sprintf(header, "%s: %s", key, value);
  return header;
}

bool can_proxy_modify_key(const char *key) {
  assert(Pcre_match("^[A-Z][A-Za-z-]+[a-z]$", key));

  if (strcmp(key, "User-Agent") == 0) {
    return true;
  }
  if (strcmp(key, "Connection") == 0) {
    return true;
  }
  if (strcmp(key, "Proxy-Connection") == 0) {
    return true;
  }
  return false;
}

char *map_header(const char *header) {
  assert(is_header(header));

  char *key = get_header_key(header);
  if (!can_proxy_modify_key(key)) {
    free(key);
    return strdup(header);
  }
  if (strcmp(key, "User-Agent") == 0) {
    free(key);
    return make_header("User-Agent", UA_DEFAULT);
  }
  if (strcmp(key, "Connection") == 0) {
    free(key);
    return make_header("Connection", "close");
  }
  if (strcmp(key, "Proxy-Connection") == 0) {
    free(key);
    return make_header("Proxy-Connection", "close");
  }
  // control never reaches here
  assert(false);
  free(key);
  return NULL;
}

int get_port(const char *absolute_url) {
  assert(is_absolute_url(absolute_url));
  if (!Pcre_match("(?<=:)\\d+", absolute_url)) {
    return HTTP_PORT_DEFAULT;
  }
  char *port_string = Pcre_capture("(?<=:)\\d+", absolute_url);
  int port = atoi(port_string);
  free(port_string);
  return port;
}