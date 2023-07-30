all: program

program: main.o
	g++ main.o gates.o reader.o -o exe

main.o: main.cpp
	g++ -c -std=c++11 main.cpp gates.cpp reader.cpp

clean:
	rm *.o exe

run:
	./exe