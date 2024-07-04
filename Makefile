CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: 
	$(CC) $(CFLAGS) main.c -o fetchme
	./move_to_path.sh

clean:
	rm -f fetchme
	./remove_from_path.sh
