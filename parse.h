#include <stdbool.h>

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
char *prepend(const char *original_string, char prepend_char);
char *absolute_to_relative(char *absolute_url);