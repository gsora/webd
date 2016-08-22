SOURCE := webd.c utils.c

CC := gcc
CFLAGS := -Wextra -Wall -Werror -O2
DEBUG := -g

debug:
	$(CC) $(SOURCE) $(CFLAGS) $(DEBUG) -o webd

release:
	$(CC) $(SOURCE) $(CFLAGS) -o webd

all: debug

clean:
	rm -rf *.o webd webd.dSYM
