#include "syscall_c.hpp"

void* mem_alloc(size_t size) {
    if (size == 0) return 0;

    // C API prima bajtove.
    // ABI mem_alloc prima broj blokova.
    uint64 volatile blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

    __asm__ volatile("mv a1, %0" : : "r" (blocks));
    __asm__ volatile("mv a0, %0" : : "r" (0x01));
    __asm__ volatile("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));

    return (void*)ret;
}

int mem_free(void* ptr) {
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)ptr));
    __asm__ volatile("mv a0, %0" : : "r" (0x02));
    __asm__ volatile("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));

    return (int)ret;
}

int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg) {
    (void)handle;
    (void)start_routine;
    (void)arg;
    return -1;
}

int thread_exit() {
    return -1;
}

void thread_dispatch() {
}

int sem_open(sem_t* handle, unsigned init) {
    (void)handle;
    (void)init;
    return -1;
}

int sem_close(sem_t handle) {
    (void)handle;
    return -1;
}

int sem_wait(sem_t id) {
    (void)id;
    return -1;
}

int sem_signal(sem_t id) {
    (void)id;
    return -1;
}

int sem_wait_n(sem_t id, unsigned n) {
    (void)id;
    (void)n;
    return -1;
}

int sem_signal_n(sem_t id, unsigned n) {
    (void)id;
    (void)n;
    return -1;
}

int time_sleep(time_t time) {
    (void)time;
    return -1;
}

char getc() {
    return EOF;
}

void putc(char c) {
    (void)c;
}
