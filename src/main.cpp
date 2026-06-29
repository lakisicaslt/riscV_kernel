#include "hw.h"
#include "syscall_c.hpp"
#include "MemoryAllocator.hpp"

void userMain() __attribute__((weak));

void userMain() {
    // Prazan default userMain.
    // Javni testovi će obezbediti svoju verziju ove funkcije.
}

static void shutdown() {
    volatile uint32* shutdownAddress = (uint32*)0x100000;
    *shutdownAddress = 0x5555;
}

int main() {
    MemoryAllocator::init();

    userMain();

    shutdown();

    return 0;
}