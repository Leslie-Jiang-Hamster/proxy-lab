#include "parse.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void TEST_is_method() {
  #undef SCOPE
  #define SCOPE "is_method"

  puts("\nTesting: " SCOPE);

  assert(is_method("GET") == true);
  assert(is_method("get") == false);
  assert(is_method("") == false);
  assert(is_method(NULL) == false);

  puts("All passed: " SCOPE);
}

void TEST_is_relative_url() {
  #undef SCOPE
  #define SCOPE "is_relative_url"

  puts("\nTesting: " SCOPE);

  assert(is_relative_url("123") == false);
  assert(is_relative_url("/") == true);
  assert(is_relative_url("(/)") == false);
  assert(is_relative_url("/foo") == true);
  assert(is_relative_url("/foo/bar/") == true);
  assert(is_relative_url("/foo/?bar=baz") == true);
  assert(is_relative_url("/foo/bar=baz") == false);
  assert(is_relative_url("foo/bar") == false);
  assert(is_relative_url("/foo/bar?baz=123#") == true);
  assert(is_relative_url("/foo/bar#") == true);
  assert(is_relative_url("/foo/bar?baz=123;nox=456#") == true);
  assert(is_relative_url("/foo/bar?baz=123#nox=456") == false);
  assert(is_relative_url("/foo/bar?baz=123http://foo.bar") == false);
  assert(is_relative_url("") == false);
  assert(is_relative_url(NULL) == false);

  puts("All passed: " SCOPE);
}

void TEST_is_absolute_url() {
  #undef SCOPE
  #define SCOPE "is_absolute_url"

  puts("\nTesting: " SCOPE);

  assert(is_absolute_url("foobar") == false);
  assert(is_absolute_url("http://foobar") == false);
  assert(is_absolute_url("foo.bar") == true);
  assert(is_absolute_url("foo.bar/baz") == true);
  assert(is_absolute_url("foo.bar/?foo=bar;baz=nox#fragment") == true);
  assert(is_absolute_url("https://foo.bar") == true);
  assert(is_absolute_url("foo.bar.baz") == true);
  assert(is_absolute_url("ftp://foo.bar.baz") == false);
  assert(is_absolute_url("http://foo.bar/") == true);
  assert(is_absolute_url("https://foo.bar?foo=bar#") == true);
  assert(is_absolute_url("http://foo.bar/?foo=bar;baz=nox") == true);
  assert(is_absolute_url("http://../..") == false);
  assert(is_absolute_url("") == false);
  assert(is_absolute_url(NULL) == false);

  puts("All passed: " SCOPE);
}

void TEST_is_version() {
  #undef SCOPE
  #define SCOPE "is_version"

  puts("\nTesting: " SCOPE);

  assert(is_version("HTTP/1.0") == true);
  assert(is_version("HTTP/1") == false);
  assert(is_version("HTTP/1.0.0") == false);
  assert(is_version("HTTP/foo.bar") == false);
  assert(is_version("") == false);
  assert(is_version(NULL) == false);

  puts("All passed: " SCOPE);
}

void TEST_is_triple() {
  #undef SCOPE
  #define SCOPE "is_triple"

  puts("\nTesting: " SCOPE);

  assert(is_triple("GET foo.bar HTTP/1.0") == true);
  assert(is_triple("GET foo.bar") == false);
  assert(is_triple("FOOBAR foo.bar HTTP/1.1") == false);
  assert(is_triple("") == false);
  assert(is_triple(NULL) == false);

  puts("All passed: " SCOPE);
}

void TEST_get_maybe_method() {
  #undef SCOPE
  #define SCOPE "get_maybe_method"

  puts("\nTesting: " SCOPE);

  assert(strcmp(get_maybe_method("GET foo.bar HTTP/1.0"), "GET") == 0);
  assert(strcmp(get_maybe_method("POST example.com HTTP/1.1"), "POST") == 0);

  puts("All passed: " SCOPE);
}

