#include "char.h"
#include <iostream>

// print the given character to the given stream
void pc(std::ostream &stream, char key) {
    // replace spaces with _
    if(key == ' ') {
	key = '_';
    // replace string-terminator with a 0
    } else if(key == '\0') {
	key = '$';
    }

    // print the key to the given stream
    stream << key;
}

// print the given char[] to the given stream
void pc(std::ostream &stream, char *array, int len) {
    // print each char in the array
    for(int i = 0; i < len; i++) {
	pc(stream, array[i]);
    }
}

// print the given null-terminated string to the given stream
void pc(std::ostream &stream, char *string) {
    // keep printing chars until we get to the end of the string
    while(*string != '\0') {
	pc(stream, *string);
	string++;
    }
}
