CC=gcc
CFLAGS=-Wall -g -ggdb -O0
LDFLAGS=-pthread
HEAD=$(wildcard:*.h)

MM_SRC=mmanage.c debug.c
MM_OBJ=$(MM_SRC:.c=.o)

VM_SRC=vmaccess.c vmappl.c debug.c
VM_OBJ=$(VM_SRC:.c=.o)

CFLAGS+=-DDEBUG -DVMEM_PAGESIZE=8 -DVMEM_ALGO=0

%.o: %.c $(HEAD)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: all clean

all: mmanage vmappl

mmanage: $(MM_OBJ)
	$(CC) $(MM_OBJ) -o mmanage $(LDFLAGS)

vmappl: $(VM_OBJ)
	$(CC) $(VM_OBJ) -o vmappl $(LDFLAGS)

test: vmtest.o vmaccess.o debug.o
	$(CC) vmtest.o vmaccess.o debug.o  -o vmtest $(LDFLAGS)

clean:
	rm -f mmanage *.o

