CC=gcc
CFLAGS=-Wall -ggdb -O0
LDFLAGS=-pthread
MM_SRC=mmanage.c debug.c
HEAD=$(wildcard:*.h)

%.o: %.c $(HEAD)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: all clean

all: mmanage

mmanage: $(MM_SRC:.c=.o)
	$(CC) $(MM_SRC:.c=.o) -o mmanage $(LDFLAGS)

clean:
	rm -f mmanage *.o

