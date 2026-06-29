#include "RiscV.hpp"
#include "MemoryAllocator.hpp"

void RiscV::handleSupervisorTrap() {
    uint64 scause = r_scause();
    uint64 volatile sepc = r_sepc();
    uint64 volatile sstatus = r_sstatus();

    // ecall iz korisničkog režima: scause = 8
    // ecall iz sistemskog režima: scause = 9
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        sepc += 4;

        uint64 code;
        asm volatile("ld %0, 80(fp)" : "=r"(code));

        switch (code) {
            case 0x01: {
                // ABI mem_alloc prima broj blokova.
                size_t blocks;
                asm volatile("ld %0, 88(fp)" : "=r"(blocks));

                void* addr = MemoryAllocator::mem_alloc(blocks);

                asm volatile("sd %0, 80(fp)" : : "r"((uint64)addr));
                break;
            }

            case 0x02: {
                void* ptr;
                asm volatile("ld %0, 88(fp)" : "=r"(ptr));

                int ret = MemoryAllocator::mem_free(ptr);

                asm volatile("sd %0, 80(fp)" : : "r"((uint64)ret));
                break;
            }

            default: {
                asm volatile("sd %0, 80(fp)" : : "r"((uint64)-1));
                break;
            }
        }
    }

        // Prekid od tajmera; za sada samo čistimo pending bit.
    else if (scause == ((1UL << 63) | 1)) {
        mc_sip(SIP_SSIP);
    }

        // Ostale prekide/izuzetke za sada ne obrađujemo.
    else {
        while (true) {
        }
    }

    w_sepc(sepc);
    w_sstatus(sstatus);
}

