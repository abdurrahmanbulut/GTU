CC = gcc
CFLAGS=-g 	# you can add those but it will give just some warnings:  -Wall -Wextra 
TARGET = terminal
OBJECTS = terminal.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)