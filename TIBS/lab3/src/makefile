CC=gcc
CFLAGS=-Wall -g -ggdb -O0
LDFLAGS=-pthread
HEAD=$(wildcard:*.h)

MM_SRC=mmanage.c debug.c
MM_OBJ=$(MM_SRC:.c=.o)

VM_SRC=vmaccess.c vmappl.c debug.c
VM_OBJ=$(VM_SRC:.c=.o)

TM_SRC=vmaccess.c vmtest.c debug.c
TM_OBJ=$(TM_SRC:.c=.o)

CFLAGS+=-DDEBUG
CFLAGS+=-DVMEM_PAGESIZE=16
CFLAGS+=-DVMEM_ALGO=0

%.o: %.c $(HEAD)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: all clean

all: mmanage vmappl test

mmanage: $(MM_OBJ)
	$(CC) $(MM_OBJ) -o mmanage $(LDFLAGS)

vmappl: $(VM_OBJ)
	$(CC) $(VM_OBJ) -o vmappl $(LDFLAGS)

test: $(TM_OBJ)
	$(CC) $(TM_OBJ)  -o vmtest $(LDFLAGS)

clean:
	rm -f mmanage vmappl vmtest *.o

