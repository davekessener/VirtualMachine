SRC=$(wildcard *.l)
CSRC=$(SRC:.l=.c)

.PHONY: all clean

%.c: %.l
	lex -o $@ $<
	cc -o $(@:.c=) $@ -ll

all: $(CSRC)

clean:
	-rm -f $(CSRC) $(CSRC:.c=)

