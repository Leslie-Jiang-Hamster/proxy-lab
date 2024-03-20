#include <stdbool.h>

// number constants
#define METHOD_SIZE 5
#define URL_SIZE 100
#define VERSION_SIZE 10
#define PADDING 5
#define LINE_SIZE (METHOD_SIZE + URL_SIZE + VERSION_SIZE)
#define HTTP_PORT_DEFAULT 80

// string constants
#define METHOD_SIZE_S "5"
#define URL_SIZE_S "100"
#define VERSION_SIZE_S "10"
#define UA_DEFAULT "Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3"

// regex patterns
#define ABSOLUTE "(https?://)?" HOSTNAME "(:\\d+)?"
#define RELATIVE "(/\\w+)*/?((?<=/)([\\w\\.-]+))?(\\?(\\w+(=\\w+)?\\&)*(\\w+(=\\w+)?))?(#\\w*)?"
#define RELATIVE_STRICT "(/\\w+)*/((?<=/)([\\w\\.-]+))?(\\?(\\w+(=\\w+)?\\&)*(\\w+(=\\w+)?))?(#\\w*)?"
#define HOSTNAME "(\\w+(\\.\\w+)+|localhost)"
#define TRIPLE "\\S{1," METHOD_SIZE_S "} \\S{1," URL_SIZE_S "} \\S{1," VERSION_SIZE_S "}"
#define VERSION "HTTP/\\d+\\.\\d+"
#define HEADER "[A-Z][A-Za-z-]+: .*"

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
bool is_line(const char *string);
bool is_header(const char *string);
char *get_header_key(const char *header);
char *get_header_value(const char *header);
char *make_header(const char *key, const char *value);
bool can_proxy_modify_key(const char *key);
char *map_header(const char *header);
int get_port(const char *absolute_url);
bool is_rn_line(char *maybe_rn_line);
char *rn_line_to_line(char *rn_line);
char *line_to_rn_line(char *line);