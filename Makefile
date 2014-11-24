CC = gcc 
CFLAGS = -g -pedantic -std=c99 -Wall -Wextra

all: encode

encode: encode.c
	$(CC) $(CFLAGS) -o encode encode.c

clean:
	rm -f encode
