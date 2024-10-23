CC = gcc
CFLAGS = -Wall -Wextra -O2

.PHONY: all clean test

all: move protect_unlink.so

move: move.c
	$(CC) $(CFLAGS) -o move move.c

protect_unlink.so: protect_unlink.c
	$(CC) -fPIC -shared -ldl -o protect_unlink.so protect_unlink.c

test: move protect_unlink.so
	chmod +x test.sh
	./test.sh

clean:
	rm -f move protect_unlink.so
	rm -f testfile infile outfile PROTECT_infile
