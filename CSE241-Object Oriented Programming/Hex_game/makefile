all: program

program: main.o
	g++ main.o helper.o -o exe

main.o: main.cpp
	g++ -c -std=c++11 main.cpp helper.cpp

clean:
	rm *.o exe

run:
	./exe