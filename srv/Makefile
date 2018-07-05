CC = gcc
CFLAGS = -Wall -Wextra -Werror -O0 -g

BINARY = ../bin/_srv

CFILES = $(wildcard *.c)
HEADERS = $(wildcard *.h)

all: $(BINARY)

clean:
	rm -f $(BINARY)

$(BINARY): $(CFILES) $(HEADERS) ../include/xmodem/xmodem.h
	echo $(CFILES)
	echo $(HEADERS)
	$(CC) -o $(BINARY) $(CFILES) -I../include/ -lreadline -pthread
