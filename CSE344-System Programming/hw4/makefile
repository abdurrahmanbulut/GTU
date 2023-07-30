CC=gcc
CFLAGS=-lpthread -lrt
TARGETS = biboClient biboServer

all: $(TARGETS)

biboServer: biboServer.c
	$(CC) -o biboServer biboServer.c $(CFLAGS)

biboClient: biboClient.c
	$(CC) -o biboClient biboClient.c $(CFLAGS)

clean:
	rm -f biboServer biboClient