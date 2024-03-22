#include <stdio.h>
#include <assert.h>
#include "proxy.h"
#include "csapp.h"
#include "pool.h"
#include "handle.h"
#include "cache.h"

static const int THREAD_NUMBER = 12;
static struct sockaddr_storage addr;
static pthread_t tid;

static char *get_port_from_cmdline(int argc, char **argv) {
    assert(argc == 2);
    int port = atoi(argv[1]);
    assert(0 < port && port < (2 << 16));
    return argv[1];
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
    Cache_init();
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
