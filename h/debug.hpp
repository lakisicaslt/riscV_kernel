#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "hw.h"

void debugPutc(char c);
void debugPrint(const char* str);
void debugPrintln(const char* str);
void debugPrintUInt(uint64 value);
void debugPrintHex(uint64 value);

#endif
