DEBUG=-ggdb -DDEBUG

libgz.so: gzstream.cc gzstream.h
	g++ -fPIC $(DEBUG) -c gzstream.cc -o gzstream.o
	g++ -shared -Wl,-soname,libgz.so -o libgz.so gzstream.o -lz

clean:
	-rm *.o *.so

