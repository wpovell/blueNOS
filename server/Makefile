CC = gcc
CFLAGS = -Wall -Wextra -Werror -O0 -g

BINARY = ../bin/server

all: $(BINARY)

clean:
	rm -f $(BINARY)

$(BINARY): server.c server.h ../include/xmodem/xmodem.h
	$(CC) -o $(BINARY) server.c -I../include/
