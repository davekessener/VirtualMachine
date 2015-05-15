#ifndef DAV_LISP_INTERFACE_H
#define DAV_LISP_INTERFACE_H

typedef unsigned HANDLE;

void eval(HANDLE);
HANDLE cons(HANDLE, HANDLE);
HANDLE symbol(const char *);
HANDLE number_i(uint64_t);
HANDLE number_f(double);
HANDLE string(const char *);
HANDLE append(HANDLE, HANDLE);

#endif

