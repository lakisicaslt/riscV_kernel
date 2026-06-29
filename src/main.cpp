#include "hw.h"
#include "syscall_c.hpp"

void userMain() __attribute__((weak));

void userMain() {
    // Prazan default userMain.
    // Javni testovi će obezbediti svoju verziju ove funkcije.
}


int main() {
    userMain();

    volatile uint32* shutdown = (uint32*)0x100000;
    *shutdown = 0x5555;

    return 0;
}

