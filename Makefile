CC = gcc
CFLAGS = -Wall -Wextra -std=c99
BINARY = fetchme

all: 
	$(CC) $(CFLAGS) main.c -o $(BINARY)

install:
	$(CC) $(CFLAGS) main.c -o $(BINARY)
	./move_to_path

clean:
	rm -f fetchme
	./remove_from_path.sh
