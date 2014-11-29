CC = gcc 
CFLAGS = -g -pedantic -std=c99 -Wall -Wextra

all: encode decode

encode: encode.c
	$(CC) $(CFLAGS) -o encode encode.c

decode: decode.c
	$(CC) $(CFLAGS) -o decode decode.c

clean:
	rm -f encode decode
