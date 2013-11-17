#ifndef _CHAR_H_INCLUDED__
#define _CHAR_H_INCLUDED__
#include <iostream>

// print the given character to the given stream
void pc(std::ostream &, char);
// print the given char[] to the given stream
void pc(std::ostream &, char *, int len);
// print the given null-terminated string to the given stream
void pc(std::ostream &, char *);

#endif
