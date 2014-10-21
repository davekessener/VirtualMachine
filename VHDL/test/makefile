CC=ghdl
VISU=gtkwave
TARGET=test
CFLAGS=--ieee=synopsys --warn-no-vital-generic
RUNFLAGS=--stop-time=500ns --assert-level=error
SRCDIR=src
TESTBENCHDIR=testbench
SIMDIR=simu

.PHONY: all analyse compile run visualize

all: visualize

analyse:
	$(CC) -i $(CFLAGS) --workdir=$(SIMDIR) --work=work $(SRCDIR)/*.vhdl $(TESTBENCHDIR)/$(TARGET)_file.vhd

compile: analyse
	$(CC) -m $(CFLAGS) --workdir=$(SIMDIR) --work=work $(TARGET)_file
	@mv $(TARGET)_file $(SIMDIR)/

run: compile
	$(SIMDIR)/$(TARGET)_file $(RUNFLAGS) --vcdgz=$(SIMDIR)/$(TARGET)_file.vcdgz

visualize: run
	gunzip --stdout $(SIMDIR)/$(TARGET)_file.vcdgz | $(VISU) --vcd

