CC=cc
CFLAGS=-Wall -ggdb -O0 -Wno-psabi
MACROS=-DUNIX

all: main

main: main.c.o tokenizer.c.o preprocessor.c.o squeue.c.o assembler.c.o numeval.c.o include.c.o evaluate.c.o tkn.c.o dstack.c.o logger.c.o stackmachine.c.o parameter.c.o params.c.o
	$(CC) $(CFLAGS) main.c.o tokenizer.c.o preprocessor.c.o squeue.c.o assembler.c.o numeval.c.o include.c.o evaluate.c.o tkn.c.o dstack.c.o logger.c.o stackmachine.c.o parameter.c.o params.c.o -o $@ -lm


main.c.o: main.c tokenizer.h preprocessor.h squeue.h assembler.h numeval.h include.h evaluate.h tkn.h dstack.h logger.h stackmachine.h parameter.h params.h
	$(CC) $(CFLAGS) $(MACROS) -c main.c -o $@

squeue.c.o: squeue.c squeue.h
	$(CC) $(CFLAGS) $(MACROS) -c squeue.c -o $@

preprocessor.c.o: preprocessor.c preprocessor.h tokenizer.h squeue.h
	$(CC) $(CFLAGS) $(MACROS) -c preprocessor.c -o $@

params.c.o: params.c params.h include.h parameter.h
	$(CC) $(CFLAGS) $(MACROS) -c params.c -o $@

logger.c.o: logger.c logger.h include.h evaluate.h tkn.h dstack.h stackmachine.h parameter.h
	$(CC) $(CFLAGS) $(MACROS) -c logger.c -o $@

parameter.c.o: parameter.c parameter.h
	$(CC) $(CFLAGS) $(MACROS) -c parameter.c -o $@

dstack.c.o: dstack.c dstack.h include.h
	$(CC) $(CFLAGS) $(MACROS) -c dstack.c -o $@

include.c.o: include.c include.h
	$(CC) $(CFLAGS) $(MACROS) -c include.c -o $@

stackmachine.c.o: stackmachine.c stackmachine.h include.h dstack.h parameter.h
	$(CC) $(CFLAGS) $(MACROS) -c stackmachine.c -o $@

tokenizer.c.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) $(MACROS) -c tokenizer.c -o $@

assembler.c.o: assembler.c assembler.h tokenizer.h numeval.h include.h evaluate.h tkn.h dstack.h logger.h stackmachine.h parameter.h params.h
	$(CC) $(CFLAGS) $(MACROS) -c assembler.c -o $@

evaluate.c.o: evaluate.c evaluate.h include.h tkn.h dstack.h logger.h stackmachine.h parameter.h
	$(CC) $(CFLAGS) $(MACROS) -c evaluate.c -o $@

tkn.c.o: tkn.c tkn.h include.h
	$(CC) $(CFLAGS) $(MACROS) -c tkn.c -o $@

numeval.c.o: numeval.c numeval.h include.h evaluate.h tkn.h dstack.h logger.h stackmachine.h parameter.h params.h
	$(CC) $(CFLAGS) $(MACROS) -c numeval.c -o $@


clean:
	rm -f main.c.o tokenizer.c.o preprocessor.c.o squeue.c.o assembler.c.o numeval.c.o include.c.o evaluate.c.o tkn.c.o dstack.c.o logger.c.o stackmachine.c.o parameter.c.o params.c.o

remove: clean
	rm -f main

