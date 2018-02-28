CC = gcc
CFLAGS = -W -Wall
TARGET = test
OBJECTS = common.o multicast_manager.o token_manager.o main.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o test

