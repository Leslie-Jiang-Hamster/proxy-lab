#include <stdio.h>
#include <assert.h>
#include "proxy.h"
#include "parse.h"
#include "csapp.h"
#include "pool.h"

static const int THREAD_NUMBER = 12;
static const int RN_LINE_SIZE = 1024;
static const int RESPONSE_SIZE = 102400;
static struct sockaddr_storage addr;
static pthread_t tid;

static char *get_port_from_cmdline(int argc, char **argv) {
    assert(argc == 2);
    int port = atoi(argv[1]);
    assert(0 < port && port < (2 << 16));
    return argv[1];
}

static void handle(int connfd) {
    rio_t rio;
    Rio_readinitb(&rio, connfd);

    char rn_line[RN_LINE_SIZE];
    memset(rn_line, 0, sizeof(rn_line));
    
    Rio_readlineb(&rio, rn_line, sizeof(rn_line));
    char *request_line = rn_line_to_line(rn_line);
    puts(request_line);
    assert(is_request_line(request_line));

    char *absolute_url = get_maybe_url(request_line);
    char *hostname = get_hostname(absolute_url);
    char *port = calloc(RN_LINE_SIZE, sizeof(char));
    sprintf(port, "%d", get_port(absolute_url));
    free(absolute_url);

    char *mapped_request_line = map_request_line(request_line);
    free(request_line);
    char *mapped_rn_request_line = line_to_rn_line(mapped_request_line);
    free(mapped_request_line);

    int serverfd = Open_clientfd(hostname, port);
    free(hostname);
    free(port);
    Rio_writen(serverfd, mapped_rn_request_line, strlen(mapped_rn_request_line));
    free(mapped_rn_request_line);

    int this_line_size;
    while ((this_line_size = Rio_readlineb(&rio, rn_line, sizeof(rn_line)))) {
        char *maybe_header = rn_line_to_line(rn_line);
        puts(maybe_header);
        if (!is_header(maybe_header)) {
            char *http_end = line_to_rn_line(maybe_header);
            free(maybe_header);
            Rio_writen(serverfd, http_end, strlen(http_end));
            free(http_end);
            break;
        }
        char *mapped_header = map_header(maybe_header);
        free(maybe_header);
        char *mapped_rn_header = line_to_rn_line(mapped_header);
        free(mapped_header);
        Rio_writen(serverfd, mapped_rn_header, strlen(mapped_rn_header));
        free(mapped_rn_header);
    }

    char *response = calloc(RESPONSE_SIZE, sizeof(char));
    int response_size = Rio_readn(serverfd, response, RESPONSE_SIZE);
    Rio_writen(connfd, response, response_size);
    free(response);

    Close(connfd);
    Close(serverfd);
}

static void *thread(void *varg) {
    Pthread_detach(pthread_self());

    while (1) {
        int connfd = Pool_pop();
        handle(connfd);
    }

    return NULL;
}

static void create_threads(int thread_number) {
    for (int i = 0; i < thread_number; i++) {
        pthread_create(&tid, NULL, thread, NULL);
    }
}

int main(int argc, char **argv)
{
    Pool_init();
    create_threads(THREAD_NUMBER);
    int listenfd = Open_listenfd(get_port_from_cmdline(argc, argv));

    while (1) {
        socklen_t addr_size = sizeof(addr);
        int connfd = Accept(listenfd, (SA *)&addr, &addr_size);
        Pool_push(connfd);
    }

    return 0;
}