void TEST_get_maybe_url() {
  #undef SCOPE
  #define SCOPE "get_maybe_url"

  puts("\nTesting: " SCOPE);

  assert(strcmp(get_maybe_url("GET foo.bar HTTP/1.0"), "foo.bar") == 0);
  assert(strcmp(get_maybe_url("POST example.com HTTP/1.1"), "example.com") == 0);

  puts("All passed: " SCOPE);
}

void TEST_get_maybe_version() {
  #undef SCOPE
  #define SCOPE "get_maybe_version"

  puts("\nTesting: " SCOPE);

  assert(strcmp(get_maybe_version("GET foo.bar HTTP/1.0"), "HTTP/1.0") == 0);
  assert(strcmp(get_maybe_version("POST example.com HTTP/1.1"), "HTTP/1.1") == 0);

  puts("All passed: " SCOPE);
}

void TEST_is_request_line() {
  #undef SCOPE
  #define SCOPE "is_request_line"

  puts("\nTesting: " SCOPE);

  assert(is_request_line("GET foo.bar HTTP/1.0") == true);
  assert(is_request_line("POST example.com HTTP/1.1") == false);
  assert(is_request_line("GET foo.bar") == false);
  assert(is_request_line("") == false);
  assert(is_request_line(NULL) == false);

  puts("All passed: " SCOPE);
}

void TEST_substr() {
  #undef SCOPE
  #define SCOPE "substr"

  puts("\nTesting: " SCOPE);

  assert(strcmp(substr("bar", 1, 2), "a") == 0);
  assert(strcmp(substr("bar", 0, 3), "bar") == 0);
  assert(strcmp(substr("bar", 0, 0), "") == 0);
  assert(substr("bar", 1, 0) == NULL);
  assert(substr("bar", -1, 0) == NULL);
  assert(substr("", 0, 1) == NULL);
  assert(substr(NULL, 0, 0) == NULL);

  puts("All passed: " SCOPE);
}

void TEST_Pcre_capture() {
  #undef SCOPE
  #define SCOPE "Pcre_capture"

  puts("\nTesting: " SCOPE);

  assert(strcmp(Pcre_capture("\\d", "foo1bar"), "1") == 0);
  assert(strcmp(Pcre_capture("\\S+", " foo "), "foo") == 0);
  assert(strcmp(Pcre_capture("/\\w+", "http://foo.bar"), "/foo") == 0);

  puts("All passed: " SCOPE);
}

void TEST_prepend() {
  #undef SCOPE
  #define SCOPE "prepend"

  puts("\nTesting: " SCOPE);

  assert(strcmp(prepend("oo", 'f'), "foo") == 0);
  assert(strcmp(prepend("", 'f'), "f") == 0);
  assert(strcmp(prepend("foo", '\0'), "") == 0);
  assert(prepend(NULL, 'f') == NULL);

  puts("All passed: " SCOPE);
}

void TEST_absolute_to_relative() {
  #undef SCOPE
  #define SCOPE "absolute_to_relative"

  puts("\nTesting: " SCOPE);

  assert(strcmp(absolute_to_relative("https://foo.bar"), "/") == 0);
  assert(strcmp(absolute_to_relative("http://foo.bar/foo"), "/foo") == 0);
  assert(strcmp(absolute_to_relative("https://foo.bar:43/foo/bar/#"), "/foo/bar/#") == 0);
  assert(strcmp(absolute_to_relative("http://foo.bar:8080?foo=bar"), "/?foo=bar") == 0);
  assert(strcmp(absolute_to_relative("foo.bar/?foo=bar;baz=nox#fragment"), "/?foo=bar;baz=nox#fragment") == 0);

  puts("All passed: " SCOPE);
}

int main() {
  TEST_is_method();
  TEST_is_relative_url();
  TEST_is_absolute_url();
  TEST_is_version();
  TEST_is_triple();
  TEST_get_maybe_method();
  TEST_get_maybe_url();
  TEST_get_maybe_version();
  TEST_is_request_line();
  TEST_substr();
  TEST_Pcre_capture();
  TEST_prepend();
  TEST_absolute_to_relative();

  return 0;
}