CC = g++
CFLAGS = -c -std=c++11
VALG = valgrind -v
PTHREAD = -pthread

all:operateArrays

part2: operateArrays.o
	$(CC) operateArrays.o -o operateArrays $(PTHREAD) -std=c++11

part2.o: operateArrays.cpp
	$(CC) $(CFLAGS) operateArrays.cpp
clean:
	rm -rf *o operateArrays