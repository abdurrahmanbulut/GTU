FLAGS= -c -Wall -ansi -pedantic -errors -std=gnu99

pCp: pCp.o
	gcc pCp.o -o pCp -pthread

pCp.o: pCp.c
	gcc ${FLAGS} pCp.c

clean:
	rm -rf *.o pCp