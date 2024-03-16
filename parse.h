#include <stdbool.h>

#define METHOD_SIZE 5
#define URL_SIZE 100
#define VERSION_SIZE 10
#define PADDING 5
#define LINE_SIZE (METHOD_SIZE + URL_SIZE + VERSION_SIZE)

#define METHOD_SIZE_S "5"
#define URL_SIZE_S "100"
#define VERSION_SIZE_S "10"

#define ABSOLUTE "(https?://)?\\w+(\\.\\w+)+(:\\d+)?"
#define RELATIVE "(/\\w+)*/?((?<=/)([\\w\\.]+))?(\\?(\\w+=\\w+\\&)*(\\w+=\\w+))?(#\\w*)?"
#define RELATIVE_STRICT "(/\\w+)*/((?<=/)([\\w\\.]+))?(\\?(\\w+=\\w+\\&)*(\\w+=\\w+))?(#\\w*)?"

char *substr(const char *original_string, int l, int r);
bool is_method(const char *method);
bool is_relative_url(const char *maybe_url);
bool is_absolute_url(const char *maybe_url);
bool is_version(const char *maybe_version);
bool is_triple(const char *maybe_triple);
char *get_maybe_method(const char *triple);
char *get_maybe_url(const char *triple);
char *get_maybe_version(const char *triple);
bool is_request_line(const char *line);
char *Pcre_capture(const char *pattern, const char *target);
char *absolute_to_relative(char *absolute_url);
char *make_request_line(const char *relative_url);
char *get_hostname(const char *absolute_url);
char *map_request_line(const char *line);
char *concat(const char *string_1, const char *string_2);