#include <stdio.h>
#include <pcre.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define METHOD_SIZE 5
#define URL_SIZE 100
#define VERSION_SIZE 10

#define METHOD_SIZE_S "5"
#define URL_SIZE_S "100"
#define VERSION_SIZE_S "10"

#define PADDING 5

char *substr(const char *original_string, int l, int r) {
  if (original_string == NULL || l > r || l < 0 || r > strlen(original_string)) {
    return NULL;
  }
  int new_string_size = r - l + PADDING;
  char *new_string = calloc(new_string_size, sizeof(char));
  strncpy(new_string, original_string + l, r - l);
  return new_string;
}

char *prepend(const char *original_string, char prepend_char) {
  if (original_string == NULL)  {
    return NULL;
  }
  char *new_string = calloc(strlen(original_string) + PADDING, sizeof(char));
  sprintf(new_string, "%c%s", prepend_char, original_string);
  return new_string;
}

pcre *Pcre_compile(const char *pattern) {
  #undef SCOPE
  #define SCOPE "Pcre_compile: "

  const char *error;
  int error_offset;

  pcre *compile_result = pcre_compile(pattern, 0, &error, &error_offset, NULL);
  if (compile_result == NULL) {
    puts(SCOPE "compile error");
    puts(error);
  }
  return compile_result;
}

bool Pcre_match(const char *pattern, const char *target) {
  #undef SCOPE
  #define SCOPE "Pcre_exec: "
  
  const int OVECTOR_SIZE = 30;
  int ovector[OVECTOR_SIZE];

  pcre *compile_result = Pcre_compile(pattern);
  if (compile_result == NULL) {
    return false;
  }

  int exec_result = pcre_exec(compile_result, NULL, target, strlen(target), 0, 0, ovector, OVECTOR_SIZE);
  if (exec_result < 0 && exec_result != PCRE_ERROR_NOMATCH) {
    printf(SCOPE "error: target: %s\n", target);
    pcre_free(compile_result);
    return false;
  }
  if (exec_result == PCRE_ERROR_NOMATCH) {
    pcre_free(compile_result);
    return false;
  }
  pcre_free(compile_result);
  return true;
}

char *Pcre_capture(const char *pattern, const char *target) {
  assert(Pcre_match(pattern, target));

  #undef SCOPE
  #define SCOPE "Pcre_capture: "

  const int OVECTOR_SIZE = 30;
  int ovector[OVECTOR_SIZE];

  pcre *compile_result = Pcre_compile(pattern);
  pcre_exec(compile_result, NULL, target, strlen(target), 0, 0, ovector, OVECTOR_SIZE);
  pcre_free(compile_result);

  char *captured_string = substr(target, ovector[0], ovector[1]);
  if (captured_string == NULL) {
    puts(SCOPE "substr extraction error");
  }
  return captured_string;
}

bool is_method(const char *method) {
  return method != NULL && strcmp(method, "GET") == 0;
}

bool is_relative_url(const char *maybe_url) {
  if (maybe_url == NULL) {
    return false;
  }
  #undef RELATIVE
  #define RELATIVE "(((/\\w+)*/)|((/\\w+)+))(\\?(\\w+=\\w+;)*(\\w+=\\w+))?(#\\w*)?"
  return Pcre_match("^" RELATIVE "$", maybe_url);
}

bool is_absolute_url(const char *maybe_url) {
  if (maybe_url == NULL) {
    return false;
  }
  #undef ABSOLUTE
  #define ABSOLUTE "(https?://)?\\w+(\\.\\w+)+(:\\d+)?"
  #undef RELATIVE
  #define RELATIVE "(((/\\w+)*/)|((/\\w+)+))?(\\?(\\w+=\\w+;)*(\\w+=\\w+))?(#\\w*)?"
  return Pcre_match("^" ABSOLUTE "(" RELATIVE ")?$", maybe_url);
}

bool is_version(const char *maybe_version) {
  if (maybe_version == NULL) {
    return false;
  }
  #undef VERSION
  #define VERSION "HTTP/\\d+\\.\\d+"
  return Pcre_match("^" VERSION "$", maybe_version);
}

bool is_triple(const char *maybe_triple) {
  if (maybe_triple == NULL) {
    return false;
  }
  #undef TRIPLE
  #define TRIPLE "\\S{1," METHOD_SIZE_S "} \\S{1," URL_SIZE_S "} \\S{1," VERSION_SIZE_S "}"
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
  if (!is_triple(line)) {
    return false;
  }

  char *maybe_method = get_maybe_method(line);
  if (!is_method(maybe_method)) {
    return false;
  }
  free(maybe_method);

  char *maybe_url = get_maybe_url(line);
  if (!is_absolute_url(maybe_url)) {
    return false;
  }
  free(maybe_url);

  char *maybe_version = get_maybe_version(line);
  if (!is_version(maybe_version)) {
    return false;
  }
  free(maybe_version);

  return true;
}

char *absolute_to_relative(char *absolute_url) {
  assert(is_absolute_url(absolute_url));

  #undef SCOPE
  #define SCOPE "absolute_to_relative: "

  #undef RELATIVE
  #define RELATIVE "(((/\\w+)*/)|((/\\w+)+))?(\\?(\\w+=\\w+;)*(\\w+=\\w+))?(#\\w*)?$"

  char *protocol_ripped, *relative_url_;

  if (Pcre_match("//", absolute_url)) {
    protocol_ripped = Pcre_capture("(?<=//).*", absolute_url);
    relative_url_ = Pcre_capture(RELATIVE, protocol_ripped);
    free(protocol_ripped);
  }
  else {
    relative_url_ = Pcre_capture(RELATIVE, absolute_url);
  }

  if (Pcre_match("^/", relative_url_)) {
    return relative_url_;
  }

  char *relative_url = prepend(relative_url_, '/');
  free(relative_url_);
  if (relative_url == NULL) {
    puts(SCOPE "prepend error");
  }

  return relative_url;
}

// char *map_request_line(const char *line) {
//   #undef SCOPE
//   #define SCOPE "map_request_line: "

//   if (!is_request_line(line)) {
//     puts(SCOPE "not a request line");
//     return NULL;
//   }
  
//   char *absolute_url = get_maybe_url(line);
//   char *relative_url = absolute_to_relative(absolute_url);
//   char *new_line = request_line(relative_url);

//   return new_line;
// }