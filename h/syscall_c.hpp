#ifndef _syscall_c
#define _syscall_c

//deklaracije sistemskih poziva koje ce javni testovi ocekivati

#include "hw.h"

class _thread;
typedef _thread* thread_t;

class _sem;
typedef _sem* sem_t;

// Memory
void* mem_alloc(size_t size);
int mem_free(void* ptr);

// Threads
int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();

// Semaphores
int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);

// Za sada ih samo deklarišemo zbog kompatibilnosti,
int sem_wait_n(sem_t id, unsigned n);
int sem_signal_n(sem_t id, unsigned n);

int time_sleep(time_t);

const int EOF = -1;
char getc();
void putc(char);

#endif
