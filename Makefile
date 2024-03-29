# Makefile for Proxy Lab 
#
# You may modify this file any way you like (except for the handin
# rule). You instructor will type "make" on your specific Makefile to
# build your proxy from sources.

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread -lpcre

all: proxy

csapp.o: csapp.h
	$(CC) $(CFLAGS) -c csapp.c

proxy.o: csapp.h proxy.h pool.h handle.h

proxy: proxy.o csapp.o pool.o handle.o parse.o cache.o
	$(CC) $(CFLAGS) proxy.o csapp.o pool.o handle.o parse.o cache.o -o proxy $(LDFLAGS)

parse.o: parse.h

pool.o: csapp.h pool.h

handle.o: handle.h csapp.h parse.h cache.h

cache.o: cache.h

test: test.o parse.o pool.o csapp.o cache.o
	$(CC) $(CFLAGS) test.o parse.o pool.o csapp.o cache.o -o test $(LDFLAGS)

# Creates a tarball in ../proxylab-handin.tar that you can then
# hand in. DO NOT MODIFY THIS!
handin:
	(make clean; cd ..; tar cvf $(USER)-proxylab-handin.tar proxylab-handout --exclude tiny --exclude nop-server.py --exclude proxy --exclude driver.sh --exclude port-for-user.pl --exclude free-port.sh --exclude ".*")

clean:
	rm -f *~ *.o proxy core *.tar *.zip *.gzip *.bzip *.gz
