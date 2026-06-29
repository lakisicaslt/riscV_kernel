#ifndef RISCV_HPP
#define RISCV_HPP

#include "hw.h"

class RiscV {
public:
    static void supervisorTrap();
    static void handleSupervisorTrap();

    static uint64 r_scause();
    static uint64 r_sepc();
    static uint64 r_sstatus();

    static void w_sepc(uint64 sepc);
    static void w_sstatus(uint64 sstatus);
    static void w_stvec(uint64 stvec);

    static void mc_sip(uint64 mask);

    static const uint64 SIP_SSIP = (1UL << 1);
};

inline uint64 RiscV::r_scause() {
    uint64 volatile scause;
    asm volatile("csrr %0, scause" : "=r"(scause));
    return scause;
}

inline uint64 RiscV::r_sepc() {
    uint64 volatile sepc;
    asm volatile("csrr %0, sepc" : "=r"(sepc));
    return sepc;
}

inline uint64 RiscV::r_sstatus() {
    uint64 volatile sstatus;
    asm volatile("csrr %0, sstatus" : "=r"(sstatus));
    return sstatus;
}

inline void RiscV::w_sepc(uint64 sepc) {
    asm volatile("csrw sepc, %0" : : "r"(sepc));
}

inline void RiscV::w_sstatus(uint64 sstatus) {
    asm volatile("csrw sstatus, %0" : : "r"(sstatus));
}

inline void RiscV::w_stvec(uint64 stvec) {
    asm volatile("csrw stvec, %0" : : "r"(stvec));
}

inline void RiscV::mc_sip(uint64 mask) {
    asm volatile("csrc sip, %0" : : "r"(mask));
}

#endif
