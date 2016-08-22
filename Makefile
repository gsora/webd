SOURCE := webd.c utils.c

CC := gcc
CFLAGS := -Wextra -Wall -Werror
DEBUG := -g

debug:
	$(CC) $(SOURCE) $(CFLAGS) $(DEBUG) -o webd

release:
	$(CC) $(SOURCE) $(CFLAGS) -O2 -o webd

all: debug

clean:
	rm -rf *.o webd webd.dSYM
