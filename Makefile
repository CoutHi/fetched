CC = gcc
CFLAGS = -Wall -Wextra -std=c99
INCLUDEFLAGS = -I/usr/include/stb -lm
BINARY1 = fetchme
BINARY2 = asciify

all: 
	$(CC) $(CFLAGS) main.c -o $(BINARY1)
	$(CC) ascii.c $(INCLUDEFLAGS) -o $(BINARY2)

install:
	$(CC) $(CFLAGS) main.c -o $(BINARY1)
	$(CC) ascii.c $(INCLUDEFLAGS) -o $(BINARY2)
	./move_to_path.sh

uninstall:
	rm -f $(BINARY1) $(BINARY2) 
	./remove_from_path.sh
