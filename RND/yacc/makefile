SRC=$(wildcard *.y)
CSRC=$(SRC:.y=.tab.c)

.PHONY: all clean

%.tab.c: %.y
	bison --defines=$(@:.tab.c=.tab.h) -o $@ $<
	lex -o $(@:.tab.c=.yy.c) $(@:.tab.c=.l)
	cc -o $(@:.tab.c=) $(@:.tab.c=.yy.c) $@ -ll -ly

all: $(CSRC)

clean:
	-rm -f *.c $(SRC:.y=)

