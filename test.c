#include "parse.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void TEST_is_method() {
  #define SCOPE "is_method"

  puts("Testing: " SCOPE);

  assert(is_method("GET") == true);
  assert(is_method("get") == false);
  assert(is_method("") == false);
  assert(is_method(NULL) == false);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_is_relative_url() {
  #define SCOPE "is_relative_url"
  puts("Testing: " SCOPE);

  assert(is_relative_url("123") == false);
  assert(is_relative_url("/index.html") == true);
  assert(is_relative_url("/") == true);
  assert(is_relative_url("(/)") == false);
  assert(is_relative_url("/foo") == true);
  assert(is_relative_url("/foo/bar/") == true);
  assert(is_relative_url("/foo/?bar=baz") == true);
  assert(is_relative_url("/foo/bar=baz") == false);
  assert(is_relative_url("foo/bar") == false);
  assert(is_relative_url("/foo/bar?baz=123#") == true);
  assert(is_relative_url("/foo/bar#") == true);
  assert(is_relative_url("/foo/bar?baz=123&nox=456#") == true);
  assert(is_relative_url("/foo/bar?baz=123#nox=456") == false);
  assert(is_relative_url("/foo/bar?baz=123http://foo.bar") == false);
  assert(is_relative_url("") == false);
  assert(is_relative_url(NULL) == false);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_is_absolute_url() {
  #define SCOPE "is_absolute_url"

  puts("Testing: " SCOPE);

  assert(is_absolute_url("http://localhost:80") == true);
  assert(is_absolute_url("foobar") == true);
  assert(is_absolute_url("foo.bar:80/index.html#") == true);
  assert(is_absolute_url("http://foobar") == true);
  assert(is_absolute_url("foo.bar") == true);
  assert(is_absolute_url("foo.bar/baz") == true);
  assert(is_absolute_url("foo.bar/?foo=bar&baz=nox#fragment") == true);
  assert(is_absolute_url("https://foo.bar") == true);
  assert(is_absolute_url("foo.bar.baz") == true);
  assert(is_absolute_url("ftp://foo.bar.baz") == false);
  assert(is_absolute_url("http://foo.bar/") == true);
  assert(is_absolute_url("https://foo.bar?foo=bar#") == true);
  assert(is_absolute_url("http://foo.bar/?foo=bar&baz=nox") == true);
  assert(is_absolute_url("http://../..") == false);
  assert(is_absolute_url("") == false);
  assert(is_absolute_url(NULL) == false);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_is_version() {
  #define SCOPE "is_version"

  puts("Testing: " SCOPE);

  assert(is_version("HTTP/1.0") == true);
  assert(is_version("HTTP/1") == false);
  assert(is_version("HTTP/1.0.0") == false);
  assert(is_version("HTTP/foo.bar") == false);
  assert(is_version("") == false);
  assert(is_version(NULL) == false);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_is_triple() {
  #define SCOPE "is_triple"

  puts("Testing: " SCOPE);

  assert(is_triple("GET foo.bar HTTP/1.0") == true);
  assert(is_triple("GET foo.bar") == false);
  assert(is_triple("FOOBAR foo.bar HTTP/1.1") == false);
  assert(is_triple("") == false);
  assert(is_triple(NULL) == false);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_get_maybe_method() {
  #define SCOPE "get_maybe_method"

  puts("Testing: " SCOPE);

  assert(strcmp(get_maybe_method("GET foo.bar HTTP/1.0"), "GET") == 0);
  assert(strcmp(get_maybe_method("POST example.com HTTP/1.1"), "POST") == 0);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_get_maybe_url() {
  #define SCOPE "get_maybe_url"

  puts("Testing: " SCOPE);

  assert(strcmp(get_maybe_url("GET foo.bar HTTP/1.0"), "foo.bar") == 0);
  assert(strcmp(get_maybe_url("POST example.com HTTP/1.1"), "example.com") == 0);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_get_maybe_version() {
  #define SCOPE "get_maybe_version"

  puts("Testing: " SCOPE);

  assert(strcmp(get_maybe_version("GET foo.bar HTTP/1.0"), "HTTP/1.0") == 0);
  assert(strcmp(get_maybe_version("POST example.com HTTP/1.1"), "HTTP/1.1") == 0);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_is_request_line() {
  #define SCOPE "is_request_line"

  puts("Testing: " SCOPE);

  assert(is_request_line("GET foo.bar HTTP/1.0") == true);
  assert(is_request_line("POST example.com HTTP/1.1") == false);
  assert(is_request_line("GET http://www.cmu.edu/hub/index.html HTTP/1.1") == true);
  assert(is_request_line("GET foo.bar") == false);
  assert(is_request_line("") == false);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_substr() {
  #define SCOPE "substr"

  puts("Testing: " SCOPE);

  assert(strcmp(substr("bar", 1, 2), "a") == 0);
  assert(strcmp(substr("bar", 0, 3), "bar") == 0);
  assert(strcmp(substr("bar", 0, 0), "") == 0);
  assert(substr("bar", 1, 0) == NULL);
  assert(substr("bar", -1, 0) == NULL);
  assert(substr("", 0, 1) == NULL);
  assert(substr(NULL, 0, 0) == NULL);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_Pcre_capture() {
  #define SCOPE "Pcre_capture"

  puts("Testing: " SCOPE);

  assert(strcmp(Pcre_capture("\\d", "foo1bar"), "1") == 0);
  assert(strcmp(Pcre_capture("\\S+", " foo "), "foo") == 0);
  assert(strcmp(Pcre_capture("/\\w+", "http://foo.bar"), "/foo") == 0);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_absolute_to_relative() {
  #define SCOPE "absolute_to_relative"

  puts("Testing: " SCOPE);

  assert(strcmp(absolute_to_relative("localhost"), "/") == 0);
  assert(strcmp(absolute_to_relative("https://foo.bar"), "/") == 0);
  assert(strcmp(absolute_to_relative("http://foo.bar/foo"), "/foo") == 0);
  assert(strcmp(absolute_to_relative("https://foo.bar:43/foo/bar/#"), "/foo/bar/#") == 0);
  assert(strcmp(absolute_to_relative("http://foo.bar:8080?foo=bar"), "/?foo=bar") == 0);
  assert(strcmp(absolute_to_relative("foo.bar/?foo=bar&baz=nox#fragment"), "/?foo=bar&baz=nox#fragment") == 0);
  assert(strcmp(absolute_to_relative("foo.bar/index.html#"), "/index.html#") == 0);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_MACROS() {
  #define SCOPE "MACROS"

  puts("Testing: " SCOPE);

  assert(LINE_SIZE == 115);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_make_request_line() {
  #define SCOPE "make_request_line"

  puts("Testing: " SCOPE);

  assert(strcmp(make_request_line("/"), "GET / HTTP/1.0") == 0);
  assert(strcmp(make_request_line("/foo"), "GET /foo HTTP/1.0") == 0);
  assert(strcmp(make_request_line("/?foo=bar&baz=nox#fragment"), "GET /?foo=bar&baz=nox#fragment HTTP/1.0") == 0);
  assert(strcmp(make_request_line("/foo?foo=bar&baz=nox"), "GET /foo?foo=bar&baz=nox HTTP/1.0") == 0);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_get_hostname() {
  #define SCOPE "get_hostname"

  puts("Testing: " SCOPE);

  assert(strcmp(get_hostname("https://foo.bar"), "foo.bar") == 0);
  assert(strcmp(get_hostname("http://foo.bar/foo"), "foo.bar") == 0);
  assert(strcmp(get_hostname("https://foo.bar:43/foo/bar/#"), "foo.bar") == 0);
  assert(strcmp(get_hostname("http://foo.bar:8080?foo=bar"), "foo.bar") == 0);
  assert(strcmp(get_hostname("foo.bar/?foo=bar&baz=nox#fragment"), "foo.bar") == 0);
  assert(strcmp(get_hostname("foo.bar"), "foo.bar") == 0);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_map_request_line() {
  #define SCOPE "map_request_line"

  puts("Testing: " SCOPE);

  assert(strcmp(map_request_line("GET http://www.cmu.edu/hub/index.html HTTP/1.1"), "GET /hub/index.html HTTP/1.0") == 0);
  assert(strcmp(map_request_line("GET https://baidu.com HTTP/1.1"), "GET / HTTP/1.0") == 0);
  assert(strcmp(map_request_line("GET http://www.cmu.edu/hub/index.html HTTP/1.1"), "GET /hub/index.html HTTP/1.0") == 0);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_concat() {
  #define SCOPE "concat"

  puts("Testing: " SCOPE);

  assert(strcmp(concat("foo", "bar"), "foobar") == 0);
  assert(strcmp(concat("foo", ""), "foo") == 0);
  assert(strcmp(concat("", "bar"), "bar") == 0);
  assert(strcmp(concat("", ""), "") == 0);
  assert(concat(NULL, NULL) == NULL);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_is_line() {
  #define SCOPE "is_line"

  puts("Testing: " SCOPE);

  assert(is_line("GET /hub/index.html HTTP/1.0") == true);
  assert(is_line("foo\nbar") == true);
  assert(is_line("") == true);
  assert(is_line("foo\r\nbar") == false);
  assert(is_line(NULL) == false);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_is_header() {
  #define SCOPE "is_header"

  puts("Testing: " SCOPE);

  assert(is_header("foobar") == false);
  assert(is_header("foo: bar") == false);
  assert(is_header("Host: localhost:4500") == true);
  assert(is_header("User-Agent: curl/7.81.0") == true);
  assert(is_header("Accept: */*") == true);
  assert(is_header("Accept-Encoding: gzip") == true);
  assert(is_header("") == false);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_get_header_key() {
  #define SCOPE "get_header_key"
  puts("Testing: " SCOPE);

  assert(strcmp(get_header_key("Host: localhost:4500"), "Host") == 0);
  assert(strcmp(get_header_key("User-Agent: curl/7.81.0"), "User-Agent") == 0);
  assert(strcmp(get_header_key("Accept: */*"), "Accept") == 0);
  assert(strcmp(get_header_key("Accept-Encoding: gzip"), "Accept-Encoding") == 0);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_get_header_value() {
  #define SCOPE "get_header_value"
  puts("Testing: " SCOPE);

  assert(strcmp(get_header_value("Host: localhost:4500"), "localhost:4500") == 0);
  assert(strcmp(get_header_value("User-Agent: curl/7.81.0"), "curl/7.81.0") == 0);
  assert(strcmp(get_header_value("Accept: */*"), "*/*") == 0);
  assert(strcmp(get_header_value("Accept-Encoding: gzip"), "gzip") == 0);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_make_header() {
  #define SCOPE "make_header"
  puts("Testing: " SCOPE);

  assert(strcmp(make_header("Host", "localhost:4500"), "Host: localhost:4500") == 0);
  assert(strcmp(make_header("User-Agent", "curl/7.81.0"), "User-Agent: curl/7.81.0") == 0);
  assert(strcmp(make_header("Accept", "*/*"), "Accept: */*") == 0);
  assert(strcmp(make_header("Accept-Encoding", "gzip"), "Accept-Encoding: gzip") == 0);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_can_proxy_modify_key() {
  #define SCOPE "can_proxy_modify_key"
  puts("Testing: " SCOPE);

  assert(can_proxy_modify_key("Host") == false);
  assert(can_proxy_modify_key("User-Agent") == true);
  assert(can_proxy_modify_key("Accept") == false);
  assert(can_proxy_modify_key("Accept-Encoding") == false);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_map_header() {
  #define SCOPE "map_header"
  puts("Testing: " SCOPE);

  assert(strcmp(map_header("Host: localhost:4500"), "Host: localhost:4500") == 0);
  assert(strcmp(map_header("User-Agent: curl/7.81.0"), "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3") == 0);
  assert(strcmp(map_header("Connection: keep-alive"), "Connection: close") == 0);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

void TEST_get_port() {
  #define SCOPE "get_port"
  puts("Testing: " SCOPE);

  assert(get_port("http://localhost:4500#") == 4500);
  assert(get_port("https://google.com:4500/?foo=bar&baz=nox") == 4500);
  assert(get_port("http://192.168.0.1") == 80);

  puts("All passed: " SCOPE);
  #undef SCOPE
}

int main() {
  TEST_MACROS();
  TEST_is_method();
  TEST_concat();
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
  TEST_absolute_to_relative();
  TEST_make_request_line();
  TEST_get_hostname();
  TEST_map_request_line();
  TEST_is_line();
  TEST_is_header();
  TEST_get_header_key();
  TEST_get_header_value();
  TEST_make_header();
  TEST_can_proxy_modify_key();
  TEST_map_header();
  TEST_get_port();

  return 0;
}