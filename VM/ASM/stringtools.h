#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include <string>

void trim(std::string&);
std::string string_format(const char *, ...);
const std::string token_extract(std::string&, int&);
int convert_character(const char *&);
int getBase(char);

#endif

