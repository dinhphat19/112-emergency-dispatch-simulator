CC = gcc
CFLAGS = -Wall -g -std=c99

build: tema1

tema1: tema1.c functii_tema1.c
	$(CC) $(CFLAGS) tema1.c functii_tema1.c -o tema1

clean:
	rm -f tema1 tema1.out