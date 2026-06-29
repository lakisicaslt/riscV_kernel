#include "debug.hpp"

void debugPutc(char c) {
    while ((*(volatile uint8*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT) == 0) {
        // čekamo da konzola bude spremna za slanje znaka
    }

    *(volatile uint8*)CONSOLE_TX_DATA = c;
}

void debugPrint(const char* str) {
    while (*str != '\0') {
        debugPutc(*str);
        str++;
    }
}

void debugPrintln(const char* str) {
    debugPrint(str);
    debugPutc('\n');
}

void debugPrintUInt(uint64 value) {
    if (value == 0) {
        debugPutc('0');
        return;
    }

    char buffer[21];
    int i = 0;

    while (value > 0) {
        buffer[i++] = '0' + value % 10;
        value /= 10;
    }

    while (i > 0) {
        debugPutc(buffer[--i]);
    }
}

void debugPrintHex(uint64 value) {
    debugPrint("0x");

    if (value == 0) {
        debugPutc('0');
        return;
    }

    char buffer[16];
    int i = 0;

    while (value > 0) {
        uint64 digit = value % 16;

        if (digit < 10) {
            buffer[i++] = '0' + digit;
        } else {
            buffer[i++] = 'A' + digit - 10;
        }

        value /= 16;
    }

    while (i > 0) {
        debugPutc(buffer[--i]);
    }
}
