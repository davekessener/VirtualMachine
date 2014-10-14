# Generated Sept. 29th, 2014
CC=g++-4.7
SRC=Evaluator.cc InfToPost.cc Parser.cc Tokenizer.cc main.cc 
OBJ=Evaluator.cc.o InfToPost.cc.o Parser.cc.o Tokenizer.cc.o main.cc.o 
CFLAGS=
LIBS=
INCDIRS=
LIBDIRS=

# Please replace directories
BOOSTINCDIR=-I/home/dave/include
BOOSTLIBDIR=-L/home/dave/include/lib
#

BOOSTLIBS=-lboost_regex
LIBS=$(BOOSTLIBS)
TARGET=parser

ACFLAGS=$(CFLAGS) -std=gnu++11 -Wall -O0 -ggdb -DDEBUG
ALIBS=$(LIBS) 

AINCDIRS=$(INCDIRS) $(BOOSTINCDIR) -I.
ALIBDIRS=$(LIBDIRS) $(BOOSTLIBDIR)

.PHONY: logs clean

all: logs $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(ALIBDIRS) -Wl,--start-group $(ALIBS) -Wl,--end-group

Evaluator.cc.o: Evaluator.cc Evaluator.h Stack.hpp analysis_io.hpp io.hpp lex.hpp lib.hpp
	$(CC) -c $(ACFLAGS) $(AINCDIRS) Evaluator.cc -o Evaluator.cc.o

InfToPost.cc.o: InfToPost.cc Tokenizer.h analysis_io.hpp io.hpp lex.hpp lib.hpp
	$(CC) -c $(ACFLAGS) $(AINCDIRS) InfToPost.cc -o InfToPost.cc.o

Parser.cc.o: Parser.cc Parser.h analysis_io.hpp io.hpp lex.hpp lib.hpp
	$(CC) -c $(ACFLAGS) $(AINCDIRS) Parser.cc -o Parser.cc.o

Tokenizer.cc.o: Tokenizer.cc Tokenizer.h analysis_io.hpp io.hpp lex.hpp lib.hpp
	$(CC) -c $(ACFLAGS) $(AINCDIRS) Tokenizer.cc -o Tokenizer.cc.o

main.cc.o: main.cc Evaluator.h InfToPost.h Parser.h Tokenizer.h analysis_io.hpp io.hpp lib.hpp
	$(CC) -c $(ACFLAGS) $(AINCDIRS) main.cc -o main.cc.o

logs:
	if [ -d "logs" ]; then for LF in *.log; do if [ -f "$$LF" ]; then mv $$LF logs/; fi; done; fi

clean: logs
	rm -f $(OBJ) $(TARGET)

