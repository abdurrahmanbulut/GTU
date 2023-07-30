FLAGS = -c -Wall -ansi -pedantic -errors -std=gnu99
LIBS = -lnotify

all: BibakBOXClient BibakBOXServer

BibakBOXClient: BibakBOXClient.o
	gcc BibakBOXClient.o -o BibakBOXClient -pthread $(LIBS)

BibakBOXClient.o: BibakBOXClient.c
	gcc $(FLAGS) BibakBOXClient.c

BibakBOXServer: BibakBOXServer.o
	gcc BibakBOXServer.o -o BibakBOXServer -pthread

BibakBOXServer.o: BibakBOXServer.c
	gcc $(FLAGS) BibakBOXServer.c

clean:
	rm -rf *.o BibakBOXClient BibakBOXServer
