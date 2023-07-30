all: program

program: main.o
	g++ main.o book.o catalog.o movie.o music.o -o exe

main.o: main.cpp
	g++ -c -std=c++11 main.cpp book.cpp catalog.cpp movie.cpp music.cpp

clean:
	rm *.o exe

run:
	./exe