all:
	generate-makefile > Mk
	make --no-print-directory -f Mk
	rm -f *.log

Mk:
	generate-makefile > Mk

clean: Mk
	make clean --no-print-directory -f Mk
	rm -f *.log Mk

