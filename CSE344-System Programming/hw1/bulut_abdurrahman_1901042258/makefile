CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET1 = appendMeMore
TARGET2 = q2
TARGET3 = q3

all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c

$(TARGET2): $(TARGET2).c
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c

$(TARGET3): $(TARGET3).c
	$(CC) $(CFLAGS) -o $(TARGET3) $(TARGET3).c

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)