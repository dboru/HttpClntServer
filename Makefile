CC = gcc
LDFLAGS = -lm -lpthread
all: client server
client: src/httpclient.c Makefile
	$(CC)   src/httpclient.c $(LDFLAGS) -o $@
server: src/httpserver_fork.c Makefile
	$(CC)   src/httpserver_fork.c $(LDFLAGS) -o $@


clean:
	rm -rf client server *.o
