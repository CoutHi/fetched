CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: 
	$(CC) $(CFLAGS) main.c -o fetchme

clean:
	rm -f fetchme
