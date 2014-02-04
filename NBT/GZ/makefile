libgz.so: gzstream.cc gzstream.h
	g++ -fPIC -c gzstream.cc -o gzstream.o
	g++ -shared -Wl,-soname,libgz.so -o libgz.so gzstream.o

clean:
	-rm *.o *.so

