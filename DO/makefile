CC=g++
DAVCFLAGS=
GLIBS=
DAVLIBS=-ldav_Logger
BOOSTLIBS=-lboost_system -lboost_filesystem
DAVDIR=$(shell echo ~/include)
INCDIRS=-I$(DAVDIR)
LIBDIRS=-L$(DAVDIR)/lib
CFLAGS=-std=gnu++11 -O0 -Wall -Wl,--no-as-needed -ggdb -DDEBUG $(DAVCFLAGS) $(INCDIRS)
LIBS=$(LIBDIRS) -Wl,--start-group $(DAVLIBS) $(GLIBS) $(BOOSTLIBS) $(GLLIBS) -Wl,--end-group
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TEMPLATES=$(wildcard *.hpp)
SUBS=
TARGET=do

%.o: %.cc $(HEADERS) $(TEMPLATES)
	$(CC) -c $(CFLAGS) $< -o $@

%.a:
	$(MAKE) -C $(@:.a=)/
	cp $(@:.a=)/*.a .

%.clean:
	-$(MAKE) -C $(@:.clean=)/ clean

$(TARGET): logs $(SUBS) $(OBJ)
	$(CC) $(OBJ) -Wl,--start-group $(shell find -maxdepth 1 -name "*.a") -Wl,--end-group -o $(TARGET) $(LIBS)

.PHONY: logs clean cleanlibs

logs:
	for file in *.log; do if [ -f "$$file" ]; then mv "$$file" logs/; fi; done

cleanlibs:
	rm -f *.a

clean: logs $(SUBS:.a=.clean)
	rm -f *.o $(TARGET)

